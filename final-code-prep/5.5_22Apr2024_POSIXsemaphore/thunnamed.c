/* Running with “./thunnamed”*/

// Multiple Threads

#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PAUSE_TIME 800

sem_t mysem;

void *thread_func(void *arg) {
  char *op_char = (char *)arg;
  for (int i = 0; i < 10; i++) {
    if (sem_wait(&mysem) == -1)
      exit(EXIT_FAILURE);

    // Begining of the critical section
    printf("[%c", *op_char);
    fflush(stdout);
    usleep(PAUSE_TIME);
    printf("%c]", *op_char);
    fflush(stdout);
    // End of the critical section
    usleep(PAUSE_TIME);

    if (sem_post(&mysem) == -1)
      exit(EXIT_FAILURE);
  }
  return NULL;
}

int main() {
  pthread_t tid1, tid2;
  char op_char1 = 'O';
  char op_char2 = 'X';

  sem_init(&mysem, 0, 1);

  // All threads must be created before joining. If not it is the sequencial programming.
  pthread_create(&tid1, NULL, thread_func, &op_char1);
  pthread_create(&tid2, NULL, thread_func, &op_char2);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  sem_destroy(&mysem);
  printf("\nDone!\n");

  exit(EXIT_SUCCESS);
}