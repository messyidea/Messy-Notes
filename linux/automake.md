Automake
===

###install
```
apt-get install automake 
```

###流程
+ 创建一个hello.c
+ 执行autoscan
+ 以configure.scan为蓝本，编写configure.in
+ 执行aclocal
+ 执行autoconf
+ 执行autoheader
+ 编写Makefile.in
+ 执行automake --add-missing

生成的东西可以直接执行configure,make和make install 了。
