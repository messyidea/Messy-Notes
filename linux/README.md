linux api
===


创建目录
```c
#include <sys/stat.h>
#include <sys/types.h>
int result = mkdir(char *pathname, mode_t mode);
```

删除目录
```c
#include<unistd.h>
int result = rmdir(const char *path);
```

删除链接
```c
#include <unistd.h>
int result = unlink(const char *path);
```

创建一个文件链接
```c
#include <unistd.h>
int result = link(const char *orig, const char *new);
```

重命名或删除一个链接
```c
#include <unistd.h>
int result = rename(const char *from, const char *to);
```

cd
```c
#include <unistd.h>
int result = chdir(const char *path);
```

控制文件描述符
```c
#include <fcntl.h>
#incldue <unistd.h>
#include <sys/types.h>
int result = fcntl(int fd, int cmd);
int result = fcntl(int fd, int cmd, long arg);
int result = fcntl(int fd, int cmd, struct flock *lockp);
```

驱动器属性
```c
//读取
int result = tcgetattr(int fd, struct termios *info);
//设置
int result = tccsetattr(int fd, int when, struct termios * info);
```

控制一个设备
```c
#include <sys/ioctl.h>
int result = ioctl(int ft, int  operation[,arg...]);
```

信号处理
```c
#include <signal.h>
int result = signal(int signum, void (*action)(int));
```

指定一个信号处理函数
```c
#include <signal.h>
res = sigaction(int signum, const struct sigaction *action, struct sigaction *prevaction);
```

在指定路径中查找并执行一个文件
```c
#include <unistd.h>
result = execvp(const char * file, const char * argv[])
```

创建进程
```c
#include <unistd.h>
pid_t result = fork(void);
```

等待进程结束
```c
#include <sys/types.h>
#incldue <sys/wait.h>
pid_t result = wait(int * statusptr);

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```


```c

```

