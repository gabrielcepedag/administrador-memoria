#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memo.h"
#include ".\AdministradorMemoria.c"

#define PUTCONTENT(a) putContent( a, a->item.arr->length - 1 )
#define POPITEM(a)    popItem (a, a->item.arr->length - 1 )

static int id = 0;
HASH_TABLE *arrHash[5];
size_t ind = 0;

PITEM createItem()
{  
   PITEM pitem = xxmalloc( sizeof( ITEM ) );
   pitem->refer = 1;
   pitem->id = pitem->refer + 1;
   pitem->type = ITEM_TYPE_NONE;
   //pitem->key = rand();
   pitem->key = 1987;

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

HASH_TABLE* newTable( int n )
{
   int tamagno = n + (n/2);
   HASH_TABLE *table;

   table = (HASH_TABLE*) xxmalloc( sizeof( HASH_TABLE) );

   table->length = 0;
   table->capacity = tamagno;
   table->id = id;
   id++;

   for (int i = 0; i < tamagno; i++) {
      table->item[i] = xxmalloc( sizeof(ITEM) );
      table->item[i] = NULL;
   }
   arrHash[ind] = table;
   ind++;

   return table;
}

HASH_TABLE* findTableById(size_t id)
{
   for (int i = 0; i < ind; i++) {
      if (arrHash[i]->id == id)
         return arrHash[i];
   }
   return NULL;
}

int hashKey(size_t id, PITEM item )
{
   HASH_TABLE* table = findTableById(id);
   if (table == NULL)
      return -1;

   return  (item->key << 2 + item->key >> 5) % table->capacity;
}

int insertElement(size_t id, PITEM item)
{
   HASH_TABLE* table = findTableById(id);
   if (table == NULL)
      return -1;

   int index = hashKey(id,item);
   if ( table->length < table->capacity ){

      if ( table->item[index] == NULL ){
         table->item[index] = copyItem(item);
         table->length++;
      }else{

         while ( true ){
            index = (index + 1) % table->capacity;

            if ( table->item[index] == NULL ){
               table->item[index] = copyItem(item);
               table->length++;
               break;
            }
         }
      }
   }else{
      return -1;
   }
   return index;
}

int findElement(size_t id, PITEM item)
{
   HASH_TABLE* table = findTableById(id);
   int index = hashKey(id,item);
   int cont = 0;

   if (index == -1)
      return -1;
   
   if ( table->item[index] == item )
      return index;
   else{
       while ( cont < table->capacity ){
            index = (index + 1) % table->capacity;

            if ( table->item[index] == item ){
               return index;
            }
            cont++;
         }
   }
   return -2;
}

int deleteElement(size_t id, PITEM item)
{
   HASH_TABLE* table = findTableById(id);
   int index = hashKey(id,item);
   int cont = 0;

   if ( index == -1 )
      return -1;

   if ( table->item[index] == item ){
      deleteItem(item);
      table->item[index] = NULL;
      return index;
   }
   else{
       while ( cont < table->capacity ){
            index = (index + 1) % table->capacity;

            if ( table->item[index] == item ){
               deleteItem(item);
               table->item[index] = NULL;
               return index;
            }
            cont++;
         }
   }

}

int main()
{
   srand(time(NULL));
   HASH_TABLE *hash;

   hash = newTable( 10 );

   PITEM item = createItem();
   putNumber( item, 69);

   int indice = insertElement(0,item);
   printf("KEY: %d\n",item->key);

   int ind1 = findElement(0,item);
   printf("%d\n",ind1);

   printf("contenido: %d\n",hash->item[ind1]->item.number);

   PITEM item2 = createItem();
   putNumber( item2,11);

   insertElement(0,item2);
   printf("KEY: %d\n",item2->key);

   int ind2 = findElement(0,item2);
   printf("%d\n",ind2);

   printf("contenido: %d\n",hash->item[ind2]->item.number);

   PITEM item3 = createItem();
   putNumber( item3,44);

   insertElement(0,item3);
   printf("KEY: %d\n",item3->key);

   int ind3 = findElement(0,item3);
   printf("%d\n",ind3);

   printf("contenido: %d\n",hash->item[ind3]->item.number);

   printf("RESERVADO.... %d\n",infoReservado());

   return 0;
}













/*int main2()
{
   char *chr = (char*)'c';
   PITEM number = createItem();
   PITEM str = createItem();
   PITEM ptr = createItem();
   PITEM n1 = createItem();
   PITEM str2 = createItem();

   putNumber(number,55);
   putString(str,"hola");
   putPointer(ptr,chr);
   putString(str2,"Adios");
   putNumber(n1,12);

   PITEM p2 = createArray( 5 );

   pushArrayItem( p2, number );
   pushArrayItem( p2, str );
   pushArrayItem( p2, ptr );
   pushArrayItem( p2, str2 );
   pushArrayItem( p2, n1 );

  // printf("INDEX: %d\n",indexOf(p2,"Adios"));
  // removeItem( p2, "Adios" );
   reverseList( p2 );

   printList( p2 );

   PITEM p3 = createArray(1);
   pushArrayItem( p3, ptr );

   p3 = copyList( p2, p3 );

   PITEM number10 = createItem();
   PITEM sstr2 = createItem();
   PITEM n2 = createItem();
   PITEM prueba = createItem();
   PITEM prueba2 = createItem();

   putNumber( number10,44 );
   putString(sstr2,"La para");
   putNumber(n2,9);
   putNumber(prueba,100);
   putString(prueba2,"GAbriel");

   p3 = reallocArray( p3,11 );

   pushArrayItem( p3,n2 );
   pushArrayItem( p3,sstr2 );
   pushArrayItem( p3,number10 );
   pushArrayItem( p3,prueba );
   pushArrayItem( p3,prueba2 );

   reverseList( p3 );

   POPITEM( p3 );
   //popItem(p3,0);

   deleteItem(number);
   deleteItem(str);
   deleteItem(ptr);
   deleteItem(n1);
   deleteItem(str2);
   deleteItem(number10);
   deleteItem(prueba);
   deleteItem(sstr2);
   deleteItem(n2);
   deleteItem(prueba2);

   printList(p3);

   printf("Reservado antes de liberar el array: %d\n",infoReservado());

   deleteArray(p3);
   deleteArray(p2);

   printf("Reservado despues de liberar el array: %d\n",infoReservado());

   return 0;
}*/
