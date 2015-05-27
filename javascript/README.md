JavaScript
===

编码和解码，解决非ASC码问题
```
encodeURI 用 decodeURI解码
encodeURIComponent 用 decodeURIComponent解码
```

杂
```
parseInt  返回整数部分
parseFloat  返回浮点数
Infinity    无限大
parseInt("55", 8)   进制转换，8进制的55转化为10进制，输出45
null    不存在
undefined   未定义
== 值相等
=== 值相等，类型相同，恒等
多重判断也遵循短路原则

for (var prop in MyText) { ;}   简写for

String.substr() 两个参数分别为起始位置和长度
String.substring()  两个参数分别为起始位置和结束位置

substring(a,b)  a可以大于b
slice(a,b) 和substring类似，但是a>b返回的是空字符串

re.test(str)    //re为RegExp对象，str为字符串

var re = new RegExp("xxxx", "ig")   i表示忽略大小写，g代表全局匹配。

Array.splice(2,2,'xx1','xx2') 在2位置删去两个元素，加入两个元素'xx1','xx2'

Array: push 加入到队尾， unshift 加入到数组开始。两者都返回数组长度。
pop与push相反，shift和unshift相反。

```

函数
```
函数是传值不是传引用
```

事件
```
多个事件共同触发。可以用类似stopPropagation方法来暂停相应相关低层事件。
```


