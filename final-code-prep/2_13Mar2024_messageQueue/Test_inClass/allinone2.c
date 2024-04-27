// allinone.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

struct a_msg {
  long int msg_type;
  char data[BUFSIZ];
};
int main(void) {
  int running = 1, msgID1, msgID2;
  struct a_msg a_msg;
  long int rcv_msg_type = 0;
  char buffer[BUFSIZ];

  msgID1 = msgget((key_t)1234, 0666 | IPC_CREAT);
  msgID2 = msgget((key_t)1233, 0666 | IPC_CREAT);
  if (msgID1 == -1) {
    fprintf(stderr, "msgget failed\n");
    exit(EXIT_FAILURE);
  }
  while (running) {
    if (msgrcv(msgID1, (void *)&a_msg, BUFSIZ, rcv_msg_type, IPC_EXCL) == -1) {
      fprintf(stderr, "msgrcv failed\n");
      exit(EXIT_FAILURE);
    }
    printf("Received Message: %s", a_msg.data);
    printf("Enter data: ");
    fgets(buffer, BUFSIZ, stdin);
    a_msg.msg_type = 1;
    strcpy(a_msg.data, buffer);
    //if (msgsnd(msgID, (void *)&a_msg, BUFSIZ, 0) == -1) {
    if (msgsnd(msgID2, (void *)&a_msg, BUFSIZ, IPC_NOWAIT) == -1) {
      fprintf(stderr, "msgsnd failed\n");
      //exit(EXIT_FAILURE);
    }


      if (strncmp(a_msg.data, "end", 3) == 0)
        running = 0;

    if (strncmp(buffer, "end", 3) == 0)
      running = 0;
  }

  if (msgctl(msgID2, IPC_RMID, 0) == -1) {
    fprintf(stderr, "msgctl failed\n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}