#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) // reader.c
{
  int n_bytes, fd;
  char buffer[BUFSIZ + 1];

  memset(buffer, '\0', sizeof(buffer));

  sscanf(argv[1], "%d", &fd);
  n_bytes = read(fd, buffer, BUFSIZ);
  
  printf("%d bytes have been read from %d : %s\n", n_bytes, getpid(), buffer);
  exit(EXIT_SUCCESS);
}