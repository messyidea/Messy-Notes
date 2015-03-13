#list
```python
classmates=['jack','Bob']
len(classmates)
#list元素也可以是list
list.append('xx')
list.insert(1,'xx')
list.pop()  #删除最后一个元素
list.pop(1)
```

#tuple, list的[]变为(),元素不可变
```python
classmates=('jack','Bob')
#定义一个元素的tuple
t = (1,)
```

#if
```python
if xx:
    do some thing
elif xx:
    do some thing
else:
    do some thing
```

#关于raw_input
```python
a = int(raw_input('xx'))
#如果输入数字要强转
```

#dict
```python
d={'a':100,'b':75,'c':50}
d.get('a')      #找不到返回None
d.get('a',x)    #找不到返回x
d.pop('a')      #删除
#dict的key必须是不可变的
```

#set
```python
s = set([1,2,3])
s.add(4)
s.remove(4)
s1 & s2     #set可以做交集并集
```

#函数
```python
def my_abs(x):
    if x >= 0:
        return x
    else:
        return -x

#空函数
def nop():
    pass
#类型检查
def my_abs(x):
    if not isinstance(x,(int,float)):
        raise TypeError('bad operand type')
    #do some thing
#返回多个值
def func()
    return 1,2

#默认参数必须指向不变的对象！不然函数体可能会变

#可变参数
def getsum(*numbers):
    sum = 0
    for n in numbers:
        sum = sum + n * n
    return sum

#关键字参数
def person(name, age, **kw):
    print 'name:',name,'age:',age,'other:',kw

#参数可以组合，顺序必须是：必选参数，默认参数，可变参数，关键字参数
def func(a, b, c = 0,*args, **kw):
    xxx
#*args是可变参数，接收的是tuple
#**kw是关键字参数，接收的是dict
```

#切片
```python
L[0:3]  #取0,1,2这三个元素
L[:3]   #同上
L[:10:2]    #从0到10，每2个取一个
```

#迭代
```python
d = {'a':1,'b':2,'c':3}
for key in d:
    print key
for value in d.itervalues():
    xxx

#判断是否可迭代
from collections import Iterable
isinstance('abc',Iterable)  #'abc'是否可迭代
```

#列表生成式
```python
range(1,11)
[x * x for x in range(1,11)]
[x * x for x in range(1,11) if x % 2 == 0]
[m + n for m in 'ABC' for n in 'XYZ']
#列出当前所有文件和目录名
import os
[d for d in os.listdir('.')]

d={'a':1,'b':2}
[k + '=' + v for k, v in d.iteritems()]

[s.lower() for s in L]
```

#生成器,按照函数先生成一部分
```python
g = (x * x for x in range(10))
#和列表生成式区别是它是小括号
g.next()        #获取一个
for n in g:     #取出所有
    print n

#把函数改造为generator,注意yield,每次执行到yield就中断
def fib(max):
    n,a,b = 0,0,1
    while n < max:
        yield b
        a,b = b,a+b
        n = n + 1

```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```

#
```python
```


