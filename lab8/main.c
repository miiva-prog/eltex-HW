#include "calculate.h"
#include <stdio.h>

int main() {
  int x = 0, y = 0;

  while (1) {
    int number = 0;

    printf("1) Сложение\n2) Вычитание\n3) Умножение\n4) Деление\n5) "
           "Выход\nnumber: ");
    scanf("%d", &number);

    switch (number) {
    case 1:
      printf("x: ");
      scanf("%d", &x);
      printf("y: ");
      scanf("%d", &y);
      printf("result = %d\n", add(x, y));
      break;
    case 2:
      printf("x: ");
      scanf("%d", &x);
      printf("y: ");
      scanf("%d", &y);
      printf("result = %d\n", sub(x, y));
      break;
    case 3:
      printf("x: ");
      scanf("%d", &x);
      printf("y: ");
      scanf("%d", &y);
      printf("result = %d\n", mul(x, y));
      break;
    case 4:
      printf("x: ");
      scanf("%d", &x);
      printf("y: ");
      scanf("%d", &y);
      printf("result = %d\n", div(x, y));
      break;
    case 5:
      return 0;
    default:
      printf("incorrect number\n");
      break;
    }
  }

  return 0;
}