#include"func.h"

// 在数组中寻找某个特定整型值的存储位置，并返回一个指向该位置的指针
int *find_int(int key, int array[], int array_len)
{
    int i;
    for (int i = 0; i < array_len; i++)
    {
        if (array[i] == key)
        {
            return &array[i];
        }
        
    }
    return NULL;
}

// 奇偶校验，parity表示是否满足偶校验
int even_parity(int value, int n_bits)
{
    int parity = 0;
    while (n_bits > 0)
    {
        parity += value & 1;
        value >>= 1;
        n_bits -= 1;
    }
    return (parity % 2) == 0;
}

// 整数交换，十分经典
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

// 把数组的所有元素都设置为0
void clear_array(int array[], int n_elements)
{
    while (n_elements > 0)
    {
        array[--n_elements] = 0;
    }
}

// 地址列表模块
static char name[MAX_ADDRESSES][NAME_LENGTH];
static char address[MAX_ADDRESSES][ADDR_LENGTH];
static char phone[MAX_ADDRESSES][PHONE_LENGTH];

int find_entry(char const *name_to_find)
{
    for (int entry = 0; entry < MAX_ADDRESSES; entry++)
    {
        if (strcmp(name_to_find, name[entry]) == 0)
        {
            return entry;
        }
    }
    return -1;
}

char const *lookup_address(char const *name)
{
    int entry = find_entry(name);
    return (entry == -1) ? NULL : address[entry];
}

char const *lookup_phone(char const *name)
{
    int entry = find_entry(name);
    if (entry == -1)
    {
        return phone[entry];
    }
    
}

// 将二进制整数转换为字符，递归实现数字倒序输出
void binary_to_ascii(unsigned int value)
{
    unsigned int quotient = value / 10;
    if (quotient != 0)
    {
        binary_to_ascii(quotient);
    }
    putchar(value % 10 + '0');
    // 加'0'是为了让余数变为字符的形式 
}

// 递归计算阶乘
long factorial1(int n)
{
    return (n <= 0) ? 1: n * factorial(n - 1);
}
// 迭代计算阶乘
long factorial2(int n)
{
    int result = 1;
    while (n > 1)
    {
        result *= n;
        n -= 1;
    }
    return result;
}
// 递归计算斐波那契数
long fibonacci1(int n)
{
    return (n < 2) ? n : fibonacci1(n - 1) + fibonacci1(n - 2);
}
// 迭代计算斐波那契数
long fibonacci2(int n)
{
    long result, previous_result, next_order_result;
    result = previous_result = 1;
    while (n > 2)
    {
        n -= 1;
        next_order_result = previous_result;
        previous_result = result;
        result = previous_result + next_order_result;
    }
    return result;
}

// 计算标量参数的平均值lower
float average_lower(int n_value, int v1, int v2, int v3, int v4, int v5)
{
    float sum = v1;
    if (n_value >= 2)
    {
        sum += v2;
    }
    if (n_value >= 3)
    {
        sum += v3;
    }
    if (n_value >= 4)
    {
        sum += v4;
    }
    if (n_value >= 5)
    {
        sum += v5;
    }
    return sum / n_value;
}
// 计算标量参数的平均值right
float average_right(int n_value, ...)
{
    va_list var_arg;                                 // 声明var_arg变量，用于访问参数列表的未确定部分
    int count;
    float sum = 0;
    va_start(var_arg, n_value);                      // 调用va_start来对var_arg进行初始化，n_value是参数个数
    for (count = 0; count < n_value; count++)
    {
        sum += va_arg(var_arg, int);                 // 获取参数列表的值，并加到sum中
    }
    va_end(var_arg);
    return sum / n_value;
}