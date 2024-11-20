#include <stdio.h>
#include <string.h>
#define SIZE 100

typedef struct {
  char name[10];
  char second_name[10];
  char tel[10];
} abonent;

void add(abonent *dir, int index, char *name, char *sec_name, char *tel);

void delete(abonent *dir, char *name, char *sec_name, char *tel);

void search(abonent *dir, char *name);

void print(abonent *dir);

int main() {
  abonent directory[SIZE];
  int count_abonents = 0;

  while (1) {
    int number;
    printf("1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по "
           "имени\n4) Вывод всех записей\n5) Выход\n\nВведите номер: ");
    scanf("%d", &number);

    if (number == 1) {
      char name[10], second_name[10], tel[10];

      printf("Введите имя: ");
      scanf("%s", name);
      printf("Введите фамилию: ");
      scanf("%s", second_name);
      printf("Введите телефон: ");
      scanf("%s", tel);
      add(directory, count_abonents, name, second_name, tel);

      count_abonents++;

      if (count_abonents > SIZE) {
        printf("out of range\n");
        return -1;
      }
    } else if (number == 2) {
      char name[10], second_name[10], tel[10];

      printf("Введите имя: ");
      scanf("%s", name);
      printf("Введите фамилию: ");
      scanf("%s", second_name);
      printf("Введите телефон: ");
      scanf("%s", tel);
      delete (directory, name, second_name, tel);

      count_abonents--;

      if (count_abonents < 0) {
        printf("out of range\n");
        return -1;
      }
    } else if (number == 3) {
      char name[10];

      printf("Введите имя: ");
      scanf("%s", name);
      search(directory, name);
    } else if (number == 4) {
      print(directory);
    } else if (number == 5) {
      break;
    } else {
      printf("\nincorrect number\n");
      break;
    }
  }

  return 0;
}

void add(abonent *dir, int index, char *name, char *sec_name, char *tel) {
  sprintf(dir[index].name, "%s", name);
  sprintf(dir[index].second_name, "%s", sec_name);
  sprintf(dir[index].tel, "%s", tel);
}

void delete(abonent *dir, char *name, char *sec_name, char *tel) {
  for (int n = 0; n < SIZE; n++)
    if (strcmp(dir[n].name, name) == 0 &&
        strcmp(dir[n].second_name, sec_name) == 0 &&
        strcmp(dir[n].tel, tel) == 0) {
      memset(dir[n].name, 0, sizeof(dir[n].name));
      memset(dir[n].second_name, 0, sizeof(dir[n].second_name));
      memset(dir[n].tel, 0, sizeof(dir[n].tel));
    }
}

void search(abonent *dir, char *name) {
  for (int n = 0; n < SIZE; n++)
    if (strcmp(dir[n].name, name) == 0)
      printf("%s %s\n", dir[n].name, dir[n].second_name);
}

void print(abonent *dir) {
  for (int n = 0; n < SIZE; n++)
    if (strlen(dir[n].name) > 0)
      printf("%s %s %s\n", dir[n].name, dir[n].second_name, dir[n].tel);
}
