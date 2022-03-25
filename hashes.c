#include "memo.h"
#include "hashes.h"

// todo
// hacer funciones de extraccion de valores por un key
// borrar elemento
// reordenar el hash
//

PITEM hashNew()
{

  PBASEHASH pBaseHash;

  PITEM pItem = createItem();

  pBaseHash = (PBASEHASH)xxmalloc(sizeof(BASEHASH));
  pBaseHash->pPairs = (P_HASHPAIR)xxmalloc(sizeof(HASHPAIR) * MAX_ITEM_HASH);
  pBaseHash->nSize = 0;
  pBaseHash->length = 0;

  pItem->type = ITEM_TYPE_HASH;
  pItem->item.hash.value = pBaseHash;

  return pItem;
}

void deleteHash(PITEM pitem)
{
  if (pitem)
  {
    if (IS_HASH(pitem))
    {
      PBASEHASH base = (PBASEHASH)pitem->item.hash.value;
      int i;
      for (i = 0; i < base->length; i++)
      {
        ITEM pKey, pValue;
        pKey = base->pPairs->key;
        pValue = base->pPairs->value;
        deleteItem(&pKey);
        deleteItem(&pValue);
      }
      xxfree(base->pPairs);
      xxfree(base);
    }
  }
}

static int hashItemCmp(PITEM pKey1, PITEM pKey2)
{
  if (IS_STRING(pKey1))
  {
    if (IS_STRING(pKey2))
    {
      return itemStrICmp(pKey1, pKey2);
    }
    else
      return 1;
  }
  else if (IS_INTEGER(pKey1) && IS_INTEGER(pKey2))
  {
    double d1 = itemGetInt(pKey1), d2 = itemGetInt(pKey2);
    return d1 < d2 ? -1 : (d1 > d2 ? 1 : 0);
  }
  return -1;
}

static BOOL hashFind(PBASEHASH pBaseHash, PITEM pKey, size_t *pnPos)
{
  size_t nLeft, nRight;

  nLeft = 0;
  nRight = pBaseHash->length;

  while (nLeft < nRight)
  {
    size_t nMiddle = (nLeft + nRight) >> 1;
    int i = hashItemCmp(&pBaseHash->pPairs[nMiddle].key, pKey);
    if (i == 0)
    {
      *pnPos = nMiddle;
      return TRUE;
    }
    else if (i < 0)
      nLeft = nMiddle + 1;
    else
      nRight = nMiddle;
  }

  *pnPos = nLeft;
  return FALSE;
}

static PITEM hashValuePtr(PBASEHASH pBaseHash, PITEM pKey)
{
  size_t nPos;
  int i;

  if (!hashFind(pBaseHash, pKey, &nPos))
  {
    if (nPos < pBaseHash->length)
    {
      for (i = pBaseHash->length - 1; i >= nPos; i--)
      {
        HASHPAIR pair = pBaseHash->pPairs[i];
        pair.iPos++;
        pBaseHash->pPairs[i + 1] = pair;
      }
      pBaseHash->pPairs[nPos].iPos = nPos;
      pBaseHash->pPairs[nPos].key.type = ITEM_TYPE_NONE;
      pBaseHash->pPairs[nPos].value.type = ITEM_TYPE_NONE;
    }
    pBaseHash->length++;
    pBaseHash->pPairs[nPos].key = *pKey;
  }
  return &pBaseHash->pPairs[nPos].value;
}

BOOL hashAdd(PITEM pHash, PITEM pKey, PITEM pValue)
{
  if (IS_HASH(pHash) && IS_HASHKEY(pKey))
  {
    PITEM pDest = hashValuePtr(pHash->item.hash.value, pKey);
    if (pDest)
    {
      if (pValue)
        *pDest = *pValue;
      return TRUE;
    }
  }

  return FALSE;
}

PITEM hashFindValue(PITEM hash, PITEM pKey){

  if ( IS_HASH(hash) && IS_HASHKEY(pKey)){

    size_t pos;
    PBASEHASH baseHash = (PBASEHASH)hash->item.hash.value;

    if ( hashFind(baseHash, pKey, &pos) ){
      return &baseHash->pPairs[pos].value;
    }
  }
  return NULL;
}

void reordenarHash(PITEM pHash, size_t index)
{
  PBASEHASH baseHash = (PBASEHASH)pHash->item.hash.value;
  int len = baseHash->length;
  int ind = index + 1;

  for(index; index <= len; index++, ind++){
    baseHash->pPairs[ind].iPos--;
    baseHash->pPairs[index].key = baseHash->pPairs[ind].key;
    baseHash->pPairs[index].value = baseHash->pPairs[ind].value;
  }

  return;
}

BOOL hashDeleteItem(PITEM pHash, PITEM pKey)
{
  if ( IS_HASH(pHash) && IS_HASHKEY(pKey) ){

    size_t pos;
    if ( hashFind(pHash->item.hash.value, pKey, &pos) ){
      
      PBASEHASH base = (PBASEHASH)pHash->item.hash.value;

      ITEM key, value;
      key = base->pPairs[pos].key;
      value = base->pPairs[pos].value;

      deleteItem(&key);
      deleteItem(&value);

      base->length -= 1;
      reordenarHash(pHash, pos);

      return TRUE;
    }
  }
  return FALSE;

}

void printHashContent(PITEM hash)
{
  if ( IS_HASH(hash) ){
    PBASEHASH baseHash = (PBASEHASH)hash->item.hash.value;
    for (int i = 0; i < baseHash->length; i++){

      ITEM key = hash->item.hash.value->pPairs[i].key;
      ITEM value = hash->item.hash.value->pPairs[i].value;
      printf("el key #%d es %s el valor es: %s\n", i, key.item.str, value.item.str);
    }
    printf("\n\n");
  }
}
