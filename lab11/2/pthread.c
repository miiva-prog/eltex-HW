#include <pthread.h>
#include <stdio.h>
#define SIZE 180000000
#define CORE 4

long sum = 0;

void *pth_sum();

int main() {
  pthread_t pthread[CORE];

  for (int n = 0; n < CORE; n++)
    pthread_create(&pthread[n], NULL, pth_sum, NULL);

  for (int n = 0; n < CORE; n++)
    pthread_join(pthread[n], NULL);

  printf("%ld\n", sum);

  return 0;
}

void *pth_sum() {
  long local = 0;

  for (long n = 0; n < SIZE / CORE; n++)
    local++;

  if (local == SIZE / CORE)
    sum += local;

  return NULL;
}