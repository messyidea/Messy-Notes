CSS
===

文本格式化
```
//设置字体，如果没有Arial字体，则使用Helvetica ...
font-family: Arial, Helvetica, sans-serif;

//给文本添加颜色
color: #3E8988
//rgb形式
color: rgb(100%, 100%, 100%);
//or
color: rgb(255, 255, 255);

//修改字号,数字和度量之间不能有空格，度量还可以是px, Keyword, Percentage
font-size: 1em;
//em和percentage一样，会受到继承影响。

//斜体
font-style: italic;
//not italic
font-style: normal

//粗体
font-weight: bold;

//非粗体
font-weight: normal;

//大写, 其它选项:lowercase, capitalize
text-transform: uppercase;

//不改变大小写
text-transform: none;

//文本修饰: underline, overline, line-through, blink
text-decoration: underline;

//减少字母之间的间隙
letter-spacing: -1px;

//增大单词间距,不改变字母间的间距
word-spacing: 2px;

//行间距, 也可以用em之类的单位
line-height: 150%;

//对齐文本, 可选参数: left, right, justify, center
text-align: center;

//首行缩进
text-indent: 5em

//font,最后两种属性必须是font-size和font-family
font: font-style font-variant font-weight font-size font-height font-family

//消除段落间的间隙
p {
    margin-top: 0;
    margin-bottom: 0;
}

//格式化段落首字母或者首行.首行用first-line
p:first-letter {
    font-weight: bold;
    color: red;
}


//ul标签
list-style-type: square;
list-style-type: upper-alpha;

//项目符号与序号的定位
list-style-position: outside;

//自定义图片
list-style-image: url(images/xxx.gif)

```


Margin, Padding, Border
```

```
