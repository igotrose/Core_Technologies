#include<stdio.h>

int my_strlen(char *string)
{
    int count = 0;
    while (*string != '\0')
    {
        count++;
        string++;
    }
    return count;
}

void reverse_string(char *string)
{
    int len = my_strlen(string);
    char *left = string, *right = string + len - 1;
    char tmp;
    while (left <= right)
    {
        if (*left != '\0' && *right != '\0')
        {
            tmp = *left;
            *left = *right;
            *right = tmp;    
        }
        left++;
        right--;
    }
}
int main()
{
    char str[] = "abcdefg";
    printf("%s\n", str);
    reverse_string(str);
    printf("%s", str);
    return 0;
}