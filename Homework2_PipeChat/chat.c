#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_1 "./fifo1to2"
#define FIFO_2 "./fifo2to1"

#define MAX_RBUF 80

int FIFO_FD1 , FIFO_FD2; 

int main(int argc , char *argv[]) {
    int child , nbytes; 
    // contains the buffer 
    char rbuf[MAX_RBUF] = ""; 

    if(argc != 2) {
        fprintf(stderr , "Usage : %s <[1,2]>\n", *argv); 
        exit(EXIT_FAILURE); 
    }

    /*  The Below Condition is checking if the file was not created ( return status -1 )
    , make the new file with the highest permission ( 0777 ). In the nested if , 
    there is a condition to check the if the file create unsuccessfully */ 

    if(access(FIFO_1 , F_OK) == -1) {
        FIFO_FD1 = mkfifo(FIFO_1 , 0777); 

        if(FIFO_FD1) {
            fprintf(stderr , "Could not create the first file with %s\n",FIFO_FD1);
            exit(EXIT_FAILURE); 
        }
    }

    if(access(FIFO_2 , F_OK) == -1) {
        FIFO_FD2 = mkfifo(FIFO_2 , 0777); 
        
        if(FIFO_FD2) {
            fprintf(stderr , "Could not create the second file with %s\n",FIFO_FD2);
            exit(EXIT_FAILURE); 
        }
    }

    /* If the file was created ( FIFO_FD1 and FIFO_FD2 ) then open file within the below code.
    The both files permission was set in the read only mode. */
    FIFO_FD1 = open(FIFO_1 , O_WRONLY);
    FIFO_FD2 = open(FIFO_2 , O_WRONLY);

    // Avoid to Read the Name File ( That was not relevant with this code )
    argv++; 
    
    if(strcmp(*argv , "1") == 0) {
        pid_t pid;
        pid = fork(); 

        switch(pid) {
            case -1 : 
                perror("Forking Failed !");
                exit(EXIT_FAILURE); 
                break;
            case 0 : 
                while(strncmp(rbuf , "end chat" , 8)) {

                }

                break;
            default : 
                while(strncmp(rbuf , "end chat" , 8)) {

                }
        }
    }

    if(strcmp(*argv , "2" == 0)) {
        pid_t pid;
        pid = fork(); 

        switch(pid) {
            case -1 :
                perror("Could not Fork , Process was failed ");
                exit(EXIT_FAILURE);
                break; 
            case 0 : 
                while(strncmp(rbuf , "end chat" , 8)) {

                }
                break; 
            default : 
                while(strncmp(rbuf , "end chat" , 8)) {

                }
        }
    } 

    if(FIFO_FD1 != -1) {
        close(FIFO_FD1);
    }
    if(FIFO_FD2 != -1) {
        close(FIFO_FD2); 
    }
    
    exit(EXIT_SUCCESS);    
    return 0; 
}