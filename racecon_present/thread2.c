#include <stdio.h>
#include <pthread.h>

void *func1(void *param) {
    int bar = *(int *)param;
    
    printf("=== Func 1 start with Foo = %d ===\n",bar);

    for (int i = 0; i < 4; i++) {
        bar = bar + 5;
        printf("Plus with 5 !\n"); 
        printf("Foo in Func1 is %d\n", bar);
    }
    return bar;
    printf("==================\n");
}

void *func2(void *param) {
    int baz = *(int *)param;

    printf("=== Func 2 start with Foo = %d ===\n",baz);

    for (int i = 0; i < 4; i++) {
        baz = baz + 2;
        printf("Plus with 2 !\n");
        printf("Foo in Func2 is %d\n", baz);
    }
    return baz; 
    printf("==================\n");

}

int main(void) {
    pthread_t thread1, thread2;
    int bar, baz;
    int foo = 1;

    pthread_create(&thread1, NULL, &func1, &foo);
    pthread_create(&thread2, NULL, &func2, &foo);

    pthread_join(thread1, (void *)&bar);
    pthread_join(thread2, (void *)&baz);

    printf("==================\n");
    printf("Finally , Foo in F1 = %d and Foo in F2 = %d\n" , bar , baz); 

    return 0;
}
