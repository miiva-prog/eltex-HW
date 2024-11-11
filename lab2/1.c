#include <stdio.h>

int main() {
  int N;

  printf("size:");
  scanf("%d", &N);

  if (N <= 0) {
    printf("incorrect size\n");
    return -1;
  }

  int arr[N][N], value = 1;

  for (int n = 0; n < N; n++)
    for (int i = 0; i < N; i++)
      arr[n][i] = value++;

  for (int n = 0; n < N; n++) {
    for (int i = 0; i < N; i++)
      printf("%d ", arr[n][i]);

    printf("\n");
  }

  return 0;
}