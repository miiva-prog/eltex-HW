#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "production.h"

struct market showcase[SIZE];
int demand[PTH_PUCHASER];

int getrand(int min, int max) {
  return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

void *buy(void *argument) {
  int *index = (int *)argument;

  for (int n = 0; n < SIZE; n++) {
    if (showcase[n].mode == false) {
      showcase[n].mode = true;

      printf(
          "market - (%d) open\npurchaser - (%d) stocked up\ndemand - (%d)\n\n",
          n, *index, demand[*index]);

      while (showcase[n].room > 0 && demand[*index] > 0) {
        demand[*index]--;
        showcase[n].room--;
      }

      printf("market - (%d) closed\n\n", n);
      break;
    }
  }

  return NULL;
}

void *loading() {
  for (int n = 0; n < SIZE; n++) {
    if (showcase[n].mode == false)
      return NULL;
  }

  for (int n = 0; n < SIZE; n++) {
    showcase[n].mode = false;
    showcase[n].room = 500;
  }

  printf("stores are loaded!\n\n");
  return NULL;
}

int sum() {
  int result = 0;
  for (int n = 0; n < PTH_PUCHASER; n++)
    result += demand[n];
  return result;
}