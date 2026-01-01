#include<stdio.h>
#include<string.h>
 
#define MAX_LENGTH 1000
int main()
{   
    char line[MAX_LENGTH];
    char longestLine[MAX_LENGTH];
    int maxLength = 0;
    while (fgets(line, MAX_LENGTH, stdin))
    {   
        int length = strlen(line);

        if(line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
            length -- ;
        }
        if(length > maxLength)
        {
            maxLength = length;
            strcpy(longestLine, line);
        }
    }
    if(maxLength > 0)
    {
        printf("Longest line: %s\n", longestLine);
    }
    return 0;
}