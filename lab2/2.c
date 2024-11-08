#include <stdio.h>

int main() {
  int N, value = 1;

  printf("size:");
  scanf("%d", &N);

  int arr[N];

  for (int n = 0; n < N; n++)
    arr[n] = value++;

  for (int n = N - 1; n >= 0; n--)
    printf("%d ", arr[n]);

  printf("\n");

  return 0;
}