# Linux的常用命令
Linux命令的格式
```bash
    command [-options] [parameter] 
```
命令可以通过 ```man command``` 去查看帮助或者通过官网 ```www.gnu.org``` 去查询
## 目录/文件操作命令
|操作命令|作用|
|:-------:|:----:|
|```pwd```|打印当前所在路径|
|```cd```|切换路径|
|```mkdir```|创建目录|
|```rmdir```|删除目录|
|```ls```|列出目录内容|
|```cp```|拷贝复制|
|```rm```|删除文件或目录|
|```cat```|查看文件内容|
|```touch```|修改文件时间，如果文件不存在则创建空文件|
## 改变文件的权限和属性
1. ```chgrp```
    - 改变文件所属用户组
        - 摘要
            ```bash
            chgrp [OPTION]... GROUP FILE...
            chgrp [OPTION]... --reference=RFILE FILE...
            ```
        - 示例
            ```bash
            # Change the group of /u to "staff".
            chgrp staff /u
            # Change the group of /u and subfiles to "staff".
            chgrp -hR staff /u
            ```
2. ```chown```
    - 改变文件所有者
        - 摘要
            ```bash
            chown [OPTION]... [OWNER][:[GROUP]] FILE...
            chown [OPTION]... --reference=RFILE FILE...
            ```
        - 示例
            ```bash
            # Change the owner of /u to "root".
            chown root /u
            # Likewise, but also change its group to "staff".
            chown root:staff /u
            # Change the owner of /u and subfiles to "root".
            chown -hR root /u
            ```
3. ```chmod```
    - 改变文件的权限
        - 摘要
            ```bash
            chmod [OPTION]... MODE[,MODE]... FILE...
            chmod [OPTION]... OCTAL-MODE FILE...
            chmod [OPTION]... --reference=RFILE FILE...
            ```
        - 示例
            ```bash
            # Change file permissions of FOO to be world readable and user writable
            # with no other permissions.
            chmod a=r,u+w foo
            chmod 644 foo
            # Add user and group execute permissions to FOO.
            chmod +110 filegg
            chmod ug+x file
            ```
## 查找和搜索命令
1. ```find```
    - 查找文件
        - 摘要
            ```bash
            find [-H] [-L] [-P] [-D debugopts] [-Olevel] [starting-point...] [expression]
            ```
        - 示例
            ```bash
            # Find files named core in or below the directory /tmp and delete them.
            find /tmp -name core -type f print | xargs /bin/rm -f
            ...
            ```
2. ```grep```
    - 查找文件中符合条件的字符串
        - 摘要
            ```bash
            grep [OPTION...] PATTERNS [FILE...]
            grep [OPTION...] -e PATTERNS ... [FILE...]
            grep [OPTION...] -f PATTERN_FILE ... [FILE...]
            ```
        - 示例
            ```bash
            # The following example outputs the location and contents of any line containing “f” and ending in “.c”, within all files in the current directory whose names contain “g” and end in “.h”. The -n option outputs line numbers, the -- argument treats expansions of “*g*.h” starting with “-” as file names not options, and the empty file /dev/null causes file names to be output even if only one file name happens to be of the form “*g*.h”.
            grep -n -- 'f.*\.c$' *g*.h /dev/null
            ```
## 压缩和解压缩命令
1. ```gzip```
    - 解压或压缩文件
        - 摘要 
            ```bash
            gzip [ -acdfhklLnNrtvV19 ] [-S suffix] [ name ...  ]
            ```
2. ```tar```
3. ```bzip2```
## 网络命令
1. ```ifconfig```
2. ```route```
## 其他命令
1. ```file```
2. ```which```
3. ```whereis```

