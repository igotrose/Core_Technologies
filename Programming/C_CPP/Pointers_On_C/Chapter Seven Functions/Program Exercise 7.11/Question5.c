#include<stdio.h>
#include<stdarg.h>

void printf(char *string, ...)
{
    va_list vaarg;
    char c;
    char *str;
    va_start(vaarg, string);
    while ((c = *string++) != '\0')
    {
        if (c != '%')
        {
            putchar(c);
            continue;
        }
        switch (*string != '\0' ? *string++:'\0')
        {
        case 'd':
            print_integer(va_arg(vaarg, int));
            break;
        case 'f':
            print_float(va_arg(vaarg, float));
            break;
        case 'c':
            putchar(va_arg(vaarg, int));
            break;
        case 's':
            str = va_arg(vaarg, char *);
            while (str!='\0')
            {
                putchar(*str++);
            }
            break;
        }
    }
    

}
int main()
{
    return 0;
}