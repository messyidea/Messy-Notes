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

