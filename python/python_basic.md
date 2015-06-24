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

#map and reduce
```python
def add(x,y):
    return x+y
reduce(add, [1,3,5,7,9])
#返回25

def str2int(s):
    def fn(x,y):
        return x * 10 + y
    return reduce(fn,map(int,s))

```

#排序
```python
sorted([1,3,2,5,4])
#也可以类似c++ STL一样自定义大小
sorted([1,2,4],cmp)     #cmp是一个函数

```

#匿名函数
```python
map(lambda x: x*x, [1,2,3])
#返回[1,4,9]

f = lambda x : x*x
```

#decorator
```python
def log(func):
    def wrapper(*args, **kw):
        print 'call %s():' %func.__name__
        return func(*args, **kw)
    return wrapper
#log是一个decorator,接受一个函数作为参数，返回一个函数

@log
def now():
    print 'xx'

#相当于now = log(now)
def log(func):
    @functools.wraps(func)
    def wrapper(*args, **kw):
        print 'call %s():' %func.__name__
        return func(*args, **kw)
    retrun wrapper
#上面写的decorator可以保证__name__是func

```


#偏函数
```python
import functools
int2 = functools.partial(int, base=2)
```

#模块
```python
#导入模块加别名
import cStringIO as StringIO
```

#类
```python
class Student(object):  #Student类继承object
    pass
st1 = Student()
st1.name = 'xx'         #可以自由给对象绑定属性
#必须绑定的属性要在__init__方法里面写进去
    def __init__(self,name):    #第一个必须是self
        self.name = name
#对象拥有的变量名称可能不同
#属性前加__就代表私有
```

#获取对象信息
```python
type(xx)
isinstance(h,type)      #适用于多态
isinstance(h, (type1,type2))
获取对象的所有属性和方法
dir('ABC')
hasattr(obj, 'attrname')
getattr(obj, 'attrname')
setattr(obj, 'attrname', value)

from types import MethodType
s.func = MethodType(func, s, Student)   #给对象绑定一个方法，func要事先定义好

#给class绑定方法
Classname.func = MethodType(func, None, Classname)

#限制class能添加的属性
__slots__=('xx1','xx2')     #只能添加xx1,xx2属性，只对当前类有效，对继承的子类无效

#@property装饰器可以把一个方法变成属性调用的
#Mixin(类似于接口？)，虽然python支持多重继承，但是这样好像更好一点
```

#定制类
```python
__str__         #相当于java中的toString()
__repr__ = __str__
__iter__
__getitem__(self, n)
__getattr__(self, attr)
__call__(self)

callable(xx)    #判断xx对象能否被调用
```

#元类
```python
Hello = type('Hello', (object,), dict(hello=fn))
#第一个参数是class名称，第二个参数是继承的父类集合，第三个参数是方法名称与函数的绑定

#Metaclass,怎么编写ORM?
```

#错误处理
```python
try:
    do some thing
except ValueError,e:
    do some thing
finally:
    do some thing

#抛出错误
raise FooError(xx)
```

#调试
```python
#断言
assert n != 0, 'n is zero!'
python -o err.py    #关闭assert

import logging
logging.basicConfig(level=logging.INFO) #基本设置
logging.info('xx')


python -m pdb err.py    #启动调试工具
l       #查看代码
n       #单步执行
p s     #查看s变量
q       #结束调试


#单元测试
setUp(self)
tearDown(self)

#doctest
```

#IO编程
```python
f = open('/usr/a.txt', 'r')
f.read()
f.close()

#用with不用关闭文件，会自动关闭
with open('/usr/a.txt', 'r') as f:
    print f.read()

#不一次性读取
for line in f.readlines()
    print(line.strip())         //把末尾的\n去掉

#自动转码
import codecs
with codecs.open('/usr/a.txt', 'r', 'gbk') as f:
    f.read()

#写文件
f = open('a.txt', 'w')
f.write('hello, world')
f.close
```

#操作目录
```python
import os
os.name
os.uname()
os.environ      #环境变量
os.getenv('PATH')
#查看绝对路径
os.path.abspath('.')
os.path.join('/usr', 'newdir')  #生成路径
os.mkdir('xx')    #xx是路径，创建目录
os.rmdir('xx')
os.path.split('xx')     #可以把文件和目录拆分开来
os.path.splitext('xx')  #把文件和扩展名拆分
os.rename('a.txt', 'b.txt')
os.remove('a.txt')
```

#序列化
```python
try:
    import cPickle as pickle
except ImportEroor:
    import pickle

pickle.dumps(d)     #d是一个对象

#dump入文件
f = open('dump.txt', 'wb')
pickle.dump(d, f)
f.close()

#反dump
f = open('dump.txt', 'rb')
d = pickle.load(f)
f.close()
```

#JSON
```python
Python类型
{}
dict
[]
list


import json
json.dumps(d)
json.loads(json_str)

#序列化类
Student类里面写一个student2dict函数
json.dumps(s, default = student2dict)
json.dumps(s, default = lambda obj:obj.__dict__)
json.loads(json_str, object_hook = dict2student)
```

#多进程
```python
def run_func(name):
    pass
p = Process(target = run_func, args = ('test',))
p.strat()
p.join()        #相当于linux中的wait

#进程池
from multiprocessing import Pool
import os, time, random
def long_time_task(name):
    do some thing
#...main里面
p = Pool()          #默认进程池只有4个进程，可以Pool(5)
for i in range(5):
    p.apply_async(long_time_task, args = (i,))
p.close()   #join之前一定要调用close
p.join()

#进程通信Queue(当然linux下还有Pipes)
from multiprocessing import Process, Queue
import os, time, random
q.put(value)
q.get(True)

```

#多线程
```python
import threading
t = threading.Thread(target = loop, name = 'LoopThread')
t.start()
t.join()

#锁
lock = threading.Lock()
lock.acquire()  #获取锁
lock.release()  #释放锁
#python解释器由于设计时有GIL全局锁，导致多线程没法使用多核。可以用多进程使用多核

#ThreadLocal
local_school = threading.local()
local_school.student = name     #绑定ThreadLocal的student

```

#分布式进程
```python
import Queue
from multiprocessing.managers import BaseManager
task_queue = Queue.Queue()
QueueManager.register('get_task_queue', callable = lambda:task_queue)    #注册到网络上
manager = QueueManager(address=('',5000), authkey='abc')        #绑定端口5000，验证码为abc
manager.start()

task = manager.get_task_queue() #获取Queue
task.put(value)
manager.shutdown()

#另一台机器
QueueManager.register('get_task_queue')
server_addr = '127.0.0.1'
m = QueueManager(address=(server_addr,5000), authkey='abc')
m.connect()
task = m.get_task_queue()
```

#正则表达式
```python
\d{3}           #匹配3个数字
\s              #匹配一个空格
\d{3,8}         #匹配3-8个数字
[0-9a-zA-Z\_]{3,8}   #...
^               #开头
$               #结尾

import re
re.match(r'xxx','xxx')

#切分字符串
'ab  c'.split(' ')  #无法识别连续的空格
re.split(r'\s+','ab  c')    #贪婪匹配
#()可以进行分组，分组之后用group取得
#'+'后面加一个'?'采用非贪婪匹配

#预编译
re_telephone = re.compile(r'xxx')
re_telephone.match(xxx).groups(xx)
```

#常用模块
```python
base64      #网页常用，用64个字符表示任意二进制数据

import base64
base64.b64encode('xx')
base64.b64decode('xx')

struct      #把任意数据变成字符串
struct.pack('xx', 'xxx')
struct.unpack('xx', 'xxx')

hashlib     #哈希
md5 = hashlib.md5()
md5.update('xxx')
md5.hexdigest()

from xml.parser.expat import ParserCreate    #可以解析xml

HTMLParser          #解析html

```

#网络编程
```python
import socket
s = socket.socket(socke.AF_INET, socket.SOCK_STREAM)
s.connect(('www.baidu.com', 80))    #注意参数是tuple
s.send('xxx')

s.recv(1024)
data = ' '.join(buffer)
s.close()

#......
```

#SQL
```python
#sqlite
import sqlite3

conn = sqlite3.connect('test.db')
cursor = sonn.cursor()
cursor.execute('sql')
cursor.close()
cursor.commit()
conn.close()

#mysql
import mysql.connector
conn = mysql.connector.connect(user = 'root', password = 'password', database = 'test', use_unicode = True)
cursor = conn.cursor()
cursor.execute('sql')
cursor.commit()
cursor.close()

#ORM框架，SQLAlchemy...
```

#gevent 协程
```python
from gevent import monkey; monkey.patch_socket()
import gevent

def func(n):
    xxx

g1 = gevent.spawn(func, 1)
g2 = gevent.spawn(func, 1)
g1.join()
g2.join()
#g1和g2会依次运行。func在进行io操作或者sleep的时候会让其他的实例执行
```

#pack and unpack
```python
#struct模块中最重要的三个函数是pack(), unpack(), calcsize()
pack(fmt, v1, v2, ...)
#按照给定的格式(fmt)，把数据封装成字符串(实际上是类似于c结构体的字节流)
unpack(fmt, string)
#按照给定的格式(fmt)解析字节流string，返回解析出来的tuple
calcsize(fmt)
#计算给定的格式(fmt)占用多少字节的内存

#string
string.maketrans(from, to)
#建立字符映射表
string.translate(s, table[, deletechars])
str.translate(table[, deletechars])
#解析字符串
```


