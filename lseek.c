#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    char block[BUFSIZ], c = '\0';
    int in, out, nread, mid;

    in = open("file.in", O_RDONLY);
    out = open("file.out", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    while ((nread = read(in, block, sizeof(block))) > 0) {
        write(out, block, nread);
    }

    int file_size = lseek(out, 0, SEEK_END);

    mid = file_size / 2;

    // Move the file offset to the middle
    lseek(out, mid - 8 , SEEK_SET);

    // Write 16 null characters
    for (int i = 0; i < 16; i++) {
        write(out, &c, 1);
    }

    // Close the file descriptors
    close(in);
    close(out);

    return 0;
}
