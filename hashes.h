#ifndef _HASH_INTERNAL_
#define _HASH_INTERNAL_
#endif

#include "item.h"

#ifndef __HASHES_H__
#define __HASHES_H__

#define MAX_ITEM_HASH 1000

typedef struct _HASHPAIR
{
  ITEM key;
  ITEM value;
  int iPos;
} HASHPAIR, *P_HASHPAIR;

typedef struct _BASEHASH
{
  P_HASHPAIR pPairs;
  size_t nSize;
  size_t length;
} BASEHASH, *PBASEHASH;

#endif