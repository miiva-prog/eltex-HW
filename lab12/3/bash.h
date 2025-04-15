#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARGS 50

void search_path(char *command, char *path);

int search_arguments(char *input, char **args1, char **args2);