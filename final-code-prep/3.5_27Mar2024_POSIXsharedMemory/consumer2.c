// Consumer.c
#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define SHM_NAME "/pshm"
#define MEM_SIZE 64
struct shm_st {
  atomic_int written ;//= ATOMIC_VAR_INIT(0);
  char data[MEM_SIZE];
};
int main() {
  int running = 1, shmfd, st_size;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  st_size = sizeof(struct shm_st);
  shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0660);
  if (shmfd == -1) {
    perror("shm_open failed ");
    exit(EXIT_FAILURE);
  }
  ftruncate(shmfd, st_size);
  sh_mem = mmap(0, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  printf("Memory attached at %p\n", sh_mem);
  sh_area = (struct shm_st *)sh_mem;
  atomic_store(&sh_area->written, 0);
  while (running) {
    if (atomic_load(&sh_area->written)) {
      printf("Data written in shared memory : %s", sh_area->data);
      atomic_store(&sh_area->written, 0);
      if (strncmp(sh_area->data, "end", 3) == 0)
        running = 0;
    }
  }
  munmap(sh_mem, st_size);
  close(shmfd);
  if (shm_unlink(SHM_NAME) == -1) {
    perror("shm_unlink failed ");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}