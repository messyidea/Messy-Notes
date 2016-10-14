#!/bin/bash

#from https://github.com/lancethepants/dnscrypt-mipsel-static.git, change something out of date

set -e
set -x

mkdir ~/dnscrypt && cd ~/dnscrypt

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
mkdir -p $SRC

############# ###############################################################
# LIBSODIUM # ###############################################################
############# ###############################################################

mkdir -p $SRC/sodium && cd $SRC/sodium
$WGET https://github.com/jedisct1/libsodium/releases/download/1.0.11/libsodium-1.0.11.tar.gz
tar zxvf libsodium-1.0.11.tar.gz
cd libsodium-1.0.11

LDFLAGS=$LDFLAGS \
CPPFLAGS=$CPPFLAGS \
CFLAGS=$CFLAGS \
CXXFLAGS=$CXXFLAGS \
$CONFIGURE \
--enable-minimal \
--enable-static \
--disable-shared

$MAKE
make install DESTDIR=$BASE

############ ################################################################
# DNSCRYPT # ################################################################
############ ################################################################

mkdir $SRC/dnscrypt && cd $SRC/dnscrypt
$WGET https://download.dnscrypt.org/dnscrypt-proxy/dnscrypt-proxy-1.7.0.tar.gz
tar zxvf dnscrypt-proxy-1.7.0.tar.gz
cd dnscrypt-proxy-1.7.0

LDFLAGS="-Wl,-static -static -static-libgcc -s $LDFLAGS" \
CPPFLAGS=$CPPFLAGS \
CFLAGS=$CFLAGS \
CXXFLAGS=$CXXFLAGS \
$CONFIGURE

$MAKE
make install DESTDIR=$BASE/dnscrypt
