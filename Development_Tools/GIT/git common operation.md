# GIT 的基本使用
### GIT 工作示意
![alt text](<git workflow schematic.png>)
### GIT & GITHUB-DESKTOP基本下载安装
省略
## GIT 命令
### 初始化配置
``` bash
# 初始化仓库
git init
# 设置用户名
git config --global user.name "<username>"
# 设置用户邮箱
git config --global user.email "<useremail>"
# 查看配置
git config --list
```
### 文件操作
* 添加文件进入缓存区（暂存更改）
``` bash 
# 添加特定文件
git add path/to/file
# 添加多个文件
git add file1.txt file2.txt
# 添加当前目录所有更改
git add .
```
* 临时保存未提交的更改
```
git stash
```
* 提交暂存的文件更改到本地仓库
``` bash 
git commit -m "<commit info>"
```
* 将文件从缓存区和工作区中删除
``` bash 
git rm [file]
```
* 移动或者重命名工作区的文件
``` bash 
git mv [file]
```
* 将暂存区内容撤回
```bash
git reset
```
* 撤销上一次提交记录，保留工作区和暂存区
```bash
git reset --soft HEAD^
```
* 回到指定提交，丢弃该提交之后的所有更改
```bash
git reset --hard <commit_id>
```
### 分支操作
* 查看分支
``` bash 
# 查看所有分支
git branch 
# 查看远程分支
git branch -r
# 查看本地和远程所有分支
git branch -a 
```
* 创建新分支
``` bash 
# 创建分支但不切换
git branch [branch-name]
# 创建并切换分支
git checkout -b [branch-name]
```
* 切换分支
``` bash 
git checkout [branch-name]
```
* 合并分支
```bash
git merge [branch-name]
```
* 删除分支
```bash
git branch -d [branch-name]
```
### 查看操作
* 查看提交历史，包含提交信息
``` bash
git log  
```
* 查看提交的具体改动
```bash
git show [commit_id]
# 查看最新提交
git show HEAD
# 查看前一次提交
git show HEAD^
```
### 比较操作

### 远程操作
* 移除本地已存在的远程仓库
``` bash 
git remote remove origin
```
* 连接远程仓库 --- ssh方式   
``` bash  
# 生成SSH密钥（如果没有的话）
ssh-keygen -t rsa -b 4096 -C "user_email@example.com"
# 将SSH公钥添加到远程仓库
cat ~/.ssh/id_rsa.pub
# 在本地仓库进行初始化设置（如果需要的话）
# 添加远程仓库
git remote add origin git@github.com:username/repository.git
```
* 连接远程仓库 --- https方式
```bash
# 在本地仓库进行初始化设置（如果需要的话）
# 添加远程仓库
git remote add origin https://github.com/username/repository.git
```
* 查看远程仓库
``` bash
git remote -v
```
* 切换本地远程连接仓库方式
``` bash
# 切换到SSH方式
git remote set-url origin git@github.com:username/repository.git
# 切换到HTTPS方式
git remote set-url origin https://github.com/username/repository.git
```
* 推送本地分支到远程仓库
``` bash
git push origin branch-name
```
* 推送本地所有分支到远程仓库
```bash
git push origin --all
```
* 拉取远程分支到本地仓库
``` bash
git pull origin branch-name
```
* 拉取远程仓库所有分支到本地
```bash 
for branch in $(git branch --format='%(refname:short)'); do
    git checkout "$branch" && git pull origin "$branch"
done
```
* 克隆远程仓库 --- ssh方式
``` bash
# 克隆整个仓库
git clone -b git@github.com:username/repository.git
# 克隆特定分支
git clone -b branch-name git@github.com:username/repository.git 
# 克隆指定目录
git clone git@github.com:username/repository.git folder-name
# 克隆特定分支的指定目录
git clone -b branch-name git@github.com:username/repository.git folder-name
```
* 克隆远程仓库 --- https#
``` bash
# 克隆整个仓库
git clone https://github.com/username/repository.git
# 克隆特定分支
git clone -b branch-name https://github.com/username/repository.git
# 克隆指定目录
git clone https://github.com/username/repository.git folder-name
# 克隆特定分支的指定目录
git clone -b branch-name https://github.com/username/repository.git folder-name
```
## 大文件推送拉取
GitHub有一个限制，不能上传超过100M的文件。如果要上传超过100M的文件，需要使用Git LFS（Large File Storage），但是单文件也不能超过2G
- 安装Git LFS
```bash
sudo apt-get install git-lfs
```
- 初始化Git LFS，之后会生成一个.gitattributes文件
```bash
git lfs install
```
- 跟踪大文件类型
```bash
git lfs track "*.ext"
```
