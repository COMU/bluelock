#!/usr/bin/env bash
#/lib/security
set -e

echo "pam-bluelock"
cc -pthread -fPIC -fno-stack-protector -c pam_bluelock.c
ld -x --shared -lbluetooth -lcrypt -o ./pam_bluelock.so pam_bluelock.o ../blueconf/blueconf.o ../blueconf/lex.yy.o ../blueconf/blueparse.tab.o
