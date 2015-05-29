redis notes
===

redis使用一个sds来表示一个字符串
```
struct sdshdr {
    int len;
    int free;
    char buf[];
}
```

###不用C字符串的原因
- C字符串不能满足要求,是因为C字符串不能够记录自身的长度,获取其长度的复杂度是O(n)
sdshdr用空间换时间,直接用len标记了字符串的长度
- C字符串不记录自身长度,容易造成缓冲区溢出,strcat会假定第一个参数有足够多的内存,sds则会自动检查并扩展字符串大小
- C字符串因为没有记录长度,所以不能保存'\0',而sds是二进制安全的


不停地重分配内存性能会损失很多,redis的sds实现了空间预分配和惰性空间释放两种优化策略
- 预分配
    - sds如果修改之后小于1mb,则分配和len属性同样大小的未使用空间
    - 如果修改之后,大于等于1mb,程序会分配1mb未使用空间

- 惰性空间释放
    - sdstrim之后不释放空间


```
sds - sizeof(struct sdshdr) 就是sdshdr地址
realloc(sh, sizeof(struct sdshdr)+sh->len+1);//释放free的内存

```


to line 555




