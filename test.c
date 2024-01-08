// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "memo.h"
// #include "item.h"

// int main()
// {
//   char *chr = (char *)'c';
//   PITEM number = createItem();
//   PITEM str = createItem();
//   PITEM ptr = createItem();
//   PITEM n1 = createItem();
//   PITEM str2 = createItem();

//   putNumber(number, 55);
//   putString(str, "hola");
//   putPointer(ptr, chr);
//   putString(str2, "Adios");
//   putNumber(n1, 12);

//   PITEM p2 = createArray(5);

//   pushArrayItem(p2, number);
//   pushArrayItem(p2, str);
//   pushArrayItem(p2, ptr);
//   pushArrayItem(p2, str2);
//   pushArrayItem(p2, n1);

//   // printf("INDEX: %d\n",indexOf(p2,"Adios"));
//   // removeItem( p2, "Adios" );
//   reverseList(p2);

//   printList(p2);

//   PITEM p3 = createArray(1);
//   pushArrayItem(p3, ptr);

//   p3 = copyList(p2, p3);

//   PITEM number10 = createItem();
//   PITEM sstr2 = createItem();
//   PITEM n2 = createItem();
//   PITEM prueba = createItem();
//   PITEM prueba2 = createItem();

//   putNumber(number10, 44);
//   putString(sstr2, "La para");
//   putNumber(n2, 9);
//   putNumber(prueba, 100);
//   putString(prueba2, "GAbriel");

//   p3 = reallocArray(p3, 11);

//   pushArrayItem(p3, n2);
//   pushArrayItem(p3, sstr2);
//   pushArrayItem(p3, number10);
//   pushArrayItem(p3, prueba);
//   pushArrayItem(p3, prueba2);

//   reverseList(p3);

//   POPITEM(p3);
//   // popItem(p3,0);

//   deleteItem(number);
//   deleteItem(str);
//   deleteItem(ptr);
//   deleteItem(n1);
//   deleteItem(str2);
//   deleteItem(number10);
//   deleteItem(prueba);
//   deleteItem(sstr2);
//   deleteItem(n2);
//   deleteItem(prueba2);

//   printList(p3);

//   printf("Reservado antes de liberar el array: %d\n", infoReservado());

//   deleteArray(p3);
//   deleteArray(p2);

//   printf("Reservado despues de liberar el array: %d\n", infoReservado());

//   return 0;
// }
