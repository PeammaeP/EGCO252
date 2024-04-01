/* After the #includes, the function prototypes and the global variable, we come
to the main function. There the semaphore is created with a call to semget,
which returns the semaphore ID. If the program is the rst to be called (i.e.
it's called with a parameter and argc > 1), a call is made to set_semvalue to
initialize the semaphore and op_char is set to X. [./sem & ./sem 1]*/

/* Group 1 Member : 
Chutiya Thanaluck 6513111
Anapat Kitsommart 6513123
Purin Pongpanich 6513135
Mahannop Thabua 6513136
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

static int set_semvalue(int);
static void del_semvalue(int);
static int semaphore_p(int);
static int semaphore_v(int);
static void f1(int);
static void f2(int);
static int s1,s2;

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};

int main(int argc, char *argv[]) {
  int i;
  int pause_time;
  
  pid_t child; 
  child = fork();
  
  srand((unsigned int)getpid());
  // generate the semaphore section with the ley
  s1 = semget((key_t)1234, 1, 0666 | IPC_CREAT);
  s2 = semget((key_t)4321, 1, 0666 | IPC_CREAT);

  // if the error occurs, print the error message and exit
  if (argc > 1) {
    if (!(set_semvalue(s1) || set_semvalue(s2))) {
      fprintf(stderr, "Failed to initialize semaphore\n");
      exit(EXIT_FAILURE);
    }
    sleep(2);
  }

  // Fork the process ( Checking between the parent and child)
  switch(child) {
    case -1:
      printf("Can't Create The Child Process , Maybe There is error is something ..\n"); 
      exit(EXIT_FAILURE); 
      break;
    // Child Process 
    case 0 :
      // Call the F2 , Pass The Argument by pid_t variables
      // if we uncomment the line of the semaphores , then the process could be done.
      f2(child);
      //printf("Child Process called : Done !\n");
      break;
    // Parent Process 
    default :
      // Call the F1 , Pass The Argument by pid_t variables
      // if we uncomment the line of the semaphores , then the process could be done.
      f1(child); 
      //printf("Parent Process called : Done !\n"); 
      break; 
  }
  printf("Done !\n"); 
  printf("\n%d - finished\n", getpid());
  // delete the semaphores s1 , s2 
  if (argc > 1) {
    sleep(5);
    del_semvalue(s1);
    del_semvalue(s2);
  }
  // exit with status success 
  exit(EXIT_SUCCESS);
} 


/* The function set_semvalue initializes the
 semaphore using the SETVAL command in a
 semctl call. We need to do this before we can
 use the semaphore. */
static int set_semvalue(int sem_id) {
  union semun sem_union;
  sem_union.val = 1;
  if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
    return (0);
  return (1);
}

/* The del_semvalue function has almost the same
form, except the call to semctl uses the command
IPC_RMID to remove the semaphore's ID. */

static void del_semvalue(int sem_id) {
  union semun sem_union;
  // delete the semaphores
  if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
    fprintf(stderr, "Failed to delete semaphore\n");
} 

/* semaphore_p changes the semaphore
 by -1 (waiting). */

static int semaphore_p(int sem_id) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = -1; /* P() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(sem_id, &sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_p failed\n");
    return (0);
  }
  return (1);
} 

/* semaphore_v is similar except for setting
 the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static int semaphore_v(int sem_id) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = 1; /* V() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(sem_id, &sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_v failed\n");
    return (0);
  }
  return (1);
}

// call the function of the f1 
void f1(pid_t pid){
  // print while the process is working 
  printf("[%d] f1 working in the process \n" , getpid());
  // call the semaphore_p to wait the section
  semaphore_p(s1);
  // if child process then call the f1 
  if(pid) f2(pid);
  // call the semaphore_v to release the section
  semaphore_v(s1);
}
// call the function of the f2 
void f2(pid_t pid){
  // print while the process is working 
  printf("[%d] f2 working in the process \n" , getpid());
  // call the semaphore_p to wait the section
  semaphore_p(s2);
  // if child process then call the f1 
  if(!pid) f1(pid);
  // call the semaphore_v to release the section
  semaphore_v(s2);
}
