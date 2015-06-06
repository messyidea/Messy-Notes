Shell
===

```
count=5     //赋值等于号前后不能有空格
echo $count

//系统变量
$n      第n个参数
$#      命令行参数个数
$0      当前程序名称
$?      前一个命令或者函数的返回值
$*      以"参数1 参数2..."形式保存所有的参数
$@      以"参数1"、"参数2"形式保存所有的参数
$$      本程序的pid

//常用的环境变量
PATH
HOME
COLUMNS     命令行编辑模式下可使用的命令行长度
EDITOR      默认的行编辑器
VISUAL      默认的可视编辑器
LOGNAME     用户登录名

//测试命令
test 参数 文件对象
or
[ 参数 文件对象 ]   注意有空格
test -d hello.c     查看hello.c文件是否是目录文件
[ -d hello.c ]

//参数
-d  目录
-f  正规文件
-s  文件长度大于0、非空
-r  可读
-w  可写
-x  可执行

//字符串测试
test 操作符1 字符串
test 字符串1 操作符2 字符串2

-z 空串
-n 非空串

= 相等
!= 不相等


//数值测试
test "数值1" 操作符 "数值2"

-eq  相等
-nq  不相等
-gt  第一个数大于第二个数
-lt  第一个数小于第二个数
-le  第一个数小于等于第二个数
-ge  第一个数大于等于第二个数

//sample
[ "130" -ne "100" ]     前后有空格

//expr
expr 数值1 运算符 数值2

expr 10 + 10
expr 30 \* 10   使用乘号时要进行转义

expr 操作符 字符串

expr length "hello world"
expr substr "hello world" 3 5       //从第三个字符开始(从1开始计数),连续5个字符

expr index "hello world" l          //l第一次出现在第几个字符串



//流控制
if 条件1
    then 命令1
elif 条件2
    then 命令2
else 命令3
fi

多条命令用;分开，用()表示同一个语句块

//case
read dat
case $dat in
1) 命令1
    ;;
2) 命令2
    ;;
*) 命令3
    ;;
esac


//for
for file in hello.c
do
    cat $file | more
done


//while
while 表达式
do
    命令1
done


//函数
hello()
{
    命令1
}
hello           //不加括号调用


//函数参数
hello()
{
    dat=$1
    //do some thing
}
read count
hello $count



```



