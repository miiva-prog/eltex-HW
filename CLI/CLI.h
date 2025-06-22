#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESS 20

struct driver {
  int pid;
  int timer;
  int condition;
};

extern int count_drivers;