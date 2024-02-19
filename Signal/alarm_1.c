#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void my_alarm(int sig) { 
    for (int i = 0; i < 5; i++) 
    printf("Alarm !\n"); 
}

int main(void) {
    int pid;
    printf("Alarm clock is starting …\n");

    if ((pid = fork()) == 0) { 
        sleep(5); 
        kill(getppid(), SIGALRM); 
        exit(0); 
    }

    printf("Waiting for alarm …\n");

    signal(SIGALRM, my_alarm);

    pause(); 
    printf("Done !\n"); 
    exit(0);
}