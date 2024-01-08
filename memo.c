#include "memo.h"

static size_t reservado = 0;
size_t size_struct = sizeof(INFO);

void *xxcopy(void *ptr)
{
  if (!ptr)
  {
    return NULL;
  }

  INFO *temp = (INFO *)(ptr - size_struct);

  temp->referencia += 1;

  return ptr;
}

void *xxmalloc(size_t bytes)
{
  void *ptr = NULL;

  INFO temp;
  ptr = malloc(bytes + size_struct);
  // printf("Pedi %d bytes.\n",bytes);
  if (!ptr)
  {
    return NULL;
  }

  reservado += bytes;
  memset(&temp, 0x00, size_struct);

  temp.size = bytes;
  temp.referencia += 1;

  memcpy(ptr, &temp, size_struct);

  return ptr + size_struct;
}

size_t xxfree(void *pointer)
{
  INFO *temp = (INFO *)(pointer - size_struct);
  temp->referencia -= 1;
  size_t reference = temp->referencia;

  if (reference == 0)
  {
    // printf("Liberado: %d\n",temp->size);
    reservado -= temp->size;
    free(temp);
  }
  return reference;
}

size_t infoReservado()
{
  return reservado;
}

size_t infoSize()
{
  return size_struct;
}

void memory_info(void *pointer)
{
  INFO *temp;
  memset(&temp, 0x00, size_struct);
  temp = (INFO *)xxcopy(pointer - size_struct);

  printf("Cantidad de bloques reservados -> %zu bytes\n", temp->size);
  printf("Referencia del puntero: %zu\n", temp->referencia);

  return;
}

void memory_stat()
{
  printf("Cantidad de bloques reservados -> %zu bytes\n", infoReservado());

  return;
}