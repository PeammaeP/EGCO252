#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 5133

typedef struct{
    int data;
    int index_prime ;
    int prime[MAX_SIZE];
}mythread;

void* prime_computing(void* arg) { 
    mythread* thread_prime = (mythread*)arg;
    int index = 1;

    while(index <= 50000) {
        int is_prime = 1; 

        for(int i=2 ; i < thread_prime->data ; i++) {
            // if the number is not_prime
            if(thread_prime->data%i == 0) {
                is_prime = 0; 
                break;
            }
        }
        // if the number was prime ( flag = 1 )
        if(is_prime) {
            thread_prime->prime[thread_prime->index_prime] = thread_prime->data;
            thread_prime->index_prime += 1;
        } 
        // increment the number and index to reach 50,000
        thread_prime->data += 1;
        index += 1;
    }
    return NULL;
}

int main() {
    // define the thread
    pthread_t single_thread;
    // create the 
    mythread *prime_thread = (mythread*)malloc(sizeof(mythread));

    prime_thread->data = 2;
    prime_thread->index_prime = 0; 

    pthread_create(&single_thread,NULL,prime_computing,prime_thread);
    pthread_join(single_thread,NULL);

    for(int i=0 ; i < prime_thread->index_prime ; i++) {
        printf("Index %d : The prime number is %d\n",i+1,prime_thread->prime[i]);
    }

    free(prime_thread);

    return 0;
}