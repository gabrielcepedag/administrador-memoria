#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __MEMO_H__
#define __MEMO_H__

//#define XXMALLOC(a) xxmalloc(a, __LINE__)

void *xxmalloc(size_t);
size_t xxfree(void *);
size_t infoReservado();
size_t infoSize();
void *xxcopy(void *);
void memory_info(void *);
void memory_stat();

typedef struct
{
  size_t size;
  size_t referencia;
} INFO;

//#define ITEM_IS_NONE( p ) (p)->type == ITEM_TYPE_NONE

#endif
