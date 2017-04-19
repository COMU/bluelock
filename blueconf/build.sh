#!/usr/bin/env bash

set -e

echo "bison"
bison -d blueparse.y
echo "flex"
flex -8 bluelex.l
echo "libblueparse.so"
cc -fPIC -c blueparse.tab.c lex.yy.c blueconf.c
cc -shared -o libblueconf.so.1 blueparse.tab.o lex.yy.o blueconf.o
