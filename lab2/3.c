#include <stdio.h>

int main() {
  int N;

  printf("size:");
  scanf("%d", &N);

  int arr[N][N], index = N - 1;

  for (int n = 0; n < N; n++) {
    for (int i = 0; i < N; i++)
      arr[n][i] = (i < index) ? 0 : 1;

    index--;
  }

  for (int n = 0; n < N; n++) {
    for (int i = 0; i < N; i++)
      printf("%d ", arr[n][i]);

    printf("\n");
  }

  return 0;
}