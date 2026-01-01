# Makefile 的简单使用
在```Linux``` 底下通常使用 ```make``` 命令去编译程序，而 ```make``` 命令所执行的动作依赖于 ```Makefile``` 文件
```Makefile
# 最简单的Makefile文件
hello: hello.c
    gcc -o hello hello.c
clean: 
    rm -f hello
```
直接执行 ```make``` 可以编译文件，```make clean``` 可以清除编译出来的结果
### ```Makefile``` 规则
基本规格样式如下：
```
target: prerequires
<tab>command
```
**每个命令行前面必须是一个```Tab```字符**   
执行条件：
- ```prerequires``` 比 ```target``` 文件新
- ```target``` 还没生成   
### ```Makefile``` 赋值方法
- 即使变量、延时变量
    ```Makefile
        A = xxx     // 延时变量
        B ?= xxx    // 延时变量，只有第一次定义时赋值才成功；如果曾定义过，此赋值无效
        C := xxx    // 立即变量
        D += yyy    // 如果D在前面是延时变量，那么现在他还是延时变量；
                    // 如果D在前面是立即变量，那么现在他还是立即变量；
    ```
    例如：
    ```Makefile
    // A 是延时变量，只有在执行使用是才确定值
    A = $@              
    // A 是立即变量，如果$@为空，那么A也为空
    A := $@
    test:
        @echo $A
    ```
### ```Makefile``` 常用函数
#### 字符串替换和分析函数
- ```$(filter pattern..., text)```   
    把 ```text``` 中符合 ```pattern``` 格式的内容，过滤出来，保留
    ```Makefile
    objs := a.o b.o c/ d/
    DIR := $(filter %/, $(obj-y))
    # 结果为 c/ d/
    ```
- ```$(filter-out pattern..., text)```    
    把 ```text``` 中符合 ```pattern``` 格式的内容，过滤出来，舍弃
    ```Makefile
    objs := a.o b.o c/ d/
    DIR := $(filter %/, $(obj-y))
    # 结果为 a.o b.o
    ```
- ```$(patsubst pattern, replacement, text)```       
    寻找 ```text``` 中符合 ```pattern``` 格式的内容，使用 ```replacement``` 进行替代
    ```Makefile
    subdir -y := c/ d/
    subdir -y := $(patsubst %/, %, $(subdir-y))
    # 结果为：c d
    ```
- ```$(subst from, to, text)```       
    在```text``` 中使用 ```to``` 替换每一处 ```from```
    ```Makefile
    $(subst ee, EE, feet on the street)
    # 结果为：fEEt on the street
    ```
- ```$(strip string)```    
    去掉前导和结尾空格，并将中间的多个空格压缩为单个空格
    ```Makefile
    $(strip a  b  c )
    # 结果为：a b c
    ```
- ```$(findstring find,in)```   
    在字符串```in```中搜寻```find```，如果找到，则返回值是```find```，否则返回值为空。
    ```Makefile
    $(findstring a,a b c)
    $(findstring a,b c)
    # 结果为：a 和 空字符串
    ```
- ```$(sort list)```   
    将```list```中的字按字母顺序排序，并去掉重复的字。输出由单个空格隔开的字的列表
    ```Makefile
    $(sort foo bar lose)
    # 结果为：bar foo lose
    ```
#### 文件名函数
- ```$(dir names...)```    
    提取文件路径```names...```中的目录部分
    ```Makefile
    $(dir src/foo.c hacks)
    # 结果为：src/ ./
    ```
- ```$(wildcard pattern)```       
    ```pattern``` 所列出的文件是否存在，把存在的文件都列出来
    ```Makefile
    # src_files 中列出当前目录下的所有.c文件
    src_files := $(wildcard *.c)
    ```
- ```$(notdir names...)```    
    提取文件路径```names...```中的文件名部分
    ```Makefile
    $(notdir src/foo.c hacks)
    # 结果为：foo.c hacks
    ```
- ```$(suffix names...)```   
    提取文件路径```names...```中的文件后缀部分
    ```Makefile
    $(suffix src/foo.c src-1.0/bar.c hacks)     
    # 结果为：' .c .c '
    ```
- ```$(basename names...)```   
    提取文件路径```names...```中的除文件后缀的所有部分
    ```Makefile
    $(basename src/foo.c src-1.0/bar.c hacks)     
    # 结果为：src/foo src-1.0/bar hacks
    ```
- ```$(addsuffix suffix, names...)```   
    对于文件路径```names...```中的所有文件添加```suffix```的后缀，完成后将文件名串联，用单个空格隔开
    ```Makefile
    $(addsuffix .c foo bar)     
    # 结果为：foo.c bar.c
    ```
- ```$(addprefix preffix, names...)```   
    对于文件路径```names...```中的所有文件添加```preffix```的后缀，完成后将文件名串联，用单个空格隔开
    ```Makefile
    $(addprefix src/ foo.c bar.c)     
    # 结果为：src/foo.c src-1.0/bar.c
    ```
#### 其他函数
- ```$(foreach var, list, test)```    
    对 ```list``` 中的每个元素，取出来赋值给 ```var```，然后把 ```var``` 改为 ```text``` 所描述的形式
    ```Makefile
    # objs 依赖于 a.o b.o
    objs := a.o b.o
    # dep_files 最后会依赖于 a.o.d b.o.d
    dep_files := $(foreach f, $(objs), .$(f).d)
    ```
- ```$(if condition, then-part[,else-part])```   
    - 根据```condition```是否为非空字符串，返回```then-part```或者```else-part```，如果```else-part```未提供，返回空字符串
    ```Makefile
    OBJ_DIR = obj
    $(OBJ_DIR)/%.o: %.c
    $(if $(wildcard $(OBJ_DIR)),, mkdir -p $(OBJ_DIR)) 
    $(CC) -c $< -o $@
    # 目录不存在时执行 mkdir
    ```
- ```$(origin variable)```
    - 判断变量```variable```的变量来源   
        |返回值|说明|
        |-----|----|
        |```undefined```|变量未定义|
        |```default```|变量是 Make 的内置默认变量（如 ```CC```，```MAKE```）|
        |```environment```|变量来自环境变量（通过```export```导出或在 Make 启动时继承）|
        |```environment override```|变量既是环境变量，又在 Makefile 中被```override```关键字覆盖|
        |```file```|变量在 Makefile 中定义（普通变量）|
        |```command line```|变量通过命令行传递（如 ```make VAR=value```）|
        |```override```|变量在 Makefile 中被```override```关键字强制覆盖。|
        |```automatic```|变量是自动变量（如 ```$@```，```$<```，极少需要手动检查此类型）|
- ```$(shell command arguments)```
    ```Makefile
    # 假设当前目录下有1.c 2.c 1.h 2.h
    c_src := $(shell ls *.c)
    # 结果为：1.c 2.c
    ```
### 简单```Makefile```解析
```Makefile
src := $(shell ls *.c)
objs := $(patsubst %.c, %.o, $(src))

test: $(objs)
    gcc -o $@ $^

%.o: %.c
    gcc -c -o $@ $<

clean: 
    rm -f test *.o
```
```options```目录下所有的文件为 ```main.c, Makefile, sub.c, sub.h```
1. 第1行```src```变量的值为```main.c, sub.c```
2. 第2行```objs```变量的值为```main.o, sub.o```，是 src 变量经过```patsubst```函数处理后得到的。
3. 第4行目标```test```依赖于```main.o, sub.o```，开始时这两个文件还没有生成，在执行生成```test```的命令之前先将```main.o, sub.o```作为目标查找到合适的规则，以生成```main.o, sub.o```    
4. 第7、8行就是用来生成```main.o, sub.o```的规则
5. 第5行的命令在生成```main.o, sub.o```后得以执行
- ```$@ $^ $<```称为自动变量。 
    - ```$@```表示规则的目标文件名
    - ```$^```表示所有依赖的名字，名字之间用空格隔开
    - ```$<```表示第一个依赖的文件名
    - ```%```是通配符，它和一个字符串中任意个数的字符相匹配