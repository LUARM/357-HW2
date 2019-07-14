#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <stdlib.h>

#define STREQ(s1, s2) (strcmp((s1), (s2)) == 0)

void * safe_calloc(size_t numitems, size_t size, unsigned long line);

void * safe_realloc(void *ptr, size_t size, unsigned long line);


void * safe_malloc(size_t size, unsigned long line);


char * safe_strdup(char *str, unsigned long line);


#endif
