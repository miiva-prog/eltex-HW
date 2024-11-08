#include <stdio.h>

int main() {
  const int size_cell = 8;
  unsigned int number = 0, counter_units = 0;

  printf("decimal:");
  scanf("%u", &number);
  printf("binary:");

  unsigned int size = sizeof(number) * size_cell;

  while (size != 0) {
    unsigned int bit_num = number >> (size - 1) & 1;

    if (bit_num == 1)
      counter_units++;

    printf("%u", bit_num);

    size--;
  }

  printf("\ncount units:%u\n", counter_units);

  return 0;
}