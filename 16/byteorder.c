#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t val = 1;

    if ((((char *)&val)[0]) == 1) {
        printf("little endian\n");
    } else {
        printf("big endian\n");
    }

    return 0;
}
