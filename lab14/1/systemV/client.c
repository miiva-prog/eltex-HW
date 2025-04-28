#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define KEY_SHM 10
#define KEY_SEM 11
#define SIZE_MSG 128

struct msg_shm {
  int type;
  char message[SIZE_MSG];
};

int main() {
  struct msg_shm *shm;

  int semid = semget(KEY_SEM, 1, 0);

  if (semid < 0) {
    perror("semget");
    return -1;
  }

  int shmid = shmget(KEY_SHM, sizeof(struct msg_shm), 0);

  if (shmid < 0) {
    perror("shmget");
    return -1;
  }

  shm = (struct msg_shm *)shmat(shmid, 0, 0);

  if (shm == NULL) {
    perror("shmat");
    return -1;
  }

  if (shm->type == 0) {
    semctl(semid, 0, SETVAL, 1);

    if (shm->type == 0)
      printf("%s\n", shm->message);

    shm->type = 1;

    semctl(semid, 0, SETVAL, 0);
  }

  if (shm->type != 0) {
    semctl(semid, 0, SETVAL, 1);

    shm->type = 1;
    strcpy(shm->message, "Hello");

    semctl(semid, 0, SETVAL, 0);
  }

  if (shmdt(shm) < 0) {
    perror("sgmdt");
    return -1;
  }

  return 0;
}