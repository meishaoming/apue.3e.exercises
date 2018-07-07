#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t curpos;
    char *s;
    char buf[5];

    if ((fd = open(argv[1], O_RDWR | O_TRUNC | O_APPEND | O_CREAT, 0644)) == -1) {
        perror("open failed");
        exit(-1);
    }

    s = "helloworld1234567890\n";
    write(fd, s, strlen(s));

    read(fd, buf, 5);
    printf("read 1: %.*s\n", 5, buf);

    curpos = lseek(fd, 0, SEEK_CUR);
    printf("curpos %lld\n", curpos);

    lseek(fd, 5, SEEK_SET);
    read(fd, buf, 5);
    printf("read 2: %.*s\n", 5, buf);

    curpos = lseek(fd, 0, SEEK_CUR);
    printf("curpos %lld\n", curpos);

    s = "second line\n";
    write(fd, s, strlen(s));

    close(fd);
    exit(0);
}
