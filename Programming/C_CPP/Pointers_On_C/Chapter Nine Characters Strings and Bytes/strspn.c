#include<stdio.h>
#include<string.h>

void main()
{
    char string[] = "sadvaksjlc";
    int pos = strspn( string, "fds");
    printf("%d", pos);
}