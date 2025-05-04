#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void disp(int sig_num) {
  printf("call SIGUSR1 - (number signal)%d\n", sig_num);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    perror("arguments");
    return -1;
  }

  unlink(argv[1]);

  if (mkfifo(argv[1], 0666) == -1) {
    perror("mkfifo");
    return -1;
  }

  int fd = open(argv[1], O_RDWR);

  if (fd < 0) {
    perror("open");
    return -1;
  }

  char str[10];

  sprintf(str, "%d", getpid());
  write(fd, str, sizeof(str));

  struct sigaction new_disp;
  sigset_t mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);

  new_disp.sa_mask = mask;
  new_disp.sa_handler = disp;

  if (sigaction(SIGUSR1, &new_disp, NULL) < 0) {
    perror("sigaction");
    return -1;
  }

  while (1)
    sleep(1);

  close(fd);
  unlink(argv[1]);

  return 0;
}