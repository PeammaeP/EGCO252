#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  int n_bytes, pipefd[2];
  char buffer[BUFSIZ + 1], data[BUFSIZ];
  
  memset(buffer, '\0', sizeof(buffer));

  if (pipe(pipefd) == 0) {
    read(0, data, BUFSIZ); //Get input from stdin

    // create the writer part 
    n_bytes = write(pipefd[1], data, strlen(data));
    printf("%d bytes have been written!\n", n_bytes);

    //create the reader part 
    n_bytes = read(pipefd[0], buffer, BUFSIZ); 
    printf("%d bytes have been read!\n", n_bytes);

    printf("Data read from the pipe: %s\n", buffer);
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_FAILURE);
}