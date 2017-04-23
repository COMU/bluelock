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

%{
#include "blueconf.h"

int yywrap(void)
{
        return 1;
}

%}


%union {
    char *word;
}

/* Bison declarations */
%token OPENBRACE ENDBRACE COMMA EQUAL

%token <word>  MAC DEVICENAME USERNAME

%%
/* Grammar rules */
records:
       | records record
       ;

record: USERNAME OPENBRACE devices ENDBRACE { set_record($1); }
      ;

devices: devices DEVICENAME EQUAL MAC COMMA { append_device($2, $4); }
       | DEVICENAME EQUAL MAC COMMA { append_device($1, $3); }
       ;

%%
