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

#include <stdlib.h>
#include <string.h>

#include "blueconf.h"

Device* c_device(char *name, char *mac)
{
    Device *d = (Device *) malloc(sizeof(Device));
    d->name = name;
    d->mac = mac;
    d->next = NULL;
    return d;
}

Device* d_device(Device *d)
{
    Device *next = d->next;
    free(d->name);
    free(d->mac);
    free(d);
    return next;
}

void append_device(char *name, char *mac)
{
    if (rList->buffhead == 0)
        append_record(NULL);

    Record *r = rList->head;
    Device* d = c_device(name, mac);
    
    d->next = r->dListHead;
    r->dListHead = d;
    r->size++;
}

Record* c_record(char *username)
{
    Record *r = (Record *) malloc(sizeof(Record));
    r->username = username;
    r->dListHead = NULL;
    r->next = NULL;
    return r;
}

Record* d_record(Record *r)
{
    Device *next;
    next = r->dListHead;
    
    for (int i=0; i < r->size; ++i)
        next = d_device(next);

    Record *nextRecord = r->next;
    free(r->username);
    free(r);
    return nextRecord;
}


void append_record(char *username)
{
    Record *r = c_record(username);

    r->next = rList->head;
    rList->head = r;
    rList->buffhead = 1;
    rList->size++;
}

void set_record(char *username)
{
    rList->head->username = username;
    rList->buffhead = 0;
}

void c_record_list(RecordList **l)
{
    (*l) = (RecordList *) malloc(sizeof(RecordList));
    (*l)->size = 0;
    (*l)->head = NULL;
    (*l)->buffhead = 0;
}

void d_record_list(RecordList *l)
{
    Record *next;
    next = l->head;
    
    for (int i=0; i < l->size; ++i)
        next = d_record(next);

    free(l);
}

void print_dList(Device *d)
{
    if (d == NULL)
        return;
    printf(" - Device -> name = %s, mac = %s\n", d->name, d->mac);
    print_dList(d->next);
}

void print_rList(Record *r)
{
    if (r == NULL)
        return;
    printf("Record -> username = %s, size = %d\n", r->username, r->size);
    print_dList(r->dListHead);
    print_rList(r->next);
}

int is_match(RecordList *l, char *macaddr, const char *username)
{
    Record *r = l->head;
    while (r != NULL) {
        Device *dd = r->dListHead;
        while (dd != NULL && !strcmp(username, r->username)) {
            if (!strcasecmp(macaddr, dd->mac))
                return 1;
            else
                dd = dd->next;
        }
        r = r->next;
    }
    return 0;
}

extern char* yytext;
extern int yylineno;
int yyerror(char *s) {
    fprintf(stderr, "Bluelock[e]: %s on line %d - %s\n", s, yylineno, yytext);
}
	
extern FILE*  yyin;
void get_conf()
{
		c_record_list(&rList);

		yyin = fopen(ConfPath, "r");

		if (yyin == NULL)
			fprintf(stderr, "Bluelock[e]: Can not open configuration file.\n");

		if (!yyparse())
			fprintf(stderr, "Bluelock[i]: Successfully parsed.\n");

		fclose(yyin);
}

