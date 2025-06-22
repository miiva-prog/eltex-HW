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

int create_driver(struct driver *car);

void send_task(struct driver car);

int get_status(struct driver car);

void get_drivers(struct driver *car);