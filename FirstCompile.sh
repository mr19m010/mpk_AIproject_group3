#!/bin/sh
aclocal
autoheader
touch NEWS ChangeLog
automake --add-missing
autoconf
automake
./configure
make clean
make

