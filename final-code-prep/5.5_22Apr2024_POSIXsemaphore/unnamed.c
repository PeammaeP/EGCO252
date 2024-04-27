/* Running with “./unnamed”*/

// Multiple Processes

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#define SHM_NAME "/pshm"
#define MEM_SIZE 64
#define PAUSE_TIME 800
struct shm_st {
  sem_t sem;
  char data[MEM_SIZE];
};
int main() {
  int i, shmfd, st_size;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  char op_char;

  st_size = sizeof(struct shm_st);
  shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660);
  if (shmfd == -1) {
    perror("shm_open failed ");
    exit(EXIT_FAILURE);
  }
  // Add i = to prevent warning from compliler
  i = ftruncate(shmfd, st_size);
  sh_mem = mmap(0, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  printf("Memory attached at %p\n", sh_mem);
  sh_area = (struct shm_st *)sh_mem;
  if (sem_init(&sh_area->sem, 1, 1) == -1) {
    perror("sem_init: ");
    exit(EXIT_FAILURE);
  }

  int pid = fork();
  if (pid)
    op_char = 'X';
  else
    op_char = 'O';

  for (i = 0; i < 10; i++) {
    if (sem_wait(&sh_area->sem) == -1)
      exit(EXIT_FAILURE);

    // Begining of the critical section
    printf("[%c", op_char);
    fflush(stdout);
    usleep(PAUSE_TIME);
    printf("%c]", op_char);
    fflush(stdout);
    // End of the critical section

    if (sem_post(&sh_area->sem) == -1)
      exit(EXIT_FAILURE);
    usleep(PAUSE_TIME);
  }

  if (pid) {
    usleep(PAUSE_TIME);
    sem_destroy(&sh_area->sem);
    wait(NULL);
    printf("%d - finished\n", getpid());
  } else
    printf("\n%d - finished\n", getpid());

  exit(EXIT_SUCCESS);
}