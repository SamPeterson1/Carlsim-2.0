#ifndef IO_H

#define IO_H

#include <stdio.h>

#define BUF_SIZE 100000

void stdflush(void);
size_t stdread(char *ptr, size_t size);
size_t stdwrite(char *ptr, size_t size);

#endif