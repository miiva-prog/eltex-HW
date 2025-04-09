#include <pthread.h>
#include <stdio.h>
#define SIZE 180000000
#define CORE 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long sum = 0;

void *mutex_sum();

int main() {
  pthread_t pthread[CORE];

  for (int n = 0; n < CORE; n++)
    pthread_create(&pthread[n], NULL, mutex_sum, NULL);

  for (int n = 0; n < CORE; n++)
    pthread_join(pthread[n], NULL);

  printf("%ld\n", sum);

  return 0;
}

void *mutex_sum() {

  pthread_mutex_lock(&mutex);

  for (long n = 0; n < SIZE / CORE; n++)
    sum++;

  pthread_mutex_unlock(&mutex);

  return NULL;
}