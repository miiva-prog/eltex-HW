#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "production.h"

int main() {
  srand(time(NULL));

  pthread_t purchaser[PTH_PUCHASER];
  int arr[SIZE], recv;
  pthread_t loader;

  for (int n = 0; n < SIZE; n++) {
    showcase[n].room = 500;
    showcase[n].mode = false;
  }

  for (int n = 0; n < PTH_PUCHASER; n++)
    demand[n] = getrand(10000, 12000);

  while (sum() > 0) {
    for (int n = 0; n < PTH_PUCHASER; n++) {
      arr[n] = n;
      pthread_create(&purchaser[n], NULL, buy, (void *)&arr[n]);
    }

    for (int n = 0; n < PTH_PUCHASER; n++)
      pthread_join(purchaser[n], (void *)&recv);

    pthread_create(&loader, NULL, loading, NULL);
    pthread_join(loader, NULL);
  }

  return 0;
}