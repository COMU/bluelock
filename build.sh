#!/usr/bin/env bash

set -e

cd ./blueconf
./build.sh
cd ../pam-bluelock
./build.sh
echo "copying.."
sudo mkdir -p /lib/security/
sudo cp ./pam_bluelock.so /lib/security/pam_bluelock.so

echo "cleaning.."
rm ../blueconf/{blueparse.tab.o,lex.yy.o,blueconf.o,blueparse.tab.c,blueparse.tab.h,lex.yy.c} ./pam_bluelock.o
