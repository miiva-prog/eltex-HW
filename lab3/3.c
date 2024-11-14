#include <stdio.h>
#define SIZE 10

int main() {
  int arr[SIZE];

  for (int n = 0; n < SIZE; n++)
    arr[n] = n + 1;

  printf("3 = %d\n", *(arr + 2));
  printf("7 = %d\n", *(arr + 6));
  printf("10 = %d\n", *(arr + 9));

  for (int n = 0; n < SIZE; n++)
    printf("%d ", *(arr + n));

  printf("\n");

  return 0;
}