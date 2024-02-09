// Thread With Race Condition
#include <stdio.h> 
#include <pthread.h> 

int foo; // global variable

void *func1() {
    printf("=== Func 1 start with Foo = %d ===\n",foo);

    for(int i = 0 ; i < 4 ; i++) {
        foo = foo + 5; 
        printf("Plus with 5 !\n"); 
        printf("Foo in Func1 is %d\n" , foo);
    }
    return foo; 
    printf("==================\n");

}

void *func2() {
    printf("=== Func 2 start with Foo = %d ===\n",foo);

    for(int i = 0 ; i < 4 ; i++) {
        foo = foo + 2;
        printf("Plus with 2 !\n");
        printf("Foo in Func2 is %d\n" , foo); 
    }
    return foo; 
    printf("==================\n");

}

int main(void) {
    pthread_t thread1 , thread2; 
    int bar , baz; 
    foo = 1; 

    pthread_create(&thread1 , NULL , &func1 , &foo);
    pthread_create(&thread2 , NULL , &func2 , &foo);

    pthread_join(thread1 , (void *)&bar);
    pthread_join(thread2 , (void *)&baz);

    printf("==================\n");
    printf("Finally , Foo in F1 = %d and Foo in F2 = %d\n" , bar , baz); 
 
    return 0; 
}