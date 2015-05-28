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
padding:内容与边框线之间的距离
border: 盒子周边的直线
background-color： 填充边框内部空间，包括padding
margin: 一个标签与另一个标签的间隔

//从top开始顺时针
margin: 0 10px 10px 20px;
padding: 10px 5px 5px 10px;

//四边都用同样的值
margin:0

//margin 冲突会选用大的那个(bottom margin 和 top margin)，如果要取消冲突，可以换成padding

//行内盒子只能设置left/right padding/margin

//让块级元素和行内元素一样。
display: inline;
//让行内元素和块级元素一样
display: block;

//可以乱序
border: 4px solid #F00;

//用border-top/right/bottom/left可以控制每条边框
border-bottom: 2px dashed red

//强迫样式落至左浮动和右浮动元素下方
clear: both
```

图片相关
```
//图片平铺效果,可选repeat-x/y
background-repeat: no-repeat

//控制图片位置,比如移到右上角
background-position: right top;
//精确控制
background-position: 5px 8px;

//不滚动
background-attachment: fixed;

background: background-color background-image background-attachment background-position


```
