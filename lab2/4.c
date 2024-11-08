#include <stdio.h>

int main() {
  int N, value = 1;

  printf("size:");
  scanf("%d", &N);

  int arr[N][N];
  int size = N;

  while (value <= N * N) {
    int index = size;

    for (int n = N - size; n < size; n++)
      arr[N - size][n] = value++;

    index--;

    for (int n = N - index; n < size; n++)
      arr[n][size - 1] = value++;

    for (int n = index - 1; n > N - size; n--)
      arr[index][n] = value++;

    for (int n = size - 1; n > N - size; n--)
      arr[n][N - size] = value++;

    size--;
  }

  for (int n = 0; n < N; n++) {
    for (int i = 0; i < N; i++)
      printf("%d ", arr[n][i]);

    printf("\n");
  }

  return 0;
}