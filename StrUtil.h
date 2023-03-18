#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_ARG_COUNT 16
#define MAX_ARG_LENGTH 256

char **initBuffer(void);
void parse(const char *str, char **args, int *argc);