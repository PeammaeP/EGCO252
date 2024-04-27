#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void) {
  FILE *write_fp;
  char buffer[BUFSIZ + 1];
  sprintf(buffer, "This is a message for writing to a process!\n"); // Print in memory (Buffer)
  write_fp = popen("od -c", "w");
  if (write_fp != NULL) {
    fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
    pclose(write_fp);
    exit(EXIT_SUCCESS);
  }
  exit(EXIT_FAILURE);
}