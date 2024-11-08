#include <stdio.h>

int main() {
  const int size_cell = 8, change_bit = 3;
  unsigned int number = 0;
  int change_number = 0;

  printf("decimal:");
  scanf("%u", &number);
  printf("number for change:");
  scanf("%d", &change_number);

  if (change_number > 255) {
    printf("out of range\n");
    return -1;
  }

  unsigned int size = sizeof(number) * size_cell;

  number &= ~(255 << ((change_bit - 1) * 8));
  number |= (change_number & 255) << ((change_bit - 1) * 8);

  printf("binary:");

  while (size != 0) {
    printf("%u", number >> (size - 1) & 1);

    size--;
  }

  printf("\n");

  return 0;
}