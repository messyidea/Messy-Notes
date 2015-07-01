clear
echo "程序运行中..."
#ipLine="`ifconfig | grep 192`"
#echo $ipLine
#preIp=${ipLine:5:11}
preIp=192.168.1.
echo "程序默认将在$preIp 2 - $preIp 255范围内进行扫描"
echo "但这将花费较多时间，请问是否进行手动设置？（y/n)"
read choice
if test $choice = "y"
	then
		echo -n "请输入起始位置(2-255):"
		read i
		echo -n "请输入结束位置($i-255):"
		read j
		echo "下面将对 $i 到 $j 范围进行扫描"
	else
		i=2
		j=255
		echo "下面将直接对2-255整个范围进行扫描"
fi
while test $i -ne $j
do
	echo "正在检测ip的连接情况，请耐心等待"
	ip=$preIp$i
	echo "正在ping端口$ip ..."
	"`ping -c 1 $ip `"
	let i=i+1
	clear
done
clear
echo "所有ip检测完毕，正在计算结果..."
echo "树莓派的ip可能在下列ip中"
echo "`arp -a ` "

