#include <stdio.h>
#define SIZE 20

int main(int argc, char **argv) {
  FILE *file = fopen(argv[1], "w");

  if (file == NULL) {
    printf("can't open file\n");
    return -1;
  }

  char arr[SIZE];

  fprintf(file, "%s", "String from file");
  fclose(file);

  file = fopen(argv[1], "r");

  if (file == NULL) {
    printf("can't open file\n");
    return -1;
  }

  fgets(arr, SIZE, file);
  printf("%s\n", arr);
  fclose(file);

  return 0;
}