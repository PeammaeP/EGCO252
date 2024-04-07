/* Group 1 Member : 
Chutiya Thanaluck 6513111
Anapat Kitsommart 6513123
Purin Pongpanich 6513135
Mahannop Thabua 6513136
*/

#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

// creating the 2 fifo ( fifo1to2 and fifo2to1)
#define FIFO_1 "./fifo1to2"
#define FIFO_2 "./fifo2to1"

// determine the maximum buffer size 
#define MAX_RBUF 80

// creating the 2 file ( file_1 and file_2 of each fifo )
int FIFO_FD1 , FIFO_FD2; 

int main(int argc , char *argv[]) {
    int child , nbytes; 

    // contains the buffer 
    char rbuf[MAX_RBUF] = ""; 

    // if the argument was not insert , return the printf of the " please usage [1,2]"
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
            fprintf(stderr , "Could not create the first file with %s\n",FIFO_1);
            exit(EXIT_FAILURE); 
        }
    }

    if(access(FIFO_2 , F_OK) == -1) {
        FIFO_FD2 = mkfifo(FIFO_2 , 0777); 
        
        if(FIFO_FD2) {
            fprintf(stderr , "Could not create the second file with %s\n",FIFO_2);
            exit(EXIT_FAILURE); 
        }
    }

    /* If the file was created ( FIFO_FD1 and FIFO_FD2 ) then open file within the below code.
    The both files permission was set in the read only mode. */

    // Avoid to Read the Name File ( That was not relevant with this code )
    argv++; 
    
    // check that the argument is 1
    if(strcmp(*argv , "1") == 0) {
        // open the file name ( both fifo1 and fifo2 )
        FIFO_FD1 = open(FIFO_1 , O_WRONLY);
        FIFO_FD2 = open(FIFO_2 , O_RDONLY);

        // build the child process 
        child = fork(); 

        switch(child) {
            // if the process was not created , return exit status 
            case -1 :
                perror("Could not Fork , Process was failed ");
                exit(EXIT_FAILURE);
                break; 
            case 0 : 
                // Child Process 
                // Creating the while loop ( user doesn't type " end chat " )
                while(strncmp(rbuf , "end chat" , 8)) {
                /*  
                Child Process : Reading File from FIFO_FD2 while to file_2 was sent the message
                to the file_1 using the nbytes to receive the previous file , write to the display
                */
                    if((nbytes = read(FIFO_FD2 , rbuf , MAX_RBUF)) > 0 ) {
                        printf("User 2 : \n");
                        write(1,rbuf,nbytes);
                    }
                }
                break; 
            default : 
                 /*
            Parent Process : Get The Input Message from the user keyboard and write into the
            file FIFO_FD2
            */
                while(strncmp(rbuf , "end chat" , 8)) {
                    if((nbytes = read(0,rbuf,MAX_RBUF)) > 0) {
                        write(FIFO_FD1,rbuf,nbytes);
                    }
                }
        }
        printf("\n");
    }

    // check if the argument is 2 
    if(strcmp(*argv , "2") == 0) {
        // open the file name ( both fifo1 and fifo2 )
        FIFO_FD1 = open(FIFO_1 , O_RDONLY);
        FIFO_FD2 = open(FIFO_2 , O_WRONLY);

         // build the child process 
        child = fork(); 

        switch(child) {
            // if the process was not created , return exit status 
            case -1 :
                perror("Could not Fork , Process was failed ");
                exit(EXIT_FAILURE);
                break; 
            case 0 : 
            // Child Process 
            // Creating the while loop ( user doesn't type " end chat " )
            /*  
            Child Process : Reading File from FIFO_FD1 while to file_1 was sent the message
            to the file_2 using the nbytes to receive the previous file , write to the display
            */
                while(strncmp(rbuf, "end chat" , 8)) {
                    if((nbytes = read(FIFO_FD1 , rbuf , MAX_RBUF)) > 0) {
                        printf("User 1 : \n");
                        write(1,rbuf,nbytes);
                    }
                }
                break; 
            default : 
            /*
            Parent Process : Get The Input Message from the user keyboard and write into the
            file FIFO_FD2
            */
                while(strncmp(rbuf , "end chat" , 8)) {
                    if((nbytes = read(0,rbuf,MAX_RBUF)) > 0) {
                        write(FIFO_FD2,rbuf,nbytes);
                    }
                }
        }
        printf("\n");
    } 

    if(child > 0) {
        // Parent Process -> Call the function sig_end 
        kill(child , SIGUSR1);
    }
    else if(child == 0) {
        // Child Process -> Call the function sig_end 
        kill(getppid() , SIGUSR1); 
    }
    
    // close both file ( fifo_1 , fifo_2 )
    if(FIFO_FD1 != -1) {
        close(FIFO_FD1);
    }
    if(FIFO_FD2 != -1) {
        close(FIFO_FD2); 
    }

    // exit with success status     
    exit(EXIT_SUCCESS);    
    return 0; 
}