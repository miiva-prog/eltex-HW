#include "bash.h"

void search_path(char *command, char *path) {
  char arr[50];

  strcpy(arr, command);

  char *token = strtok(arr, " "), *argument = NULL;

  while (token != NULL) {
    argument = token;
    token = strtok(NULL, " ");
  }

  if (argument != NULL) {
    if (argument[0] == '/') {
      strcpy(path, argument);
    } else if (strcmp(argument, "..") == 0 ||
               strstr(argument, "../") == argument) {
      for (int i = 0; i < strlen(argument); i++)
        if (argument[i] == '.' && argument[i + 1] == '.') {
          char *slash = strrchr(path, '/');

          if (slash != NULL)
            *slash = '\0';

          i++;
        }

      if (strlen(path) == 0)
        strcpy(path, "/");
    }
  }
}

int search_arguments(char *input, char **args) {
  char *token = strtok(input, " ");
  int index = 0;

  while (token != NULL && index < ARGS - 1) {
    args[index++] = token;
    token = strtok(NULL, " ");
  }

  args[index] = NULL;

  return index;
}