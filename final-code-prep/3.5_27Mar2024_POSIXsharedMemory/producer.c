// Producer.c
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
  /*
    _Atomic int written; // Atomic integer
    _Atomic int *p1;
    _Atomic(int) *p2;
    atomic_int *p3; // Must use with stdatomic.h
  */
  /* Atomic does not have floating point type, so we use int instead */

  //_Atomic int written;

  int written;
  char data[MEM_SIZE];
};
int main() {
  int running = 1, shmfd, st_size;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  char buffer[BUFSIZ];
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
  sh_area->written = 0;
  while (running) {
    while (sh_area->written) {
      usleep(200);
    }
    printf("Enter data : ");
    fgets(buffer, BUFSIZ, stdin);
    strcpy(sh_area->data, buffer);
    sh_area->written = 1;
    if (strncmp(buffer, "end", 3) == 0)
      running = 0;
  }
  munmap(sh_mem, st_size);
  close(shmfd);
  exit(EXIT_SUCCESS);
}