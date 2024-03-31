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

struct shm_st {
  int written, type;
  char data[BUFSIZ];
};

static void sig_end() { exit(EXIT_SUCCESS); }

int main(int argc, char *argv[]) {
  int shmID, child, running = 1;
  void *sh_mem = NULL;
  struct shm_st *sh_area;
  char buffer[BUFSIZ] = "";
  //srand((unsigned int)getpid());

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <1,2>\n", *argv);
    exit(EXIT_FAILURE);
  }

  shmID = shmget((key_t)21930, MEM_SIZE, 0666 | IPC_CREAT);

  if (shmID == -1) {
    fprintf(stderr, "shmget failed\n");
    exit(EXIT_FAILURE);
  }

  sh_mem = shmat(shmID, NULL, 0);

  if (sh_mem == (void *)-1) {
    fprintf(stderr, "shmat failed\n");
    exit(EXIT_FAILURE);
  }

  sh_area = (struct shm_st *)sh_mem;

  sh_area->written = 0;
  strcpy(sh_area->data, "0");
  signal(SIGUSR1,sig_end);
  argv++;

  if (strcmp(*argv, "1") == 0) {
    child = fork();
    
    if(child == -1) {
      perror("Forking failed");
      exit(EXIT_FAILURE);
    }
    
    else if(child == 0) {
      while (running) {
        if (sh_area->written == 1 && sh_area->type == 2) {
          printf("-------------------\n");
          printf("User 2 : \n");
          write(2, sh_area->data, strlen(sh_area->data));
          sh_area->written = 1;
          sh_area->type = 1;
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
      
    else{
      do {
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
    
    if(child == -1){
      perror("Forking failed");
      exit(EXIT_FAILURE);
    }
    
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
