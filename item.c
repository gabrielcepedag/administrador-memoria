#include "memo.h"
#include "item.h"

#define PUTCONTENT(a) putContent(a, a->item.arr->length - 1)
#define POPITEM(a) popItem(a, a->item.arr->length - 1)

PITEM createItem()
{
  PITEM pitem = xxmalloc(sizeof(ITEM));
  pitem->refer = 1;
  pitem->id = pitem->refer + 1;
  pitem->type = ITEM_TYPE_NONE;

  return pitem;
}

PITEM itemPutString(const char *szText)
{
  PITEM pItem = createItem();

  putString(pItem, szText);

  return pItem;
}

size_t len(PITEM pitem)
{
  return pitem->item.arr->length;
}

size_t deleteItem(PITEM pitem)
{
  if (!pitem)
  {
    return -1;
  }

  if (pitem->type == ITEM_TYPE_NONE)
  {
    return -2;
  }

  if (pitem->id == pitem->refer + 1)
  {

    pitem->refer -= 1;
    pitem->id -= 1;

    size_t referencia = pitem->refer;

    if (referencia == 0)
    {
      if (IS_HASH(pitem))
      {
        deleteHash(pitem);
      }
      xxfree(pitem);
    }
    // printf("DELETE --- ID: %d ----- REFERENCIA: %d\n",pitem->id,pitem->refer);

    return referencia;
  }
  return -3;
}

size_t deleteArray(PITEM temp)
{
  if (!temp)
  {
    return -1;
  }

  if (temp->type != ITEM_TYPE_ARRAY)
  {
    return -1;
  }

  if (temp->id == temp->refer + 1)
  {
    temp->refer -= 1;
    temp->id -= 1;

    if (temp->refer == 0)
    {
      size_t length = len(temp);
      for (int i = 0; i < length; i++)
      {
        deleteItem(temp->item.arr->item[i].pitem);
      }

      xxfree(temp->item.arr->item); // DELETE ITEM_ARRAY
      xxfree(temp->item.arr);       // DELETE BASE_ARRAY
      xxfree(temp);                 // DELETE PITEM
    }
    return temp->refer;
  }

  return -1;
}

PITEM copyItem(PITEM pitem)
{
  if (pitem == NULL)
  {
    return NULL;
  }

  pitem->refer += 1;
  pitem->id += 1;

  return pitem;
}

void putNumber(PITEM pitem, int number)
{
  if (pitem->type == ITEM_TYPE_NONE || pitem->type == ITEM_TYPE_INTEGER)
  {
    pitem->type = ITEM_TYPE_INTEGER;
    pitem->item.number = number;
  }
  return;
}

void putString(PITEM pitem, const char *str)
{
  if (pitem->type == ITEM_TYPE_NONE || pitem->type == ITEM_TYPE_STRING)
  {
    pitem->type = ITEM_TYPE_STRING;
    sprintf(pitem->item.str, "%s", str);
  }
  return;
}

void putPointer(PITEM pitem, void *ptr)
{
  if (pitem->type == ITEM_TYPE_NONE || pitem->type == ITEM_TYPE_POINTER)
  {
    pitem->type = ITEM_TYPE_POINTER;
    pitem->item.pointer = ptr;
  }
  return;
}

PITEM createArray(size_t capacity)
{
  PITEM pitem = createItem();
  BASE_ARRAY *baseArray = xxmalloc(sizeof(BASE_ARRAY));
  baseArray->capacity = capacity;
  baseArray->length = 0;
  baseArray->item = xxmalloc(sizeof(ITEM_ARRAY) * capacity);
  pitem->item.arr = baseArray;
  pitem->type = ITEM_TYPE_ARRAY;

  return pitem;
}

void pushArrayItem(PITEM pitem, PITEM ptr)
{
  size_t index = len(pitem);

  if (index < pitem->item.arr->capacity)
  {
    pitem->item.arr->item[index].pitem = copyItem(ptr);
    pitem->item.arr->item[index].index = index;
    pitem->item.arr->length++;
  }
  else if (index >= pitem->item.arr->capacity)
  {
    printf("Error al intentar a%cadir un item al array.\n", 164);
    printf("La cantidad de espacio solicitada fue de: %zu y ya hay %zu items agregados.\n\n", pitem->item.arr->length, index);
  }
  else
  {
    printf("Error al intentar a%cadir un item al array por razones desconocidas.\n", 164);
  }
}

void putContent(PITEM pitem, size_t index)
{
  if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type == ITEM_TYPE_INTEGER)
  {
    printf("El contenido en la posicion [%zu] es: %d\n", index, pitem->item.arr->item[index].pitem->item.number);
  }
  else if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type == ITEM_TYPE_STRING)
  {
    printf("El contenido en la posicion [%zu] es: %s\n", index, pitem->item.arr->item[index].pitem->item.str);
  }
  else if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type == ITEM_TYPE_POINTER)
  {
    printf("El contenido en la posicion [%zu] es un puntero.\n", index);
  }
  else if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type > ITEM_TYPE_UNKNOWN)
  {
    printf("Hay problema con la asignacion de tipo del ITEM en el indice: [%zu]\n", index);
  }
  else if (pitem->type != ITEM_TYPE_ARRAY)
  {
    printf("Esto no es un item de tipo arreglo.\n");
  }
}

void printList(PITEM pitem)
{
  for (int i = 0; i < len(pitem); i++)
  {
    putContent(pitem, i);
  }
  printf("-------------------------------------------------\n");
}

PITEM reallocArray(PITEM pitem, size_t newCapacity)
{
  PITEM newPtr = createArray(newCapacity);
  newPtr->item.arr->capacity = newCapacity;
  newPtr->item.arr->length = len(pitem);

  for (int ind = 0; ind < len(pitem); ind++)
  {
    newPtr->item.arr->item[ind].pitem = copyItem(pitem->item.arr->item[ind].pitem);
    newPtr->item.arr->item[ind].index = pitem->item.arr->item[ind].index;
  }
  deleteArray(pitem);

  return newPtr;
}

size_t popItem(PITEM pitem, size_t index)
{
  deleteItem(pitem->item.arr->item[index].pitem);
  pitem->item.arr->length -= 1;

  for (int ind = index; ind < len(pitem); ind++)
  {
    pitem->item.arr->item[ind].pitem = pitem->item.arr->item[ind + 1].pitem;
  }
  return len(pitem);
}

PITEM copyList(PITEM source, PITEM dest)
{
  size_t destinyLenght = len(dest);
  size_t destinyCapacity = dest->item.arr->capacity - len(dest);
  size_t sourceLenght = len(source);

  if (sourceLenght <= destinyCapacity)
  {

    for (size_t ind = 0; ind < sourceLenght; ind++)
    {
      pushArrayItem(dest, source->item.arr->item[ind].pitem);
      deleteItem(source->item.arr->item[ind].pitem);
    }
  }
  else if (sourceLenght > destinyCapacity)
  {

    size_t newCapacity = destinyLenght + sourceLenght;
    dest = reallocArray(dest, newCapacity);

    for (size_t ind = 0; ind < sourceLenght; ind++)
    {
      pushArrayItem(dest, source->item.arr->item[ind].pitem);
      deleteItem(source->item.arr->item[ind].pitem);
    }
  }
  return dest;
}

void swapItem(PITEM *p1, PITEM *p2)
{
  PITEM temp;

  temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

void reverseList(PITEM pitem)
{
  size_t leng = len(pitem) - 1;
  size_t cont = len(pitem) / 2;

  if (cont > 0)
  {
    for (size_t ind = 0; ind <= cont; ind++, leng--)
    {
      swapItem(&pitem->item.arr->item[ind].pitem, &pitem->item.arr->item[leng].pitem);
    }
  }
  return;
}

size_t indexOf(PITEM pitem, void *ptr)
{
  size_t lenght = len(pitem);

  if (pitem->type == ITEM_TYPE_ARRAY)
  {

    for (int ind = 0; ind < lenght; ind++)
    {
      //if (pitem->item.arr->item[ind].pitem->item.number == ptr)
       // return ind;
      if ((strcmp(pitem->item.arr->item[ind].pitem->item.str, ptr)) == 0)
        return ind;
      else if (pitem->item.arr->item[ind].pitem->item.pointer == ptr)
        return ind;
      else if (pitem->item.arr->item[ind].pitem->item.arr == ptr)
        return ind;
    }
  }
  else
  {
    return -2;
  }

  return -1;
}

size_t removeItem(PITEM pitem, void *ptr)
{
  if (pitem->type == ITEM_TYPE_ARRAY)
  {
    size_t index = indexOf(pitem, ptr);
    if (index != -1)
    {
      popItem(pitem, index);
    }
    return index;
  }
  return -1;
}

double itemGetInt(PITEM pitem)
{
  if (pitem && IS_INTEGER(pitem))
  {
    return (double)pitem->item.number;
  }
  return 0;
}

int itemStrICmp(PITEM pitem1, PITEM pitem2)
{
  return strcmp(pitem1->item.str, pitem2->item.str);
}