use Python to maintence the system
===

python调用ls
```python
import subprocess

subprocess.call(["ls","-l"])
#subprocess.call("ls -l",shell=True)
```

```
str.find('xxx')
#有返回index，没有返回-1
str.index('xxx')
#没有则报错


somestr.startswith('xxx')

somestr.endswith('xxx')

lstrip(),rstrip(),strip()
#删除空格，包括\t\n
lstrip('<')


#create file
infile = open("foo.txt", "r")
print infile.read()

from __future__import with_statement
with open("xx.txt", 'w') as f:
    f.write('xxx')

f.read(5)
f.readline()

#html file
import urllib
url_file = urllib.urlopen("http://www.baidu.com")
urllib_docs = url_file.read()



```

