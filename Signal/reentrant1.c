#include <stdio.h>

int g_var = 1;

int f() { 
    g_var = g_var + 2;
    return g_var; 
}
int g() { 
    return f() + 2; 
}

int main() { 
    printf("F is : %d\n" , f());
    printf("g_var is %d\n", g());
    return 0; 
}