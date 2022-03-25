#include <stdio.h>

#ifndef __ITEM_H__
#define __ITEM_H__

#define ITEM_TYPE(p) ((p)->type)

#define ITEM_TYPE_NONE 0x0
#define ITEM_TYPE_STRING 0x1
#define ITEM_TYPE_POINTER 0x2
#define ITEM_TYPE_INTEGER 0x4
#define ITEM_TYPE_ARRAY 0x8
#define ITEM_TYPE_HASH 0x10
#define ITEM_TYPE_UNKNOWN 0x20fl
#define ITEM_TYPE_HASH_HASHKEY (ITEM_TYPE_STRING | ITEM_TYPE_INTEGER)

#define IS_NONE(p) (ITEM_TYPE(p) == ITEM_TYPE_NONE)
#define IS_ARRAY(p) (ITEM_TYPE(p) == ITEM_TYPE_ARRAY)
#define IS_POINTER(p) (ITEM_TYPE(p) == ITEM_TYPE_POINTER)
#define IS_INTEGER(p) (ITEM_TYPE(p) == ITEM_TYPE_INTEGER)
#define IS_HASH(p) (ITEM_TYPE(p) == ITEM_TYPE_HASH)
#define IS_STRING(p) (ITEM_TYPE(p) == ITEM_TYPE_STRING)
#define IS_HASHKEY(p) ((ITEM_TYPE(p) & ITEM_TYPE_HASH_HASHKEY) != 0)

typedef int BOOL;

#define FALSE 0
#define TRUE (!0)

struct strItem;
struct strBaseArray;
struct _BASEHASH;
struct ITEM;

struct strHash
{
  struct _BASEHASH *value;
};

typedef struct
{
  int type;
  int refer;
  size_t id;
  union
  {
    void *pointer;
    int number;
    char str[30];
    struct strBaseArray *arr;
    struct strHash hash;
  } item;

} ITEM, *PITEM;

#ifndef _HASH_INTERNAL_
/* internal structure for hashes */
typedef struct _BASEHASH
{
  void *value;
} BASEHASH, *PBASEHASH;
#endif

typedef struct strItem
{
  PITEM pitem;
  size_t index;

} ITEM_ARRAY;

typedef struct strBaseArray
{
  size_t length;
  size_t capacity;
  ITEM_ARRAY *item;

} BASE_ARRAY;

PITEM createItem();

int itemStrICmp(PITEM pitem1, PITEM pitem2);
PITEM copyItem(PITEM pitem);
PITEM itemPutString(const char *szText);
size_t deleteItem(PITEM pitem);
void putString(PITEM pitem, const char *str);
double itemGetInt(PITEM pitem);

PITEM hashNew();
BOOL hashAdd(PITEM pHash, PITEM pKey, PITEM pValue);
void deleteHash(PITEM pHash);

#endif