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

#define ITEM_TYPE_NONE    0
#define ITEM_TYPE_STRING  1
#define ITEM_TYPE_POINTER 2
#define ITEM_TYPE_INTEGER 3
#define ITEM_TYPE_ARRAY   4
#define ITEM_TYPE_UNKNOWN 5

#define MAX_SIZE 100

//#define ITEM_IS_NONE( p ) (p)->type == ITEM_TYPE_NONE

struct strItem;
struct strBaseArray;

typedef struct {
   int type;
   int refer;
   size_t id;
   unsigned int key;

   union {
      void *pointer;
      int number;
      char str[ 30 ];
      struct strBaseArray* arr;
   }item;

} ITEM, *PITEM;

typedef struct strItem{
   PITEM pitem;
   size_t index;

}ITEM_ARRAY;

typedef struct strBaseArray{
   size_t length;
   size_t capacity;
   ITEM_ARRAY* item;

}BASE_ARRAY;

typedef struct {
   size_t free;
   size_t id;
   size_t capacity;
   size_t length;
   PITEM item[MAX_SIZE];

}HASH_TABLE;

#endif
