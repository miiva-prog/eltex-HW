#include <stdio.h>

int main() {
  int N;

  printf("size:");
  scanf("%d", &N);

  int arr[N];

  for (int n = 0; n < N; n++)
    scanf("%d", &arr[n]);

  for (int n = N - 1; n >= 0; n--)
    printf("%d ", arr[n]);

  printf("\n");

  return 0;
}