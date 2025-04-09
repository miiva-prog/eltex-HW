#include "pth.h"

int main() {
  pthread_t pthread[SIZE];
  int arr[SIZE], index = 0;
  int recv;

  for (; index < SIZE; index++) {
    arr[index] = index;
    pthread_create(&pthread[index], NULL, print_index, (void *)&arr[index]);
  }

  for (int n = 0; n < SIZE; n++)
    pthread_join(pthread[n], (void *)&recv);

  return 0;
}