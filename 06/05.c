#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t tm;
    struct tm *tmp;
    char buf[64];

    time(&tm);
    
    tmp = localtime(&tm);

    strftime(buf, sizeof(buf), "%a %b %e %X %Z %Y", tmp);

    printf("%s\n", buf);

    return 0;
}
