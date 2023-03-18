#include "StrUtil.h"
#include <stdio.h>

char **initBuffer(void)
{

    char **ret;

    ret = (char **)calloc(1, MAX_ARG_COUNT * sizeof(char *));
    *(ret + 0) = (char *)calloc(1, MAX_ARG_COUNT * (MAX_ARG_LENGTH + 1) * sizeof(char));
    for (int i = 0; i < MAX_ARG_COUNT; i++)
    {
        *(ret + i) = (*ret + (MAX_ARG_LENGTH + 1) * i);
    }

    return ret;
}

void parse(const char *str, char **args, int *argc) {
    int i = 0;
    *argc = 0;
    int argLen = 0;
    char c;

    while (1) {
        c = str[i ++];

        if (c >= 33 && c <= 126) {
            args[*argc][argLen ++] = c;
        } else {
            if (argLen > 0) {
                argLen = 0;
                (*argc) ++;
            }
            
            if (c == '\0') break;
        }
    }
}
