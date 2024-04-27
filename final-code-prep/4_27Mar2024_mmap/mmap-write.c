// Write a Random Number to a Memory-Mapped File
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

/* Return a uniformly random number in the range [low,high]. */
int random_range(unsigned const low, unsigned const high) {
  unsigned const range = high - low + 1;
  return low + (int)(((double)range) * rand() / (RAND_MAX + 1.0));
}
int main(int argc, char *const argv[]) {
  //Run with ./mmap-write.c <filename>
  //Run this file before read
  //Ex. ./mmap-write ./demo
  if(argc == 1) {
    printf("You need to enter a file name\n");
    printf("Ex. ./mmap-write ./demo\n");
    exit(EXIT_FAILURE);
  }
  int fd;
  void *file_memory;
  /* Seed the random number generator. */
  srand(time(NULL));
  /* Prepare a file large enough to hold an unsigned integer. */
  //If you not initialize the file it will be throw segmentation falut
  fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  lseek(fd, FILE_LENGTH + 1, SEEK_SET);
  write(fd, "", 1);
  lseek(fd, 0, SEEK_SET);
  /* Create the memory mapping. */
  file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  /* Write a random integer to memory-mapped area. */
  //sprintf((char *)file_memory, "%d\n", random_range(-100, 100));
  int mark_num = random_range(-100, 100);
  sprintf((char *)file_memory, "%d\n", mark_num);
  printf("Mark number %d is written to the memory-mapped file %d\n",mark_num, mark_num);
  /* Release the memory (unnecessary because the program exits). */
  munmap(file_memory, FILE_LENGTH);
  return 0;
}