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
#include "blueparse.tab.h"
%}

%option yylineno

white [ \n\t]+
macid ([0-9A-Fa-f]{2}:){5}([0-9A-Fa-f]{2})
username [a-z_][a-z0-9_]{0,30}
devicename \"[^\n"]+\"
commentline "#"

%%

{white} { }
{macid} { yylval.word = strdup(yytext);
    return MAC;
}
{username} { yylval.word = strdup(yytext);
    return USERNAME;
}
{devicename} { int yytextlenght = strlen(yytext);
    char *d = (char *) malloc(sizeof(char) * (yytextlenght - 1));
    if (d == NULL)
        return DEVICENAME;
    strcpy(d, (yytext + 1));
    d[yytextlenght - 2] = '\0';
    yylval.word = d;
    return DEVICENAME;
}

{commentline} { char c;
    do {
        c = input();
    } while (c != '\n');
    yylineno++;
}

"," return COMMA;
"{" return OPENBRACE;
"}" return ENDBRACE;
"=" return EQUAL;
