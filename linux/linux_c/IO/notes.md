IO
===

linux lock
lockf() and fcntl()

```
struct flock lock;
lock.l_whence = SEEK_SET;
lock.l_start = 0;
lock.l_len = 0;
lock.l_type = type;
lock.l_pid = -1;

//judge if locked
fcntl(fd, F_GGETLK, &lock);

```

IO多路复用
select 存在内存需要检查多余的文件描述符的缺点，而且每次调用select之后必须重置监听的文件描述符集
poll比select效率高
