#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
  int sig;

  sprintf(str, "%d", getpid());
  write(fd, str, sizeof(str));

  sigset_t mask;

  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);

  if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
    perror("sigprocmask");
    return -1;
  }

  while (1) {
    sigwait(&mask, &sig);
    printf("signal - %d\n", sig);
  }

  close(fd);
  unlink(argv[1]);

  return 0;
}