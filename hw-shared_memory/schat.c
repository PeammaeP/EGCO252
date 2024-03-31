#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MEM_SIZE 4096

// declared the structure of the shared memory 
struct shm_st {
  int written, type;
  char data[BUFSIZ];
};

static void sig_end() { exit(EXIT_SUCCESS); }


int main(int argc, char *argv[]) {
  // define the essential variable e.g. Share Memory , Shared Area , Buffer Size
  int shmID, child, running = 1;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  char buffer[BUFSIZ] = "";
  // srand((unsigned int)getpid());

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <1,2>\n", *argv);
    exit(EXIT_FAILURE);
  }

  // Create the shared memory 
  shmID = shmget((key_t)21930, MEM_SIZE, 0666 | IPC_CREAT);

  if (shmID == -1) {
    fprintf(stderr, "shmget failed\n");
    exit(EXIT_FAILURE);
  }
  // Detaching Shared Memory ( Return Heap to Process )
  // First Address of Shared Memory 
  sh_mem = shmat(shmID, NULL, 0);

  if (sh_mem == (void *)-1) {
    fprintf(stderr, "shmat failed\n");
    exit(EXIT_FAILURE);
  }
  // typecasting in order to sh_area has struct shm_set type
  sh_area = (struct shm_st *)sh_mem;
  
  // Initialized the variable ( Not Write Anything )
  sh_area->written = 0;
  strcpy(sh_area->data, "0");
  signal(SIGUSR1,sig_end);
  argv++;

  if (strcmp(*argv, "1") == 0) {
    child = fork();
    
    // can't fork the process 
    if(child == -1) {
      perror("Forking failed");
      exit(EXIT_FAILURE);
    }
    // if child process 
    else if(child == 0) {
      while (running) {
        // if the data has written as the type from 2 ( schat2 )
        if (sh_area->written == 1 && sh_area->type == 2) {
          printf("-------------------\n");
          printf("User 2 : \n");
          write(2, sh_area->data, strlen(sh_area->data));
          sh_area->written = 0;
          //sh_area->written = 2;
          //sh_area->type = 1;
          printf("-------------------\n");

          if (strncmp(sh_area->data, "end chat", 8) == 0) {
            running = 0;
          }
        }
        sleep(1);
      }
      if (shmdt(sh_mem) == -1 || shmctl(shmID, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmdt or shmctl filed\n");
        exit(EXIT_FAILURE);
      }
    }
    // parent process 
    else{
      do {
        // Writing the buffer into the shared memory 
        fgets(buffer, BUFSIZ, stdin);
        sh_area->type = 1;
        strcpy(sh_area->data, buffer);
        sh_area->written = 1;
      } while (strncmp(buffer, "end chat", 8));
      if (shmdt(sh_mem) == -1 || shmctl(shmID, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmdt or shmctl filed\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  else if (strcmp(*argv, "2") == 0) {

    child = fork(); 
    
    if(child == -1) {
      perror("Forking failed");
      exit(EXIT_FAILURE);
    }
    // child process
    else if(child == 0){
      while (running) {
        if (sh_area->written == 1 && sh_area->type == 1) {
          printf("-------------------\n");
          printf("User 1 : \n");
          write(2, sh_area->data, strlen(sh_area->data));
          sh_area->written = 0;
          printf("-------------------\n");

          if (strncmp(sh_area->data, "end chat", 8) == 0) {
            running = 0;
          }
        }
        sleep(1);
      }
      if (shmdt(sh_mem) == -1 || shmctl(shmID, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmdt or shmctl filed\n");
        exit(EXIT_FAILURE);
      }
    }
    // parent process 
    else{
      do {
        fgets(buffer, BUFSIZ, stdin);
        sh_area->type = 2;
        strcpy(sh_area->data, buffer);
        sh_area->written = 1;
      } while (strncmp(buffer, "end chat", 8));

      if (shmdt(sh_mem) == -1 || shmctl(shmID, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmdt or shmctl filed\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  if (child > 0) {
    kill(child, SIGUSR1);
    wait(NULL);
  }

  else if (child == 0) {
    kill(getppid(), SIGUSR1);
  }

  exit(EXIT_SUCCESS);
}
