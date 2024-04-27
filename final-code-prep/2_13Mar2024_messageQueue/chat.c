// Chat.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

struct a_msg {
  long int msg_type;
  char data[BUFSIZ];
};

pid_t parent_pid;
int main(int argc, char **argv) {
  pid_t pid;
  parent_pid = getpid();
  pid = fork();
  if (pid != 0) { // Parent
    int running = 1, msgID;
    struct a_msg a_msg;
    char buffer[BUFSIZ];

    msgID = msgget((key_t)atoi(argv[1]), 0666 | IPC_CREAT);
    if (msgID == -1) {
      fprintf(stderr, "msgget failed\n");
      exit(EXIT_FAILURE);
    }
    while (running) {
      printf("Enter data: ");
      fgets(buffer, BUFSIZ, stdin);
      a_msg.msg_type = 1;
      strcpy(a_msg.data, buffer);
      if (msgsnd(msgID, (void *)&a_msg, BUFSIZ, IPC_NOWAIT) == -1) {
        fprintf(stderr, "msgsnd failed\n");
        // exit(EXIT_FAILURE);
      }
      if (strncmp(buffer, "end", 3) == 0)
        running = 0;
    }
    exit(EXIT_SUCCESS);
  } else { // Child Process
    int running = 1, msgID;
    struct a_msg a_msg;
    long int rcv_msg_type = 0;
    msgID = msgget((key_t)atoi(argv[2]), 0666 | IPC_CREAT);
    if (msgID == -1) {
      fprintf(stderr, "msgget failed\n");
      exit(EXIT_FAILURE);
    }
    while (running) {
      if (msgrcv(msgID, (void *)&a_msg, BUFSIZ, rcv_msg_type, 0) == -1) {
        fprintf(stderr, "msgrcv failed\n");
        exit(EXIT_FAILURE);
      }
      printf("Received Message: %s", a_msg.data);
      if (strncmp(a_msg.data, "end", 3) == 0)
        running = 0;
    }
    if (msgctl(msgID, IPC_RMID, 0) == -1) {
      fprintf(stderr, "msgctl failed\n");
      exit(EXIT_FAILURE);
    }
     kill(pid, SIGINT);
  }
  
}