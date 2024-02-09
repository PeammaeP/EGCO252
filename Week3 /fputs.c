#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
  char block[BUFSIZ];
  FILE *in , *out;
  int nread;

  in = fopen("file.in", "r");
  out = fopen("file.out", "w");

  while (fgets(block , sizeof(block), in)) { 
    fputs(block , out); 
  }

  fclose(in);

  return 0;
}