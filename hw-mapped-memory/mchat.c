/* Group 1 Member :
    Chutiya Thanaluck 6513111
    Anapat Kitsommart 6513123
    Purin Pongpanich 6513135
    Mahannop Thabua 6513136
*/

#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define FILE_LENGTH 2 * (BUFSIZ + 12)

// define the structure of the mapped memory
struct mm_st
{
    int written_1, written_2;
    int pre_1, pre_2;
    int cur_1, cur_2;

    char data_1[BUFSIZ];
    char data_2[BUFSIZ];
};

// function of signal handler : Waiting for Child Process done then the parent process was done too.
static void sig_end()
{
    wait(NULL);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // Create the essential variable 
    // fd : file of chat-log 
    int fd;
    // child : it has the 2 processes in this code : child and parent and it's need to be handle properly
    pid_t child;
    // buffer : contains the input from the user
    char buffer[BUFSIZ];
    // mapped : Define The Memory Mapped which initialized with void type ( Typecasting Required )
    void *mapped = NULL;

    // Validate data
    if (argc != 2)
    {
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "1") != 0 && strcmp(argv[1], "2") != 0)
    {
        exit(EXIT_FAILURE);
    }

    // Setup data
    if (strcmp(argv[1], "1") == 0) {
        argv[2] = "2";
    }
    else {
        argv[2] = "1";
    }

    // Open file chat-log which has the properties : Read and Write 
    // also if the file was not exist , then create and set the permission to the owner to read and write
    fd = open("chat-log", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Write the Null Character to the End of File then set to the Initial Position of the file
    // If the file doesn't has Null Character : The Program wasn't know that what is the position of the end of file
    lseek(fd, FILE_LENGTH, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);

    // Create memory mapping
    mapped = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (mapped == MAP_FAILED)
    {
        perror("Error mapping file");
        exit(EXIT_FAILURE);
    }

    // create the memory mapped area
    struct mm_st *mm_area;
    // typecasting from void to mm_st type 
    // Bring The pointer mm_area and point to the mapped memory ( chat-log located )
    mm_area = (struct mm_st *)mapped;

    // Clear The Data in The Mapped Memory Area 
    memset(mm_area, 0, FILE_LENGTH);

    // Initialized Variable in the struct 
    mm_area->written_1 = 0;
    mm_area->written_2 = 0;
    mm_area->pre_1 = 0;
    mm_area->pre_2 = 0;
    mm_area->cur_1 = 0;
    mm_area->cur_2 = 0;

    close(fd);

    // Waiting for catch the SIGUSR1 signal to apply with function sig_end
    signal(SIGUSR1, sig_end);

    // Create the User following to the IPC Diagram.
    if (atoi(argv[1]) == 1) {
        // fork the process 
        child = fork();
        switch (child) {
        case -1:
            printf("Fork error");
            exit(EXIT_FAILURE);
            break;
        // Child Process : Display The Result to the User 1 ( From User 2 )
        case 0:
            while (strncmp(buffer, "end chat", 8) != 0) {
                // if the data from user 2 has been written
                if (mm_area->written_2) {
                    // clear buffer
                    memset(buffer , 0 , BUFSIZ);
                    int j = 0;
                    write(1, "User 2:\n", 8);
                    // copy the input from user 2 and write to the buffer
                    // in order to display to user 1 properly
                    for (int i = mm_area->pre_2; i < mm_area->cur_2; i++, j++)
                    {
                        buffer[j] = mm_area->data_2[i];
                    }
                    write(1, buffer, j);
                    // clear the written state of input user 2
                    mm_area->written_2 = 0;
                }
            }
            break;
        // Parent Process : Receive The Input from User 1 
        default:
            while(strncmp(buffer, "end chat", 8) != 0) {
                // clear the input ( buffer )
                memset(buffer, 0, BUFSIZ);
                int nbytes = read(0, buffer, BUFSIZ);
                if (nbytes > 1) {
                    // set the state that user 1 has been written the input 
                    mm_area->written_1 = 1;
                    // given the value previous and current 
                    mm_area->pre_1 = mm_area->cur_1;
                    // the current was looped in order to loop in the write the output 
                    mm_area->cur_1 += nbytes;
                    // contain the data ( input ) to buffer                     
                    strcat(mm_area->data_1, buffer);
                }
            }
        }
    }

    if (atoi(argv[1]) == 2) {
        // fork the process 
        child = fork();
        switch (child) {
        case -1:
            printf("Fork error");
            exit(EXIT_FAILURE);
            break;
        // Child Process : Display The Result to the User 2 ( From User 1 )    
        case 0:
            while (strncmp(buffer, "end chat", 8) != 0)
            // if the data from user 1 has been written
            {
                if(mm_area->written_1) {
                    // clear buffer
                    memset(buffer, 0, BUFSIZ);
                    int j = 0;
                    write(1, "User 1:\n", 8);
                    // copy the input from user 1 and write to the buffer
                    // in order to display to user 2 properly
                    for (int i = mm_area->pre_1; i < mm_area->cur_1; i++, j++)
                    {
                        buffer[j] = mm_area->data_1[i];
                    }
                    write(1, buffer, j);
                    // clear the written state of input user 1
                    mm_area->written_1 = 0;
                }
            }
            break;
        default:
            while (strncmp(buffer, "end chat", 8) != 0)
            {
                memset(buffer, 0, BUFSIZ);
                int nbytes = read(0, buffer, BUFSIZ);
                if(nbytes > 1) {
                    // set the state that user 1 has been written the input 
                    mm_area->written_2 = 1;
                    // given the value previous and current 
                    mm_area->pre_2 = mm_area->cur_2;
                    // the current was looped in order to loop in the write the output 
                    mm_area->cur_2 += nbytes;
                    // contain the data ( input ) to buffer
                    strcat(mm_area->data_2, buffer);
                }
            }
        }
    }

    // Parent Process Case : Send The SIGKILL to Child Process and waiting for child response ( terminated signal )
    if (child > 0)
    {
        kill(child, SIGKILL);
        wait(NULL);
    }
    // Child Process : Send the SIGUSR1 to the Parent Process 
    else if (child == 0)
    {
        kill(getppid(), SIGUSR1);
    }
    // Cancel the Mapping in order to Release the Resource
    munmap(mapped, FILE_LENGTH);
    exit(EXIT_SUCCESS);
}
