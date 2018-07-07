#if 0
4.17 In Section 3.16, we described the /dev/fd feature. For any user to be able to access these files, their permissions must be rw-rw-rw-. Some programs that create an output file delete the file first, in case it already exists, ignoring the return code:
        unlink(path);
        if ((fd = creat(path, FILE_MODE)) < 0)
err_sys(...);
What happens if path is /dev/fd/1?


➜  04 ./17 /dev/fd/1
unlink failed: Operation not permitted
#endif

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (unlink(argv[1]) < 0) {
        perror("unlink failed");
        return -1;
    }
    printf("unlink `%s` success\n", argv[1]);
    return 0;
}
