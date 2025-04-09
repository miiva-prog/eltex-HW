#include "pth.h"

void *print_index(void *argument) {
  int *index = (int *)argument;

  printf("index - %d\n", *index);

  return NULL;
}