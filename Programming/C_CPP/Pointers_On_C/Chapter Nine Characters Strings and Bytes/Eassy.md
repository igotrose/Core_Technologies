# 第九章 字符串、字符和字节

## 9.1 字符串基础
字符串就是一串零个或者多个字符，并且以'\0'结尾  
头文件***string.h***包含了使用字符串函数所需的原型和声明    
## 9.2 字符串长度
***strlen***函数用于计算字符串长度，即字符个数

    size_t strlen( const char* string )；
    
## 9.3 不受限制的字符串函数
最常用的字符串函数都是不受限制的，所以我们使用的需要注意使用边界，保证不会溢出 
### 9.3.1 复制字符串
***strcpy***函数用于复制字符串。把参数**src**字符串复制到**dst**参数

    char *strcpy( char *dst, const char *src );
### 9.3.2 连接字符串
***strcat***函数用于连接字符串。传入**dst**参数，找到该字符串的末尾，添加**src**字符串

    char * strcat( char* dst, const char* src );
### 9.3.3 函数的返回值
在绝大多数的字符串函数调用中，他们的返回值只是被简单地忽略
### 9.3.4 字符串比较 
***strcmp***函数用于比较字符串，该函数将通过ACSII码值逐个比较字符串，直到不匹配
    
    int strcmp( const char* s1, const char* s2 );
## 9.4 长度受限的字符串函数

    char* strncpy( char* dst, const char* src, size_t len );
    char* strncat( char* dst, const char* src, size_t len );
    int strncmp( const char* s1, const char* s2, size_t len );

## 9.5 字符串查找基础
### 9.5.1 查找一个子字符
***strchr函数***和***strrchr***函数都用于查找字符，都会返回第一次出现的目标字符的指针，一个从左一个从右

    char* strchr( const char* str, int ch);
    char* strrchr( const char* str, int ch);
### 9.5.2 查找任何几个字符
***strpbrk***函数用于查找任何一组字符第一次在字符串中出现的位置，返回一个指向**str**中第1个匹配**group**中任意字符的字符位置

    char* strpbrk( const char* str, const char* group ); 

### 9.5.3 查找一个子串
***strstr***函数，在**s1**中查找整个**s2**第一次出现的起始位置

    char* strstr( const char* s1, const char* s2 );

## 9.6 高级字符串查找
### 9.6.1 查找一个字符串前缀
***strspn***函数用于检索**str**中第一个不在**group**中出现的字符的下标  
***strcspn***函数用于检索**str**中第一个在**group**中出现的字符的下标

    size_t strspn( const char* str, const char* group );
    size_t strcspn( const char* str, const char* group );
### 9.6.2 查找标记 
***strtok***函数用于记录分割下标, **seq**是分割标记字符集，**str**是将被分割标记的对象

    char *strtok( char* str, const char* seq );

## 9.7 错误信息
***strerror***函数用于返回错误信息

    char* strerror( int err_number )

## 9.8 字符操作
### 9.8.1 字符分类
以下这些函数会对字符进行判断测试
 函数 | 如果参数符合条件返回真   
-----|----------------------  
 iscntrl | 任意控制字符
 isspace | 空白字符
 isdigit | 十进制数字
 isxdigit | 十六进制数
 islower | 小写字母 
 isupper | 大写字母
 iaalpha | 字母
 isalnum | 字母或数字
 ispunct | 标点符号  
 isgraph | 图形字符
 isprint | 可打印字符 
 ### 9.8.2 字符转换
***toupper***函数和***tolower***函数能对字符进行大小写转换操作  

    int tolower( int ch );
    int toupper( int ch );
## 9.9 内存操作
***memcpy***函数可以赋值任何类型的值，它从**src**复制**length**个字节到**dst**的内存起始位置  

    void* memcpy( void* dst, const void* src， size_t length );
***memmove***函数能够实现覆盖功能，它将**src**中**length**个字节的内容覆盖到到**dst**, 是比***memcpy***更加安全的函数    

    void* memmove( void* dst, const void* src, size_t length );
***memcmp***函数功能可以类比***strcmp***

    void* memcmp( const void* a, const void* b, size_t length );  
***memset***函数能对从**a**开始，长度为**length**内存初始化为特定的值  
    
    void* memset( void* a, int ch, size_t length )
