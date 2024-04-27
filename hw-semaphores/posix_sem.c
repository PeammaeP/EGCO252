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
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h> 

static void f1(int);
static void f2(int);
static sem_t *s1;
static sem_t *s2;

int main(int argc, char *argv[]) {
  int i;
  int pause_time;
  sem_t *mysem;

  pid_t child; 
  child = fork();

  srand((unsigned int)getpid());
  // generate the POSIX Semaphore with the file directory and initialized the permission and declare type of the semaphore with unlock semaphore,
  s1 = sem_open("/s1", O_CREAT|O_RDWR, 0660, 1);
  s2 = sem_open("/s2", O_CREAT|O_RDWR, 0660, 1);

  // Fork the process ( Checking between the parent and child)
  switch(child) {
    case -1:
      printf("Can't Create The Child Process , Maybe There is error is something ..\n"); 
      exit(EXIT_FAILURE); 
      break;
    // Child Process 
    case 0 :
      // Call the F2 , Pass The Argument by pid_t variables
      // if we uncomment the line of the semaphores implementation ( sem_wait and sem_post ) , then the process could be done.
      f2(child);
      //printf("Child Process called : Done !\n");
      break;
    // Parent Process 
    default :
      // Call the F1 , Pass The Argument by pid_t variables
      // if we uncomment the line of the semaphores implementation ( sem_wait and sem_post ) , then the process could be done.
      f1(child); 
      //printf("Parent Process called : Done !\n"); 
      break; 
  }
  
  printf("Done !\n"); 
  printf("\n%d - finished\n", getpid());
  
  // delete the semaphores s1 , s2 
  sem_unlink("s1");
  sem_unlink("s2");
  
  // exit with status success 
  exit(EXIT_SUCCESS);
} 

// call the function of the f1 
void f1(pid_t pid) {
  // print while the process is working 
  printf("[%d] f1 working in the process \n" , getpid());
  // call the sem_wait of the s1 to wait the section
  sem_wait(s1);
  // if child process then call the f1 
  if(pid) f2(pid);
  // call the sem_post of the s2 to release the section
  sem_post(s1);
}
// call the function of the f2 
void f2(pid_t pid){
  // print while the process is working 
  printf("[%d] f2 working in the process \n" , getpid());
  // call the sem_wait of the s2 to wait the section
  sem_wait(s2);
  // if child process then call the f1 
  if(!pid) f1(pid);
  // call the sem_post of the s2 to release the section
  sem_post(s2);
}
