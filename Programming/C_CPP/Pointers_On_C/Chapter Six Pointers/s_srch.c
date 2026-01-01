#include<stdio.h>
#include<assert.h>

#define TRUE 1
#define FALSE 0

int find_char(char **strings, char value)
{
    assert(strings != NULL);
    while (*strings != NULL)
    {
        while (**strings != '\0')
        {
            if (*(*strings)++ == value)
            {
                return TRUE;
            }
        }
        strings++;
    }
    return FALSE;
}