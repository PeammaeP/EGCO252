#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    char block[BUFSIZ], c;
    FILE *in , *out; 

    in = fopen("file.in", "r");
    out = fopen("file.out", "w");

    while ((c = getc(in)) != EOF) {
        putc(c,out);
    }

    return 0;
}