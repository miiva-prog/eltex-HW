#include <pthread.h>
#include <stdio.h>
#define SIZE 180000000

int main() {
  long sum = 0;

  for (long n = 0; n < SIZE; n++)
    sum++;

  printf("%ld\n", sum);

  return 0;
}