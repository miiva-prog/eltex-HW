#include <stdio.h>

int main() {
  const int size_cell = 8;
  int number = 0;

  printf("decimal:");
  scanf("%u", &number);

  if (number < 0) {
    printf("Error! Negative number\n");
    return -1;
  }

  printf("binary:");

  int size = sizeof(number) * size_cell;

  while (size != 0) {
    printf("%u", number >> (size - 1) & 1);

    size--;
  }

  printf("\n");

  return 0;
}