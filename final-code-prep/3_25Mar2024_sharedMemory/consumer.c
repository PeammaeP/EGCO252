// Consumer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#define MEM_SIZE 4096
struct shm_st {
  int written;
  char data[BUFSIZ];
};
int main() {
  int running = 1, shmid;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  srand((unsigned int)getpid());
  shmid = shmget((key_t)1234, MEM_SIZE, 0666 | IPC_CREAT);
  if (shmid == -1) {
    fprintf(stderr, "shmget failed\n");
    exit(EXIT_FAILURE);
  }
  sh_mem = shmat(shmid, NULL, 0);
  if (sh_mem == (void *)-1) {
    fprintf(stderr, "shmat failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Memory attached at %p\n", sh_mem);
  sh_area = (struct shm_st *)sh_mem;
  sh_area->written = 0;
  while (running) {
    if (sh_area->written) {
      printf("Data written in shared memory: %s\n", sh_area->data);
      //Print rand() value
      //Comment srand() first to see the same sequence result in each program
      int r = rand();
      printf("r = %d\n", r);
      sleep(rand() % 4);
      sh_area->written = 0;
      if (strncmp(sh_area->data, "end", 3) == 0)
        running = 0;
    }
    sleep(1);
  }
  if (shmdt(sh_mem) == -1 || shmctl(shmid, IPC_RMID, 0) == -1) {
    fprintf(stderr, "shmdt or shmctl failed\n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}