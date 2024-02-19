#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void rx_int(int sig) { 
    printf("SIGINT - %d Received!!!\n", sig); 
}

int main(void) {
    struct sigaction act;
    // set up each varible in structur ** essential ** 
    act.sa_handler = rx_int;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    // using the function 
    sigaction(SIGINT, &act, 0);

    while(1) { 
        printf("Running...\n"); 
        sleep(1); 
    }
}