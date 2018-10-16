### matplotlib 中文显示

1. 获取matplotlibrc文件所在路径。在jupyter notebook中获取：

   ```Python
   import matplotlib
   matplotlib.matplotlib_fname()
   ```

2. 把字体拷贝到`/home/messyidea/.pyenv/versions/anaconda3-4.4.0/lib/python3.6/site-packages/matplotlib/mpl-data/fonts`  （具体根据上一步做具体调整）

3. 删除相关字体chache

   ```shell
   rm ~/.cache/matplotlib -r
   ```



参考：https://www.jianshu.com/p/7b7a3e73ef21