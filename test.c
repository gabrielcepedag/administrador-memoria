#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memo.h"
#include ".\AdministradorMemoria.c"

#define PUTCONTENT(a) putContent( a, a->item.arr->length - 1 )
#define POPITEM(a)    popItem (a, a->item.arr->length - 1 )

#define MAX_HASH_TABLES 5

static int id = 0;
HASH_TABLE *arrHash[MAX_HASH_TABLES];
int arrTables[MAX_HASH_TABLES] = {0};
size_t ind = 0;

PITEM createItem()
{  
   PITEM pitem = xxmalloc( sizeof( ITEM ) );
   pitem->refer = 1;
   pitem->id = pitem->refer + 1;
   pitem->type = ITEM_TYPE_NONE;
   pitem->key = rand();

   return pitem;
}

size_t len(PITEM pitem)
{
   return pitem->item.arr->length;
}

size_t deleteItem( PITEM pitem )
{
   if (!pitem){
      return -1;
   }

   if ( pitem->type == ITEM_TYPE_NONE ){
      return -2;
   }

   if (pitem->id == pitem->refer + 1){

      pitem->refer -= 1;
      pitem->id -= 1;

      size_t referencia = pitem->refer;

      if (referencia == 0){
         xxfree(pitem);
      }
      //printf("DELETE --- ID: %d ----- REFERENCIA: %d\n",pitem->id,pitem->refer);

      return referencia;
   }
   return -3;
}

size_t deleteArray(PITEM temp)
{
   if (!temp){
      return -1;
   }

   if ( temp->type != ITEM_TYPE_ARRAY ){
      return -1;
   }

   if ( temp->id == temp->refer + 1 ){
      temp->refer -= 1;
      temp->id -= 1;

      if ( temp->refer == 0 ){
         size_t length = len(temp);

         for (int i = 0; i < length; i++){
            deleteItem( temp->item.arr->item[i].pitem );
         }

         xxfree(temp->item.arr->item);       //DELETE ITEM_ARRAY
         xxfree(temp->item.arr);             //DELETE BASE_ARRAY
         xxfree(temp);                       //DELETE PITEM
      }
      return temp->refer;
   }

   return -1;
}

PITEM copyItem(PITEM pitem)
{
   if (pitem == NULL){
      return NULL;
   }

   pitem->refer += 1;
   pitem->id += 1;

   return pitem;
}

void putNumber(PITEM pitem, int number)
{
   if ( pitem->type == ITEM_TYPE_NONE || pitem->type == ITEM_TYPE_INTEGER ){
      pitem->type = ITEM_TYPE_INTEGER;
      pitem->item.number = number;
   }
   return ;
}

void putString(PITEM pitem, char *str)
{
   if ( pitem->type == ITEM_TYPE_NONE || pitem->type == ITEM_TYPE_STRING ){
      pitem->type = ITEM_TYPE_STRING;
      sprintf(pitem->item.str, "%s", str);
   }
   return ;
}

void putPointer(PITEM pitem, void *ptr)
{
   if ( pitem->type == ITEM_TYPE_NONE || pitem->type == ITEM_TYPE_POINTER ){
      pitem->type = ITEM_TYPE_POINTER;
      pitem->item.pointer = ptr;
   }
   return ;
}

PITEM createArray(size_t capacity)
{
   PITEM pitem = createItem();
   BASE_ARRAY* baseArray = xxmalloc( sizeof(BASE_ARRAY) );
   baseArray->capacity = capacity;
   baseArray->length = 0;
   baseArray->item = xxmalloc( sizeof( ITEM_ARRAY ) * capacity );
   pitem->item.arr = baseArray;
   pitem->type = ITEM_TYPE_ARRAY;

   return pitem;
}

void pushArrayItem(PITEM pitem, PITEM ptr)
{
   size_t index = len(pitem);

   if ( index < pitem->item.arr->capacity ){
      pitem->item.arr->item[index].pitem = copyItem(ptr);
      pitem->item.arr->item[index].index = index;
      pitem->item.arr->length++;
   }
   else if( index >= pitem->item.arr->capacity ) {
      printf("Error al intentar a%cadir un item al array.\n",164);
      printf("La cantidad de espacio solicitada fue de: %d y ya hay %d items agregados.\n\n",pitem->item.arr->length,index);
   }
   else{
      printf("Error al intentar a%cadir un item al array por razones desconocidas.\n",164);
   }
}

void putContent(PITEM pitem, size_t index)
{
   if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type == ITEM_TYPE_INTEGER ){
      printf("El contenido en la posicion [%d] es: %d\n",index,pitem->item.arr->item[index].pitem->item.number);
   }
   else if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type == ITEM_TYPE_STRING ){
      printf("El contenido en la posicion [%d] es: %s\n",index,pitem->item.arr->item[index].pitem->item.str);
   }
   else if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type == ITEM_TYPE_POINTER){
      printf("El contenido en la posicion [%d] es un puntero.\n",index);
   }
   else if (pitem->type == ITEM_TYPE_ARRAY && pitem->item.arr->item[index].pitem->type > ITEM_TYPE_UNKNOWN){
      printf("Hay problema con la asignacion de tipo del ITEM en el indice: [%d]\n",index);
   }
   else if (pitem->type != ITEM_TYPE_ARRAY){
      printf("Esto no es un item de tipo arreglo.\n");
   }
}

void printList( PITEM pitem )
{
   for (int i = 0; i < len(pitem); i++){
      putContent(pitem,i);
   }
   printf("-------------------------------------------------\n");
}

PITEM reallocArray( PITEM pitem, size_t newCapacity )
{
   PITEM newPtr = createArray(newCapacity);
   newPtr->item.arr->capacity = newCapacity;
   newPtr->item.arr->length = len(pitem);

   for (int ind = 0; ind < len(pitem); ind++){
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

   for (int ind = index; ind < len(pitem); ind++){
      pitem->item.arr->item[ind].pitem = pitem->item.arr->item[ind+1].pitem;
   }
   return len(pitem);
}

PITEM copyList(PITEM source, PITEM dest)
{
   size_t destinyLenght = len(dest);
   size_t destinyCapacity = dest->item.arr->capacity - len(dest);
   size_t sourceLenght = len(source);

   if ( sourceLenght <= destinyCapacity ){

      for (size_t ind = 0; ind < sourceLenght; ind++){
         pushArrayItem(dest, source->item.arr->item[ind].pitem);
         deleteItem(source->item.arr->item[ind].pitem);
      }
   }else if ( sourceLenght > destinyCapacity ){

      size_t newCapacity = destinyLenght + sourceLenght;
      dest = reallocArray(dest, newCapacity);

      for (size_t ind = 0; ind < sourceLenght; ind++){
         pushArrayItem(dest, source->item.arr->item[ind].pitem);
         deleteItem(source->item.arr->item[ind].pitem);
      }
   }
   return dest;
}

void swapItem( PITEM* p1, PITEM* p2 )
{
   PITEM temp;

   temp = *p1;
   *p1 = *p2;
   *p2 = temp;
}

void reverseList( PITEM pitem )
{
   size_t leng = len(pitem) - 1;
   size_t cont = len(pitem) / 2;

   if (cont > 0){
      for ( size_t ind = 0; ind <= cont; ind++, leng-- ){
         swapItem( &pitem->item.arr->item[ind].pitem, &pitem->item.arr->item[leng].pitem );
      }
   }
   return ;
}

size_t indexOf( PITEM pitem, void *ptr )
{
   size_t lenght = len(pitem);

   if ( pitem->type == ITEM_TYPE_ARRAY ){

      for ( int ind = 0; ind < lenght; ind++ ){
         //if ( pitem->item.arr->item[ind].pitem->item.number == ptr )
         //   return ind;
         if ( (strcmp(pitem->item.arr->item[ind].pitem->item.str, ptr )) == 0)
            return ind;
        // else if ( pitem->item.arr->item[ind].pitem->item.pointer == ptr )
         //   return ind;
         else if ( pitem->item.arr->item[ind].pitem->item.arr == ptr )
            return ind;
      }
   }else{
      return -2;
   }

   return -1;
}

size_t removeItem( PITEM pitem, void *ptr)
{
   if ( pitem->type == ITEM_TYPE_ARRAY){
      size_t index = indexOf( pitem, ptr );
      if ( index != -1 ){
         popItem( pitem, index );
      }
      return index;
   }
   return -1;
}

PITEM createTable( int n )
{
   int tamagno = n + (n/2);
   PITEM pitem = createItem();
   HASH_TABLE *table;

   table = (HASH_TABLE*) xxmalloc( sizeof(HASH_TABLE) );

   table->free = 0;
   table->length = 0;
   table->capacity = tamagno;
   table->id = id;
   id++;

   for (int i = 0; i < tamagno; i++) {
      table->item[i] = NULL;
   }
   
   arrHash[ind] = table;
   ind++;
   pitem->item.table = table;
   pitem->type = ITEM_TYPE_HASH;

   return pitem;
}

HASH_TABLE* findTableById(size_t idTable)
{
   for (int i = 0; i < MAX_HASH_TABLES; i++) {
      if (arrHash[i]->id == idTable)
         return arrHash[i];
   }
   return NULL;
}

int hashKey(PITEM table, PITEM item )
{
   if (table == NULL)
      return -1;

   return  (item->key << 2 + item->key >> 5) % table->item.table->capacity;
}

int insertElement(PITEM table, PITEM item)
{  

   int index = hashKey(table,item);
   
   if ( table->item.table->length < table->item.table->capacity ){

      if ( table->item.table->item[index] == NULL ){
         table->item.table->item[index] = copyItem(item);
         table->item.table->length++;
      }else{

         while ( true ){
            index = (index + 1) % table->item.table->capacity;

            if ( table->item.table->item[index] == NULL ){
               table->item.table->item[index] = copyItem(item);
               table->item.table->length++;
               break;
            }
         }
      }
   }else{
      return -1;
   }
   return index;
}

int findElement(PITEM table, PITEM item)
{
   
   int index = hashKey(table,item);
   int cont = 0;

   if (index == -1)
      return -1;
   
   if ( table->item.table->item[index] == item )
      return index;
   else{
       while ( cont < table->item.table->capacity ){
            index = (index + 1) % table->item.table->capacity;

            if ( table->item.table->item[index] == item ){
               return index;
            }
            cont++;
         }
   }
   return -2;
}

int deleteElement(PITEM table, PITEM item)
{
   int index = hashKey(table,item);
   int cont = 0;

   if ( index == -1 )
      return -1;

   if ( table->item.table->item[index] == item ){
      deleteItem(table->item.table->item[index]);
      table->item.table->item[index] = NULL;
      return index;
   }
   else{
       while ( cont < table->item.table->capacity ){
            index = (index + 1) % table->item.table->capacity;

            if ( table->item.table->item[index] == item ) {
               deleteItem( table->item.table->item[index] );
               table->item.table->item[index] = NULL;
               return index;
            }
            cont++;
         }
   }
}

size_t deleteHashTable(PITEM table)
{
   if ( table == NULL || table->type != ITEM_TYPE_HASH )
      return -1;

   for (int ind = 0; ind < table->item.table->capacity; ind++){
      if ( table->item.table->item[ind] != NULL ){
         deleteItem(table->item.table->item[ind]);
         table->item.table->item[ind] = NULL;
      }
   }
   xxfree( table->item.table );
   deleteItem(table);

   return 0;
}

void printHashTable(PITEM itemTable)
{ 
   size_t size = itemTable->item.table->capacity;

   if (itemTable->type == ITEM_TYPE_HASH) {
      for (int ind = 0; ind < size; ind++){
      if ( itemTable->item.table->item[ind] != NULL ){
         if (itemTable->item.table->item[ind]->type == ITEM_TYPE_INTEGER )
            printf("El contenido en la posicion[%d] del IDhash: %d es: %d\n",ind,itemTable->item.table->id,itemTable->item.table->item[ind]->item.number);
         else if ( itemTable->item.table->item[ind]->type == ITEM_TYPE_STRING )
            printf("El contenido en la posicion [%d] del IDhash: %d es: %s\n",ind,itemTable->item.table->id,itemTable->item.table->item[ind]->item.str);
         else if ( itemTable->item.table->item[ind]->type == ITEM_TYPE_POINTER )
            printf("El contenido en la posicion [%d] del IDhash: %d es un puntero\n",ind,itemTable->item.table->id);
         else if ( itemTable->item.table->item[ind]->type == ITEM_TYPE_ARRAY )
            printf("El contenido en la posicion [%d] del IDhash: %d es un arreglo.\n",ind,itemTable->item.table->id);
         else if ( itemTable->item.table->item[ind]->type == ITEM_TYPE_UNKNOWN )
            printf("El contenido en la posicion [%d] del IDhash: %d es desconocido.\n",ind,itemTable->item.table->id);
      }  
   }
   }else
      printf("Este no es un ITEM de tipo HASH...\n");
   
}

int main()
{
   srand(time(NULL));

   PITEM table = createTable( 10 );

   PITEM item = createItem();
   putNumber(item,100);
   insertElement(table,item);

   PITEM item2 = createItem();
   putNumber(item2,200);
   insertElement(table,item2);

   PITEM item3 = createItem();
   putString(item3,"Hello world");
   insertElement(table,item3);

   printHashTable(table);

   printf("Reservado antes: %d\n",infoReservado());

   printf("%d\n",deleteElement(table,item));
   printf("%d\n",deleteElement(table,item2));
   printf("%d\n",deleteElement(table,item3));

   deleteItem(item);
   deleteItem(item2);
   deleteItem(item3);

   deleteHashTable(table);

   printf("Reservado despues: %d\n",infoReservado());


   return 0;
}