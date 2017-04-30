/*
 *
 *  pam_bluelock - PAM module for authenticate with bluetooth devices
 *
 *  Copyright (C) 2017 Canberk Koç <canberkkoc@gmail.com>
 *  Copyright (C) 2017 Çağrı Ulaş <cagriulas@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdatomic.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <getopt.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <sys/param.h>
#include <pwd.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <crypt.h>

#include "../blueconf/blueconf.h"

#define BT_JOIN_ACTIVE 0
#define BT_JOIN_INACTIVE -1

atomic_int bt_join_state = BT_JOIN_INACTIVE;

static int find_conn(int s, int dev_id, long arg)
{
	struct hci_conn_list_req *cl;
	struct hci_conn_info *ci;
	int i;

	if (!(cl = malloc(10 * sizeof(*ci) + sizeof(*cl)))) {
		perror("Can't allocate memory");
		exit(1);
	}
	cl->dev_id = dev_id;
	cl->conn_num = 10;
	ci = cl->conn_info;

	if (ioctl(s, HCIGETCONNLIST, (void *) cl)) {
		perror("Can't get connection list");
		exit(1);
	}

	for (i = 0; i < cl->conn_num; i++, ci++)
		if (!bacmp((bdaddr_t *) arg, &ci->bdaddr)) {
			free(cl);
			return 1;
		}

	free(cl);
	return 0;
}

typedef struct {
    int *retval;
    pam_handle_t *pamh;
    int flags;
    int argc;
    const char **argv;
    int (*smauth)(pam_handle_t*, int, int, const char **);
} AuthCallerArguments;

typedef struct {
    int *retval;
    const char *username;
    RecordList *rList;
} BluetoothSeekerArguments;

void *bluetooth_seeker( void *bthreadargs );
void *auth_caller( void *threadargs );

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc,
                              const char **argv)
{
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc,
                                const char **argv)
{
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,int argc,
                                   const char **argv)
{
    pthread_t thread_bluetooth_seeker, thread_auth_caller;
    int retval_bluetooth_seeker = -1;
    int retval_auth_caller = -1;
    int user_retval;
    const char *username = NULL;
    extern RecordList *rList;
    
    user_retval = pam_get_user(pamh, &username, NULL);
    if (user_retval != PAM_SUCCESS)
        return PAM_USER_UNKNOWN;

    void* libunix = dlopen("pam_unix.so", RTLD_NOW);
    int (*smauth)(pam_handle_t*, int, int, const char **);
    smauth = dlsym(libunix,"pam_sm_authenticate");

    get_conf();

    AuthCallerArguments threadargs = {&retval_auth_caller, pamh, PAM_SILENT, argc, argv, smauth};
    BluetoothSeekerArguments bthreadargs = {&retval_bluetooth_seeker, username, rList};
    
    int ret1 = pthread_create(&thread_bluetooth_seeker, NULL, bluetooth_seeker, (void*) &bthreadargs);
    if (ret1) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", ret1);
        return PAM_SYSTEM_ERR;
    }

    int ret2 = pthread_create(&thread_auth_caller, NULL, auth_caller, (void*) &threadargs);
    if (ret2) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", ret2);
        return PAM_SYSTEM_ERR;
    }

    while (1) {
        if (retval_auth_caller != PAM_SUCCESS && retval_auth_caller != -1) {
            //pthread_join(thread_bluetooth_seeker, NULL);
            pthread_cancel(thread_bluetooth_seeker);
            pthread_cancel(thread_auth_caller);
            dlclose(libunix);
            d_record_list();
            return retval_auth_caller;
        }
        if (retval_bluetooth_seeker == PAM_SUCCESS) {
            pthread_cancel(thread_auth_caller);
            //pthread_cancel(thread_bluetooth_seeker);
            //pthread_join(thread_bluetooth_seeker, NULL);
            dlclose(libunix);
            d_record_list();
            return PAM_SUCCESS;
        }
        if (retval_auth_caller == PAM_SUCCESS) {
            //pthread_cancel(thread_bluetooth_seeker);
            //pthread_join(thread_bluetooth_seeker, NULL);
            pthread_join(thread_auth_caller, NULL);
            dlclose(libunix);
            d_record_list();
            return PAM_SUCCESS;
        }
        sleep(0.001);
    }
}

void *bluetooth_seeker(void *bthreadargs)
{
    syslog(LOG_AUTH, "bluetooth_seeker: started");

    BluetoothSeekerArguments *bargs = bthreadargs;
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, search_len, flags;
    int i;
    char addr[19] = {0};
    char name[248] = {0};
    RecordList *rl = bargs->rList;
    int *retv = (int*)(bargs->retval);
	int bluetoothDevice, bluetoothDeviceId;

	struct hci_conn_info_req *cr;
	int8_t rssi;
	uint16_t handle_rssi;
	uint8_t role_rssi;
	unsigned int ptype_rssi;
	role_rssi = 0x01;
	ptype_rssi = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;


    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0) {
        perror("Opening socket");
        pthread_exit(NULL);
    }

    search_len  = 2;
    max_rsp = 32;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*) malloc(max_rsp * sizeof(inquiry_info));
    while(*retv != PAM_SUCCESS) {
        num_rsp = hci_inquiry(dev_id, search_len, max_rsp, NULL, &ii, flags);
        
        for (i = 0; i < num_rsp; i++) {
            ba2str(&(ii+i)->bdaddr, addr);
            syslog(LOG_AUTH, "bluetooth_seeker: %s", addr);
            memset(name, 0, sizeof(name));
            
            if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
                strcpy(name, "[unknown]");
                
            if(is_match(rl, addr, bargs->username)) {

				bluetoothDeviceId = hci_get_route(&(ii+i)->bdaddr);
				bluetoothDevice = hci_open_dev(bluetoothDeviceId);
				
				if(bluetoothDevice<0){
					syslog(LOG_AUTH, "RSSI FAIL");
				}
				if(ioctl(bluetoothDevice, HCIGETCONNINFO, (unsigned long) cr) < 0){
					if (hci_create_connection(bluetoothDevice, &(ii+i)->bdaddr, htobs(ptype_rssi),
								htobs(0x0000), role_rssi, &handle_rssi, 25000) < 0){
								perror("Can't create connection");}
				}
				cr = malloc(sizeof(*cr) + sizeof(struct hci_conn_info));

				if (!cr) {
					perror("Can't allocate memory");
					
				}

				bacpy(&cr->bdaddr, &(ii+i)->bdaddr);
				cr->type = ACL_LINK;
				if (ioctl(bluetoothDevice, HCIGETCONNINFO, (unsigned long) cr) < 0) {
					perror("Get connection info failed");
				}

				if (hci_read_rssi(bluetoothDevice, htobs(cr->conn_info->handle), &rssi, 1000) < 0) {
					perror("Read RSSI failed");
				}

				free(cr);

				hci_close_dev(bluetoothDevice);
	
				if((int)rssi==0){
                	*retv = PAM_SUCCESS;
                	break;
				}
            }
        }

        if (bt_join_state == BT_JOIN_ACTIVE)
            break;
    }
    free(ii);
    if (!close(sock))
        perror("Closing socket");
    hci_close_dev(dev_id);
    syslog(LOG_AUTH, "bluetooth_seeker: completed");
    pthread_exit(NULL);
}

void *auth_caller(void *threadargs)
{
    syslog(LOG_AUTH, "auth_caller: started");

    AuthCallerArguments *args = threadargs;
    int *retv = (int*) (args->retval);
    int (*smauth) (pam_handle_t*, int, int, const char **) = args->smauth;
    *retv = smauth(args->pamh, args->flags, args->argc, args->argv);
    bt_join_state = BT_JOIN_ACTIVE;
    syslog(LOG_AUTH, "auth_caller: completed");
    pthread_exit(NULL);
}
