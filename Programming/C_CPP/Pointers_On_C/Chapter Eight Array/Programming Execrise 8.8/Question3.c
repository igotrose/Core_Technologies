#include<stdio.h>
#include<stdbool.h>

bool identity_matric(int* arr[10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (i == j)
            {
                if (arr[i][j] == 1)
                {
                    return true;
                }
            }
        }       
    }
}