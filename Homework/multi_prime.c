/* Group 1 Member : 
Chutiya Thanaluck 6513111
Anapat Kitsommart 6513123
Purin Pongpanich 6513135
Mahannop Thabua 6513136
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 5133

typedef struct{
    int data;
    int index_prime;
    int prime[MAX_SIZE];
}mythread;

void* prime_computing(void *arg) {
    int index = 1; 
    mythread *thread_prime = (mythread*)arg;
    // we need to make the each thread compute 10000 numbers
    // so that we are defined the index = 10000
    while(index <= 10000) {
        int is_prime = 1;

        for(int i = 2 ; i<thread_prime->data ; i++) {
            // if the data ( stored ) was not prime
            if(thread_prime->data%i == 0) { 
                is_prime = 0; 
                break; 
            }
        }
        if(is_prime) {
            thread_prime->prime[thread_prime->index_prime] = thread_prime->data;
            thread_prime->index_prime += 1;
        }
        thread_prime->data += 1;
        index += 1;
    }
    return NULL;
}

int main() {
    pthread_t main_thread[5]; 
    mythread *thread_prime = (mythread*)malloc(sizeof(mythread));

    thread_prime->data = 2; 
    thread_prime->index_prime = 0;

    for(int i=0 ; i<5 ; i++) {
        pthread_create(&main_thread[i] , NULL , prime_computing , thread_prime);
        pthread_join(main_thread[i] , NULL);
    }

    for(int i=0 ; i < thread_prime->index_prime ; i++) {
        printf("Index %d : The prime number is %d\n",i+1,thread_prime->prime[i]);
    }
    
    free(thread_prime);

    return 0;
}