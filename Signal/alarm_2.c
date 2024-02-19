#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

/* one handler for both signals */
static void sig_usr(int);

/* argument is signal number */
static void sig_usr(int signo) {
    if (signo == SIGUSR1)
        printf("Received SIGUSR1\n");

    else if (signo == SIGUSR2)
        printf("Received SIGUSR2\n");

    else 
        printf("Received signal %d\n", signo);
}

int main(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        perror("Can't catch SIGUSR1");

    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        perror("Can't catch SIGUSR2");

    for ( ; ; ) pause();

    // ที่ไม่ทำงานถูกต้องแล้วเพราะเราสั่ง Pause ไว้ 
    // ใช้คำสั่ง kill -(number of process)
}