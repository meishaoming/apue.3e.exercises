
// 4.1 Modify the program in Figure 4.3 to use stat instead of lstat.
// What changes if one of the command-line arguments is a symbolic link?

// stat 会 follow 符号链接，即使是多级符号链接，也会最终找到实体目标
// lstat 的操作对象是符号链接本身

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat buf;
    char *ptr;

    for (int i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (stat(argv[i], &buf) < 0) {
            perror("stat error");
            continue;
        }

        if (S_ISREG(buf.st_mode)) {
            ptr = "regular";
        }
        else if (S_ISDIR(buf.st_mode)) {
            ptr = "director";
        }
        else if (S_ISLNK(buf.st_mode)) {
            ptr = "symbolic link";
        }
        else {
            ptr = "** unknown mode **";
        }

        printf("%s\n", ptr);
    }

    exit(0);
}

