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

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct device {
    char *name;
    char *mac;

    struct device *next;
} __attribute__ ((packed)) Device;

typedef struct record {
    char *username;
    uint size;
    Device *dListHead;

    struct record *next;
} __attribute__ ((packed)) Record;

typedef struct {
    uint size;
    Record *head;
    int buffhead;
} __attribute__ ((packed)) RecordList;

#ifdef __cplusplus
}
#endif

#endif // STRUCTURES_H_
