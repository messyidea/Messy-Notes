How to use git
===

使用的时候要把秘钥放到github网站上
```
ssh-keygen -t rsa -C "xxx@gmail.com"
```

克隆一个仓库
```
git clone git://xxx
```

添加文件
```
git add <filename>
#把所有新文件添加进去
git add .
```

本地提交改动
```
git commit -m "some information about change and commit"
```

推送到master
```
git push origin master
```

把仓库连接到远程服务器
```
git remote add origin <server>
```

查看git状态
```
git status
```

版本回退
```
#查看状态
git log
#回退
git reset --hard 3628164
```

branch
```
#创建一个分支"feature"并切换过去
git checkout -b feature
#切换到主分支
git checkout master
#查看分支
git branch
#删除新建分支
git branch -d feature
#强制删除分支[分支有改动]
git branch -D feature
#提交
git push origin <branch>
#丢弃文件修改
git checkout -- <filename>
```

update and merge
```
#更新仓库
git pull
#合并
git merge <branch>
#conflicts改掉之后合并成功，可以加入仓库
git add <filename>
#预览差异
git diff <source_branch> <target_branch>

```

tag
```
#[1b2e1d63ff]是提交的ID的前10位
git tag 1.0.0 1b2e1d63ff
#获取提交id
git log
#删除变迁
git tag -d [name of tag]
#提交标签
git push origin --tags

```
