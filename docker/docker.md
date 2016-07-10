docker
===

install
```
wget -qO- https://get.docker.com/ | sh
```

see the docker info
```
docker info
```

create and run a ubuntu container
```
docker run -t -i ubuntu /bin/bash
```

show runs
```
docker ps -a
```

run after exit
```
docker start [id]
docker attach [id]

# or in one command
docker start -a -i [id]
```

exec
```
docker exec -i -t [id] [command]
```

rm a container
```
docker rm [id]

# rm all
docker rm $(docker ps -q -f status=exited)
```

show the images
```
docker images
```

delete images
```
docker rmi [id]
#id 不用打全
如果出现conflit，则删除相应的正在运行的机器
```
