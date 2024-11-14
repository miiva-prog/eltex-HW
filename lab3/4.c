#include <stdio.h>
#define SIZE_STRING 100

char *sstr(char *strB, char *strA);

int main() {
  char arr[SIZE_STRING], arr2[SIZE_STRING];

  printf("input string: ");
  scanf("%s", arr);
  printf("input postmortem: ");
  scanf("%s", arr2);

  char *str = sstr(arr, arr2);

  if (str == NULL)
    printf("NULL!!!\n");
  else
    printf("%s\n", str);

  return 0;
}

char *sstr(char *strB, char *strA) {
  char *ptrB = NULL, *ptrA = NULL;

  while (*strB != '\0') {
    if (*strB == *strA) {
      ptrA = strA;
      ptrB = strB;

      while (*ptrA == *ptrB && *ptrB != '\0') {
        ptrA++;
        ptrB++;
      }

      if (*ptrA == '\0')
        return strB;
    }

    strB++;
  }

  return NULL;
}