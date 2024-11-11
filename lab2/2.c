#include <stdio.h>

int main() {
  int N, value = 1;

  printf("size:");
  scanf("%d", &N);

  if (N <= 0) {
    printf("incorrect size\n");
    return -1;
  }

  int arr[N], size = (N % 2 == 0) ? N / 2 : N / 2 + 1;

  for (int n = 0; n < N; n++)
    arr[n] = value++;

  for (int n = 0; n < size; n++) {
    int change_num = arr[N - n - 1];

    arr[N - n - 1] = arr[n];
    arr[n] = change_num;
  }

  for (int n = 0; n < N; n++)
    printf("%d ", arr[n]);

  printf("\n");

  return 0;
}