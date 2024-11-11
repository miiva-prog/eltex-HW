#include <stdio.h>

int main() {
  const int size_cell = 8, change_bit = 3;
  int number = 0, change_number = 0;

  printf("decimal:");
  scanf("%d", &number);

  if (number < 0) {
    printf("Error! Negative number\n");
    return -1;
  }

  printf("number for change:");
  scanf("%d", &change_number);

  if (change_number > 255) {
    printf("out of range\n");
    return -1;
  }

  int size = sizeof(number) * size_cell;

  number &= 0xFF00FFFF;
  number |= (change_number & 255) << ((change_bit - 1) * 8);

  printf("binary:");

  while (size != 0) {
    printf("%d", number >> (size - 1) & 1);

    size--;
  }

  printf("\n");

  return 0;
}