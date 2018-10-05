Android 7+ 加强了对非系统自带 CA 的限制，因此通过 MITM + 用户 CA 来抓取 SSL 流量的各种软件纷纷失效了。

目前已知的解决思路：

1. 修改被抓包应用的 AndroidManifest.xml。这里有自动化修改的工具，手工改也可以参考 http://dwz.cn/7xounZ 

2. 把自己的证书作为系统证书安装到 /system/etc/security/cacerts，这样便不会受到限制。当然，这需要 Root 权限，或者通过刷机安装。具体步骤参见 https://blog.jeroenhd.nl/article/android-7-nougat-and-certificate-authorities 

3. 修改系统本身来消除此限制，例如自己编译修改过的系统，或通过 Xposed 框架。太麻烦，省略#(阴险)

4. 更换早于 Android 7 的系统#(滑稽)

参考文献： https://github.com/mitmproxy/mitmproxy/issues/2054

用户自己安装的证书文件放在： /data/misc/user/0/cacerts-added
