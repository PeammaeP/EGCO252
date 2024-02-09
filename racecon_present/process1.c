// This code is about going to create the Race Condition 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid; 
    char *msg , c; 
    int n;

    printf("fork program starting\n");
    pid = fork();

    switch(pid) {
        case -1: exit(1);
        case 0: 
            msg = "Child !"; 
            n = 6; 
            break;
        default: 
            msg = "Parent !"; 
            n = 6; 
            break;
    }

    setbuf(stdout, NULL); // set unbuffered 
    
    for (; n>0; n--) {
        char *temp = msg;
        while(c = *temp++) {
            putc(c, stdout); 
            sleep(1);
        }
    }

     if (pid) {
        int stat_val; 
        pid_t child_pid;
        child_pid = wait(&stat_val);
        printf("Child has finished: PID = %d\n",child_pid);

     if (WIFEXITED(stat_val)) { 
        printf("Child exited with code %d\n", WEXITSTATUS(stat_val)); 
        }
     else printf("Child terminated abnormally\n");
    }
    exit(0);
}