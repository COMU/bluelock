/*
 *  Copyright (C) 2017 Canberk Koç <canberkkoc@gmail.com>
 *  Copyright (C) 2017 Çağrı Ulaş <cagriulas@gmail.com>
 *
 *  This file is part of libblueconf.
 *
 *  libblueconf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libblueconf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libblueconf.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RUN_H_
#define RUN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "blueparse.tab.h"

static const char ConfPath[] = "/etc/bluelock.conf";

typedef struct device {
    char *name;
    char *mac;

    struct device *next;
} __attribute__ ((packed)) Device;

typedef struct record {
    char *username;
    unsigned int size;
    Device *dListHead;
    
    struct record *next;
} __attribute__ ((packed)) Record;

typedef struct {
    unsigned int size;
    Record *head;
    int buffhead;
} __attribute__ ((packed)) RecordList;

RecordList *rList;
Record *deviceBuffer;

void set_record(char *);
void append_record(char *);
void append_device(char *, char *);
void c_record_list(RecordList **);
void d_record_list(RecordList *);

void print_dList(Device *);
void print_rList(Record *);

int is_match(RecordList *, char *, const char *);
void get_conf();

extern int yyparse(void);

#ifdef __cplusplus
}
#endif

#endif // RUN_H_
