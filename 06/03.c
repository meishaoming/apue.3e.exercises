#include <stdio.h>
#include <sys/utsname.h>

int main(void)
{
    struct utsname un;
    uname(&un);
    printf("%s\n", un.sysname);
    printf("%s\n", un.nodename);
    printf("%s\n", un.release);
    printf("%s\n", un.version);
    printf("%s\n", un.machine);
    return 0;
}
