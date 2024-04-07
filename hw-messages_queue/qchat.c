/* Group 1 Member : 
Chutiya Thanaluck 6513111
Anapat Kitsommart 6513123
Purin Pongpanich 6513135
Mahannop Thabua 6513136
*/

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>

#define MSG_SIZE 128

static void sig_end(){
  wait(NULL);
  exit(EXIT_SUCCESS);
}

// Define a structure for the message
struct message_buffer {
    long message_from;
    char message_text[MSG_SIZE];
};

int main(int argc, char *argv[]) {
  // argv[1] = this user argv[2] = other user
  key_t key = 6513136;
  int msgid;
  struct message_buffer message;
  char buffer[MSG_SIZE] = "";
  int n_byte;

  // Validate data
  if (argc != 2){
    exit(EXIT_FAILURE);
  }

  // Verify data
  if (atoi(argv[1]) != 1 && atoi(argv[1]) != 2) {
      exit(EXIT_FAILURE);
  }

  // Setup other uesr data
  if(atoi(argv[1]) == 1){
    argv[2] = "2";
  }else{
    argv[2] = "1";
  }

  // Create signal handler
  signal(SIGUSR1, sig_end);

  // Create a message queue
  msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
      perror("msgget");
      exit(EXIT_FAILURE);
  }

  // Create child process to receive message
  pid_t child = fork();

  // Assign child process to receive message and parent to send message
  switch(child){
    case -1: printf("Fork error"); exit(EXIT_FAILURE);
    case 0:
      while(strncmp(message.message_text, "end chat", 8) != 0){
        // Receive the message from other user
        if (msgrcv(msgid, &message, sizeof(message), atoi(argv[2]), 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("User %ld: %s", message.message_from, message.message_text);
      }

      // Remove the message queue
      if (msgctl(msgid, IPC_RMID, NULL) == -1) {
          perror("msgctl");
          exit(EXIT_FAILURE);
      }
      break;
    default:
      while (strncmp(buffer, "end chat", 8) != 0) {
        // Reset buffer
        memset(buffer, 0, MSG_SIZE);
        
        // Send message to other user
        if(read(0, buffer, MSG_SIZE) > 0){
          message.message_from = atoi(argv[1]);
          strcpy(message.message_text,buffer);

          if (msgsnd(msgid, &message, sizeof(message), 0) == -1) {
              perror("msgsnd");
              exit(EXIT_FAILURE);
          }
        }
      }
  }

  // Kill child process
  if(child > 0){
    kill(child, SIGUSR1);
    wait(NULL);
  }else if (child == 0){
    kill(getppid(),SIGUSR1);
  }

  exit(EXIT_SUCCESS);
}