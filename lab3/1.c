#include <stdio.h>

int main() {
  int number, change_byte;

  printf("number:");
  scanf("%d", &number);

  if (number < 0) {
    printf("incorrect input\n");
    return -1;
  }

  printf("number for change byte:");
  scanf("%d", &change_byte);

  if (change_byte > 255 || change_byte < 0) {
    printf("out of range\n");
    return -1;
  }

  char *ptr = (char *)&number;

  ptr += 2;

  *ptr = change_byte;

  printf("%d\n", number);

  return 0;
}