#ifndef __MEMO_H__

#define __MEMO_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//#define XXMALLOC(a) xxmalloc(a, __LINE__)

void *xxmalloc(size_t );
size_t xxfree(void *);
size_t infoReservado();
size_t infoSize();
void *xxcopy(void *);
void memory_info(void *);

typedef struct{
   size_t size;
   size_t referencia;
}INFO;


#endif
