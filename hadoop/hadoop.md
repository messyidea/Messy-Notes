hadoop notes
=====
start(require: ssh was started, ip config):
$HADOOP_HOME/bin/start-all.sh


hive: 
刚开始一直报错："Unable to create log directory ${build.dir}/tmp"
刚开始不知道build.dir在哪里，然后一直在hive安装目录找，找不到相关信息。
其实应该是在hadoop的fs中，但是事实上tmp文件夹是已经存在着的。
后来发现原来是hive-site.xml有问题。hive-site.xml只需要把原来的hive-default.xml改名就行了，如果要把默认的改为mysql，则再在上面的基础上面来改。


