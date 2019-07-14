#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "support.h"

void * safe_malloc(size_t size, unsigned long line) 
{
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "\n[%s:%lu] Out of memory(%lu bytes)\n", __FILE__, line, (unsigned long)size);
        exit(EXIT_FAILURE);
    }

    return p;
}


char * safe_strdup(char *str, unsigned long line) 
{
    char *p = malloc(strlen(str)+1);
    if (!p) 
    {
        fprintf(stderr, "\n[%s:%lu] Out of memory(%lu bytes)\n", __FILE__, line, (unsigned long)strlen(str)+1);
        exit(EXIT_FAILURE);
    }
    strcpy(p, str);

    return p;
}
void * safe_calloc(size_t numitems, size_t size, unsigned long line) 
{
    void *p = calloc(numitems, size);
    if (!p) {
        fprintf(stderr, "\n[%s:%lu] Out of memory(%lu bytes)\n", __FILE__, line, (unsigned long)size);
        exit(EXIT_FAILURE);
    }

    return p;
}

void * safe_realloc(void *ptr, size_t size, unsigned long line) 
{
    void *p = realloc(ptr, size);
    if (!p) 
    {
        fprintf(stderr, "\n[%s:%lu] Out of memory(%lu bytes)\n", __FILE__, line, (unsigned long)size);
        exit(EXIT_FAILURE);
    }

    return p;
}



