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


```

