#include<stdio.h>
#include<string.h>

void main()
{
    char str[] = "lazy";
    char string[] = "The quick brow dog jumps over the lazy fox";
    char fmt1[] = "1    2    3    4    5";
    char fmt2[] = "1234567890123456789012345678901234567890";

    char* pdest;
    int result;
    printf("String to be searched : \n\t%s\n", string);
    printf("\t%s\n\t%s\n\n", fmt1, fmt2);
    pdest = strstr(string, str);
    result = pdest - string + 1;
    if (pdest != NULL)
    {
        printf("%s found at position %d\n\n", str, result );
    }
    else
    {
        printf( "%s not fount\n", str );
    }
}