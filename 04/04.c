
// 4.4 Run the program in Figure 4.9 after creating the files foo and bar. What happens?

// 如果 foo 和 bar 事先已存在，则 creat 不会改变它们的权限
// creat 相当于：
//
// open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode)
//
// 若文件已存在，不会有新建操作 

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

static void err_sys(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(void)
{
    umask(0);

    if (creat("foo", RWRWRW) < 0)
        err_sys("creat error for foo");

    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if (creat("bar", RWRWRW) < 0)
        err_sys("creat error for bar");

    exit(0);
}
