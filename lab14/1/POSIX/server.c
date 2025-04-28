#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE_MSG 128
#define NAME_SHM "/Shmname"
#define NAME_SEM "/Semname"

struct msg_shm {
  int type;
  char message[SIZE_MSG];
};

int main() {
  struct msg_shm *shm;
  int shmid = shm_open(NAME_SHM, O_CREAT | O_RDWR, 0666);

  if (shmid < 0) {
    perror("shm_open");
    return -1;
  }

  ftruncate(shmid, sizeof(struct msg_shm));

  shm = mmap(NULL, sizeof(struct msg_shm), PROT_READ | PROT_WRITE, MAP_SHARED,
             shmid, 0);

  if (shm == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  sem_t *sem = sem_open(NAME_SEM, O_CREAT, 0666, 1);

  if (sem == SEM_FAILED) {
    perror("sem_open");
    return -1;
  }

  if (shm->type == 0) {
    sem_wait(sem);
    strcpy(shm->message, "Hi!");
    sem_post(sem);
  }

  while (shm->type != 1)
    ;

  sem_wait(sem);

  if (shm->type == 1)
    printf("%s\n", shm->message);

  shm->type = 0;

  sem_post(sem);
  sem_close(sem);

  if (sem_unlink(NAME_SEM) || shm_unlink(NAME_SHM)) {
    perror("unlink");
    return -1;
  }

  return 0;
}