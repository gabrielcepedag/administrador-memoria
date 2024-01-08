#include "memo.h"
#include "hashes.h"
#include "memo.c"
#include "hashes.c"
#include "item.c"

int main()
{
  int i;
  PITEM hash = hashNew();

  PITEM pKey1 = itemPutString("A");
  PITEM pValue1 = itemPutString("esto es una prueba111");

  PITEM pKey2 = itemPutString("D");
  PITEM pValue2 = itemPutString("esto es una prueba2222");

  PITEM pKey3 = itemPutString("C");
  PITEM pValue3 = itemPutString("esto es una prueba3333");
  
  PITEM pKey4 = itemPutString("P");
  PITEM pValue4 = itemPutString("esto es una prueba444");

  PITEM pKey5 = itemPutString("B");
  PITEM pValue5 = itemPutString("esto es una prueba555");

  memory_stat();

  hashAdd(hash, pKey1, pValue1);

  hashAdd(hash, pKey2, pValue2);

  hashAdd(hash, pKey3, pValue3);

  hashAdd(hash, pKey4, pValue4);

  hashAdd(hash, pKey5, pValue5);

  hashAdd(hash, pKey1, pValue3);

  printHashContent(hash);

  hashDeleteItem(hash, pKey2);

  printHashContent(hash);
  
  PITEM copyKey = itemPutString("B");
  PITEM value = hashFindValue(hash, copyKey);

  if (value != NULL)
    printf("VALUE: %s\n\n",value->item.str);

  deleteItem(pKey1);
  deleteItem(pValue1);

  deleteItem(pKey2);
  deleteItem(pValue2);

  deleteItem(pKey3);
  deleteItem(pValue3);

  deleteItem(pKey4);
  deleteItem(pValue4);

  deleteItem(pKey5);
  deleteItem(pValue5);

  deleteItem(hash);

  deleteItem(copyKey);

  memory_stat();

  return 0;
}
