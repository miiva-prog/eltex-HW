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

int search_arguments(char *input, char **args1, char **args2) {
  char *token = strtok(input, " ");
  int index1 = 0, index2 = 0;
  int found = 0;

  while (token != NULL && (index1 + index2) < ARGS - 1) {
    if (strcmp(token, "|") == 0) {
      token = strtok(NULL, " ");
      found = 1;
      continue;
    }

    if (found == 0)
      args1[index1++] = token;
    else
      args2[index2++] = token;

    token = strtok(NULL, " ");
  }

  args1[index1] = args2[index2] = NULL;

  return index1;
}