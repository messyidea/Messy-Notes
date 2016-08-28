#! /bin/sh  
  
host_dir=`echo ~`                                       # 当前用户根目录  
proc_name="Xtightvnc"                             # 进程名  
file_name="/moniter/cron.log"                         # 日志文件  
pid=0  
  
proc_num()                                              # 计算进程数  
{  
    num=`ps -ef | grep $proc_name | grep -v grep | wc -l`  
    return $num  
}  
  
proc_id()                                               # 进程号  
{  
    pid=`ps -ef | grep $proc_name | grep -v grep | awk '{print $2}'`  
}  
  
proc_num  
number=$?  
if [ $number -eq 0 ]                                    # 判断进程是否存在  
then 
    echo "restart"
    rm /tmp/.X1-lock
    rm /tmp/.X11-unix/X1
    /bin/sh /root/startvnc.sh    # 重启进程的命令，请相应修改 
    /bin/sh /root/vncrestart_tennfy.sh
    proc_id                                         # 获取新进程号  
    echo ${pid}, `date` >> $host_dir$file_name      # 将新进程号和重启时间记录  
fi  

