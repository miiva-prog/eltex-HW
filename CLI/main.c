#include "CLI.h"

int main() {
  struct driver taxi[MAX_PROCESS];

  memset(&taxi, 0, sizeof(taxi));

  int pid = create_driver(taxi);

  if (pid < 0) {
    exit(EXIT_FAILURE);
  } else if (pid != 0) {
    wait();
  } else if (pid == 0) {
    while (1) {
      int part;

      printf("1)create_driver\n2)send_task\n3)get_status\n4)get_"
             "drivers\n\n");
      scanf("%d", &part);

      if (part == 1) {
        if (create_driver(taxi) < 0) {
          perror("create_driver");
          continue;
        }
      } else if (part == 2) {
        int index;

        printf("\ninput number car(%d):", count_drivers);
        scanf("%d", &index);

        if (index > count_drivers) {
          printf("out of range\n\n");
          continue;
        }

        taxi[count_drivers].condition = 0;

        printf("\ninput time:");
        scanf("%d", &(taxi[count_drivers].timer));
        send_task(taxi[count_drivers]);

      } else if (part == 3) {
        int index;

        printf("\ninput number car(%d):", count_drivers);
        scanf("%d", &index);

        if (index > count_drivers) {
          printf("out of range\n\n");
          continue;
        }

        get_status(taxi[index]);
      } else if (part == 4) {
        get_drivers(taxi);
      } else {
        continue;
      }
    }

    exit(EXIT_SUCCESS);
  }

  return 0;
}