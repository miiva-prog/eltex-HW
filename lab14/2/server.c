#include "chat.h"

int main() {
  sem_unlink(NAME_SEM);
  shm_unlink(NAME_SHM);

  struct msg_shm *shm_us;
  int shmuser = shm_open(NAME_SHM, O_CREAT | O_RDWR, 0666);

  if (shmuser < 0) {
    perror("shm_open");
    return -1;
  }

  ftruncate(shmuser, sizeof(struct chatusers));

  shm_us = mmap(NULL, sizeof(struct chatusers), PROT_READ | PROT_WRITE,
                MAP_SHARED, shmuser, 0);

  if (shm_us == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  sem_t *sem = sem_open(NAME_SEM, O_CREAT, 0666, 1);

  if (sem == SEM_FAILED) {
    perror("sem_open");
    return -1;
  }

  char buf[5];

  while (1) {
    fgets(buf, sizeof(buf), stdin);

    if (strncmp(buf, "exit", 4) == 0)
      break;
  }

  if (sem_close(sem) < 0) {
    perror("sem_close");
    return -1;
  }

  if (sem_unlink(NAME_SEM) || shm_unlink(NAME_SHM)) {
    perror("unlink");
    return -1;
  }

  return 0;
}