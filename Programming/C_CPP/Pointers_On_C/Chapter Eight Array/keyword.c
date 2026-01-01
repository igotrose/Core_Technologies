#include<string.h>

int lookup_keyword(const char* const desired_word, const char* keyword_table[], const int size)
{
    const char** kwp;
    for (kwp = keyword_table; *kwp != NULL; kwp++)
    {
        if (strcmp(desired_word, *kwp) == 0)
        {
            return kwp - keyword_table;
        }
    }
    return -1;
}