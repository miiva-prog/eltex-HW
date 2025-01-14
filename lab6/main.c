#include "abonent.h"

int main() {
  struct abonent *directory = NULL;

  while (1) {
    int number;

    printf("1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов "
           "поимени\n4) Вывод всех записей\n5) Выход\n\nВведите номер: ");
    scanf("%d", &number);

    if (number == 1) {
      char name[SIZE_STRING], second_name[SIZE_STRING], tel[SIZE_STRING];

      printf("Введите имя: ");
      scanf("%s", name);
      printf("Введите фамилию: ");
      scanf("%s", second_name);
      printf("Введите телефон: ");
      scanf("%s", tel);

      add(&directory, name, second_name, tel);
    } else if (number == 2) {
      char name[SIZE_STRING], second_name[SIZE_STRING], tel[SIZE_STRING];

      printf("Введите имя: ");
      scanf("%s", name);
      printf("Введите фамилию: ");
      scanf("%s", second_name);
      printf("Введите телефон: ");
      scanf("%s", tel);
      delete (&directory, name, second_name, tel);
    } else if (number == 3) {
      char name[SIZE_STRING];

      printf("Введите имя: ");
      scanf("%s", name);

      struct abonent *node = search(&directory, name);

      if (node != NULL)
        printf("name: %s | second name: %s | tel: %s\n", node->name,
               node->second_name, node->tel);
      else
        printf("no such abonent\n");
    } else if (number == 4) {
      print(directory);
    } else if (number == 5) {
      break;
    } else {
      printf("incorrect value\n");
      break;
    }
  }

  return 0;
}