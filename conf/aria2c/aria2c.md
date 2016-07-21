aria2c static build with openssl support
===

# environment :
https://blog.messyidea.com/archives/43/


# build openssl:
wget https://www.openssl.org/source/openssl-1.0.1t.tar.gz
tar zxvf xxxxxxxx
./Configure -DOPENSSL_NO_HEARTBEATS --openssldir=/root/rpi/opt shared os/compiler:arm-linux-gnueabihf-gcc 

Configure params see https://wiki.openssl.org/index.php/Compilation_and_Installation


# build aria2c:
download the aria2c source code
tar zxvf xxxx
./configure --host=arm-linux-gnueabihf --without-libxml2 ARIA2_STATIC=yes --enable-static --prefix=/root/rpi/opt  --enable-shared=no PKG_CONFIG_PATH="/root/rpi/opt/lib/pkgconfig" LDFLAGS="-L/root/rpi/opt/lib"   --with-ca-bundle='/etc/ssl/certs/ca-certificates.crt'



# usefully for other dependance:
./configure --host=arm-linux-gnueabihf --enable-static --prefix=/root/rpi/opt  --enable-shared=no PKG_CONFIG_PATH="/root/rpi/opt/lib/pkgconfig" LDFLAGS="-L/root/rpi/opt/lib"  

