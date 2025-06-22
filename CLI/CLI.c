#include "CLI.h"

int count_drivers = 0;

int create_driver(struct driver *car) {
  if (car[count_drivers].pid != 0)
    count_drivers++;

  if (count_drivers > MAX_PROCESS)
    return -1;

  int pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    car[count_drivers].pid = getpid();
  } else {
    wait();
  }

  return pid;
}

void send_task(struct driver car) {
  if (car.pid == getpid()) {
    for (int n = 0; n < car.timer; n++) {
      if (car.condition != 0)
        printf("%d - Busy\n\n", car.pid);

      sleep(1);
    }

    printf("%d - Available\n\n", car.pid);
  }
}

int get_status(struct driver car) {
  if (car.condition == 0)
    printf("Available\n\n");
  else
    printf("Busy\n\n");

  return car.condition;
}

void get_drivers(struct driver *car) {
  for (int n = 0; n < count_drivers + 1; n++) {
    if (car[n].condition == 0)
      printf("%d)Available\n\n", n);
    else
      printf("%d)Busy\n\n", n);
  }
}