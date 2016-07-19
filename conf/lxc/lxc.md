lxc
===

linux container


see the version
```
lxc-create --version
```

judge if the kernel support lxc
```shell
lxc-checkconfig 
```


see all the templates
```shell
ls /usr/share/lxc/templates/
```

create (debian for example)
```shell
sudo lxc-create -n <container-name> -t debian
```

now container is at `/var/lib/lxc/<container-name>`, the root dir is at `/var/lib/lxc/<container-name>/rootfs`

start container
```shell
sudo lxc-start -n <container-name> -d 
```

see the status of containers
```shell
sudo lxc-ls --fancy 
```

attach a container
```shell
sudo lxc-console -n <container-name> 
```

you can exit the container by key `Crtl+a q`

stop and destory the container
```shell
 sudo lxc-stop -n <container-name>
 sudo lxc-destroy -n <container-name> 
```

clone the container
```shell
sudo lxc-stop -n <container-name>
sudo lxc-clone -o <container-name> -n <new-container-name>
```

freeze and unfreeze the container
```
lxc-freeze -n <container-name>
lxc-unfreeze -n <container-name>
```

network
```
root@test:/etc/lxc# cat /etc/lxc/default.conf 
lxc.network.type = veth
lxc.network.link = lxcbr0
lxc.network.flags = up
lxc.network.hwaddr = 00:16:3e:xx:xx:xx


root@test:/var/lib/lxc/lxc# cat /var/lib/lxc/lxc/config 
# Template used to create this container: /usr/share/lxc/templates/lxc-debian
# Parameters passed to the template:
# For additional config options, please look at lxc.container.conf(5)

# Uncomment the following line to support nesting containers:
#lxc.include = /usr/share/lxc/config/nesting.conf
# (Be aware this has security implications)

lxc.network.type = veth
lxc.network.link = lxcbr0
lxc.network.flags = up
lxc.network.hwaddr = 00:16:3e:9c:ad:e8
lxc.rootfs = /var/lib/lxc/lxc/rootfs
lxc.rootfs.backend = dir

# Common configuration
lxc.include = /usr/share/lxc/config/debian.common.conf

# Container specific configuration
lxc.tty = 4
lxc.utsname = lxc
lxc.arch = amd64
```


errors
===

when create an arch
```
/usr/share/lxc/templates/lxc-archlinux: line 53: pacman: command not found
```
see http://askubuntu.com/questions/610081/how-to-create-archlinux-lxc


old version lxc will not add network interface automatically.




