#include "io.h"

void stdflush(void) {
	fflush(stdout);
}

size_t stdread(char *ptr, size_t size)
{
    return fread((void *) ptr, 1, size, stdin);
}


size_t stdwrite(char *ptr, size_t size)
{
    return fwrite((void *) ptr, 1, size, stdout);
}