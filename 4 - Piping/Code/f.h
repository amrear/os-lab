#ifndef F_H
#define F_H

#include <ctype.h>

#define BUFFER_SIZE 1024

typedef struct
{
    char fpath[BUFFER_SIZE];
    char contents[BUFFER_SIZE];
} text_file;

void swap_case(char *s, int begin, int end)
{
    for (int i = begin; i < end; i++)
    {            
        if (isupper(s[i]))
            s[i] = tolower(s[i]);
        else if (islower(s[i]))
            s[i] = toupper(s[i]);
    }
}

#endif