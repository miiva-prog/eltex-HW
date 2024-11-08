#include <stdio.h>

int main() {
  const int size_cell = 8;
  int number = 0, counter_units = 0;

  printf("decimal:");
  scanf("%d", &number);

  if (number < 0) {
    printf("Error! Negative number\n");
    return -1;
  }

  printf("binary:");

  int size = sizeof(number) * size_cell;

  while (size != 0) {
    int bit_num = number >> (size - 1) & 1;

    if (bit_num == 1)
      counter_units++;

    printf("%d", bit_num);

    size--;
  }

  printf("\ncount units:%d\n", counter_units);

  return 0;
}