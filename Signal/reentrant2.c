#include <stdio.h> 

int f(int i) { 
    int priv = i; 
    priv = priv + 2; 
    return priv; 
}

int g(int i) { 
    int priv = i; 
    return f(priv) + 2; 
}

int main() { 
    //printf("G(1) is : %d\n", g(1));
    //printf("F(3) is : %d\n" , f(3));
    printf("F is : %d\n" , f(1));
    printf("g_var is : %d\n", g(1));
    return 0; 
}