/* Group 1 Member :
Chutiya Thanaluck 6513111
Anapat Kitsommart 6513123
Purin Pongpanich 6513135
Mahannop Thabua 6513136
*/

#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define FILE_LENGTH 2*(BUFSIZ+12)

struct mm_st{
  int written_1, written_2;
  int pre_1, pre_2;
  int cur_1, cur_2;

  char data_1[BUFSIZ];
  char data_2[BUFSIZ];

};

static void sig_end(){
  wait(NULL);
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
  int fd, child;
  char buffer[BUFSIZ];
  void *mapped = NULL;

  // Validate data
  if(argc != 2){
    exit(EXIT_FAILURE);
  }

  if(strcmp(argv[1], "1") != 0 && strcmp(argv[1], "2") != 0){
    exit(EXIT_FAILURE);
  }

  // Setup data
  if(strcmp(argv[1], "1") == 0){
    argv[2] = "2";
  }else{
    argv[2] = "1";
  }

  //Open file
  fd = open("Chat log", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  if(fd == -1){
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  lseek (fd, FILE_LENGTH+1, SEEK_SET);
  write(fd, "", 1);
  lseek (fd, 0, SEEK_SET);

  // Create memory mapping
  mapped = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (mapped == MAP_FAILED) {
      perror("Error mapping file");
      exit(EXIT_FAILURE);
  }

  struct mm_st *mm_area;
  mm_area = (struct mm_st *)mapped;

  memset(mm_area, 0, FILE_LENGTH);

  mm_area -> written_1 = 0;
  mm_area -> written_2 = 0;
  mm_area -> pre_1 = 0;
  mm_area -> pre_2 = 0;
  mm_area -> cur_1 = 0;
  mm_area -> cur_2 = 0;

  close(fd);

  signal(SIGUSR1, sig_end);

  // Fork child process
  child = fork();

  switch(child){
    case -1: printf("Fork error"); exit(EXIT_FAILURE);
    case 0:
      while(strncmp(buffer, "end chat", 8) != 0){
        // Receive the message from other user
        if(atoi(argv[1]) == 1 && mm_area -> written_2){
          int j = 0;
          write(1, "User 2:", 7);
          for(int i = mm_area -> pre_2; i < mm_area -> cur_2; i++,j++){
            buffer[j] = mm_area -> data_2[i];
          }
          buffer[j] = '\0'; // Null-terminate buffer
          write(1, buffer, j);
          mm_area -> written_2 = 0;
        }else if(atoi(argv[1]) == 2 && mm_area -> written_1){
          int j = 0;
          write(1, "User 1:", 7);
          for(int i = mm_area -> pre_1; i < mm_area -> cur_1; i++,j++){
            buffer[j] = mm_area -> data_1[i];
          }
          buffer[j] = '\0'; // Null-terminate buffer
          write(1, buffer, j);
          mm_area -> written_1 = 0;
        }
        memset(buffer, 0, BUFSIZ); // Reset buffer
      }
    break;
    default:
      while(strncmp(buffer, "end chat", 8) != 0){
        // Receive input from user and send to other user
        int nbytes = read(0,buffer, BUFSIZ);
        if(nbytes > 0 && atoi(argv[1]) == 1){
          mm_area -> written_1 = 1;
          mm_area -> pre_1 = mm_area -> cur_1;
          mm_area -> cur_1 += nbytes;
          strcat(mm_area -> data_1, buffer);
        }else if(nbytes > 0 && atoi(argv[1]) == 2){
          mm_area -> written_2 = 1;
          mm_area -> pre_2 = mm_area -> cur_2;
          mm_area -> cur_2 += nbytes;
          strcat(mm_area -> data_2, buffer);
        }
      }
  }

  // Kill child process
  if(child > 0){
    kill(child, SIGKILL);
    wait(NULL);
  }else if (child == 0){
    kill(getppid(),SIGUSR1);
  }

  munmap (mapped, FILE_LENGTH);
  exit(EXIT_SUCCESS);
  
}
