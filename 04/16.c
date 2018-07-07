
// 4.16 Does the UNIX System have a fundamental limitation on the depth of a directory tree? To find out, write a program that creates a directory and then changes to that directory, in a loop. Make certain that the length of the absolute pathname of the leaf of this directory is greater than your system’s PATH_MAX limit. Can you call getcwd to fetch the directory’s pathname? How do the standard UNIX System tools deal with this long pathname? Can you archive the directory using either tar or cpio?

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

#define DIRNAME    "testdir"

int main(void)
{
    char buf[PATH_MAX];
    printf("%d\n", PATH_MAX);

    for (;;) {
        if (mkdir(DIRNAME, 0755) < 0) {
            printf("mkdir failed\n");
            break;
        }
        if (chdir(DIRNAME) < 0) {
            printf("chrdir failed\n");
            break;
        }
        char *p = getcwd(buf, sizeof(buf));
        if (!p) {
            printf("getcwd failed\n");
            break;
        }
        printf("getcwd %s\n", p);
    }

    return 0;
}
