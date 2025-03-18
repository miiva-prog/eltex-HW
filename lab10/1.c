#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int pid = fork();
  int code_return;

  if (pid < 0) {
    exit(code_return);
  } else if (pid != 0) {
    printf("parent:\npid - %d\n", getpid());
    printf("ppid - %d\n", getppid());
    wait();
    printf("code return - %d\n", WEXITSTATUS(code_return));
  } else {
    printf("child:\npid - %d\n", getpid());
    printf("ppid - %d\n", getppid());
    exit(code_return);
  }

  return 0;
}