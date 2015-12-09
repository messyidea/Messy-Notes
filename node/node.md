node install
===

download the tar.gz on the website
tar zxvf xxx.tar.gz

then soft link
ln -s /root/project/node-v5.2.0-linux-x64/bin/npm /usr/local/bin/npm
ln -s /root/project/node-v5.2.0-linux-x64/bin/node /usr/local/bin/node

there is a problem, when you install something using npm, you cannot use it.you should add env

add these to /etc/profile
export NODE_HOME=/alidata/node-v0.10.19-linux-x64
export PATH=$PATH:$NODE_HOME/bin
export NODE_PATH=$NODE_HOME/lib/node_modules

important:
npm install -g xxx
xxx is not install in the system, just in the software


