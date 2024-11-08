#include <stdio.h>

int main() {
  const int size_cell = 8;
  int number = 0;

  printf("decimal:");
  scanf("%d", &number);

  if (number > 0) {
    printf("Error! Positive number\n");
    return -1;
  }

  printf("binary:");

  int size = sizeof(number) * size_cell;

  while (size != 0) {
    printf("%d", number >> (size - 1) & 1);

    size--;
  }

  printf("\n");

  return 0;
}