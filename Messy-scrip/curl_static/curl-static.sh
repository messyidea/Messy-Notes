#!/bin/bash

set -e
set -x

mkdir ~/mycurl && cd ~/mycurl

BASE=`pwd`
SRC=$BASE/src
WGET="wget --prefer-family=IPv4"
DEST=$BASE/opt
LDFLAGS="-L$DEST/lib -Wl,--gc-sections"
CPPFLAGS="-I$DEST/include"
CFLAGS="-O3 -ffunction-sections -fdata-sections"
CXXFLAGS=$CFLAGS
CONFIGURE="./configure --prefix=/opt --host=mips-unknown-linux-uclibc"
MAKE="make -j`nproc`"

mkdir $SRC

######## ####################################################################
# ZLIB # ####################################################################
######## ####################################################################

mkdir $SRC/zlib && cd $SRC/zlib
$WGET http://zlib.net/zlib-1.2.8.tar.gz
tar zxvf zlib-1.2.8.tar.gz
cd zlib-1.2.8

LDFLAGS=$LDFLAGS \
CPPFLAGS=$CPPFLAGS \
CFLAGS=$CFLAGS \
CXXFLAGS=$CXXFLAGS \
CROSS_PREFIX=mips-unknown-linux-uclibc- \
./configure \
--prefix=/opt \
--static

$MAKE
make install DESTDIR=$BASE

########### #################################################################
# OPENSSL # #################################################################
########### #################################################################

mkdir -p $SRC/openssl && cd $SRC/openssl
$WGET https://www.openssl.org/source/openssl-1.0.2h.tar.gz
tar zxvf openssl-1.0.2h.tar.gz
cd openssl-1.0.2h

./Configure linux-mips32 \
-ffunction-sections -fdata-sections -Wl,--gc-sections \
--prefix=/opt zlib \
--with-zlib-lib=$DEST/lib \
--with-zlib-include=$DEST/include

make CC=mips-unknown-linux-uclibc-gcc
make CC=mips-unknown-linux-uclibc-gcc install INSTALLTOP=$DEST OPENSSLDIR=$DEST/ssl

############# ###############################################################
# SOFTETHER # ###############################################################
############# ###############################################################

mkdir $SRC/curl && cd $SRC/curl

git clone https://github.com/curl/curl.git
cd curl
./buildconf

LDFLAGS=$LDFLAGS \
CPPFLAGS=$CPPFLAGS \
CFLAGS=$CFLAGS \
CXXFLAGS=$CXXFLAGS \
$CONFIGURE \
--with-openssl=$DEST \

$MAKE LIBS="-all-static -lcrypto -lm -ldl -lz"
make install DESTDIR=$BASE/curl
