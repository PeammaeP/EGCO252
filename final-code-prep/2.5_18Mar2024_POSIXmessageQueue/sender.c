// Sender.c
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define Q_NAME "/my_mq"
#define Q_PERM 0660      // permission
#define MAX_MSG 8        // max number of messages
#define MAX_MSG_SIZE 256 // max size of message
#define BUF_SIZE 64      // max size of buffer

struct msgs {
  int written;
  char data[BUF_SIZE];
};

int main(int argc, char *argv[]) {
  int running = 1;
  struct msgs a_msg;
  struct mq_attr q_attr;
  mqd_t mqd;
  unsigned int priority = 0;
  char buffer[BUF_SIZE], *s;

  // Initial valu for attriubte
  q_attr.mq_maxmsg = MAX_MSG;
  q_attr.mq_msgsize = MAX_MSG_SIZE;

  priority = atoi(argv[1]);
  
  mqd = mq_open(Q_NAME, O_WRONLY | O_CREAT, Q_PERM, &q_attr);
  if (mqd == -1) {
    perror("mq_open failed ");
    exit(EXIT_FAILURE);
  }
  while (running) {
    printf("Enter data: ");
    s= fgets(buffer, BUF_SIZE, stdin);
    printf("Size of msgs: %lu\n", sizeof(struct msgs));
    strcpy(a_msg.data, buffer);
    if (mq_send(mqd, (void *)&a_msg, sizeof(struct msgs), priority) == -1) {
      perror("mq_send failed ");
      exit(EXIT_FAILURE);
    }
    if (strncmp(buffer, "end", 3) == 0)
      running = 0;
  }

  mq_close(mqd);
  exit(EXIT_SUCCESS);
}