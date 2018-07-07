
// 4.6 Write a utility like cp(1) that copies a file containing holes, without writing the bytes of 0 to the output file.

// dd if=/dev/urandom seek=0 bs=4096 count=2 of=file-with-hole
// dd if=/dev/urandom seek=10 bs=4096 count=2 of=file-with-hole
// dd if=/dev/urandom seek=15 bs=4096 count=2 of=file-with-hole
// dd if=/dev/urandom seek=20 bs=4096 count=2 of=file-with-hole

// dd if=/dev/urandom seek=10 bs=4096 count=2 of=file-start-with-hole
// dd if=/dev/urandom seek=15 bs=4096 count=2 of=file-start-with-hole
// dd if=/dev/urandom seek=20 bs=4096 count=2 of=file-start-with-hole

// On MacOS, the default filesystem is HFS+ which does not support sparse files.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define min(x, y)   ((x) > (y) ? (y) : (x))

static ssize_t copy(int fd_src, int fd_dst, off_t offset, size_t size)
{
    char buf[4096];

    lseek(fd_src, offset, SEEK_SET);
    lseek(fd_dst, offset, SEEK_SET);

    size_t to_read = size;

    while (to_read) {
        ssize_t n = read(fd_src, buf, min(to_read, sizeof(buf)));
        if (n < 0) {
            return -1;
        }
        if (write(fd_dst, buf, n) != n) {
            return -1;
        }
        to_read -= n;
    }

    return size;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("%s <source> <destination>\n", argv[0]);
        exit(1);
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        perror("open failed");
        exit(1);
    }

    struct stat st;
    if (fstat(fd1, &st) < 0) {
        perror("stat failed");
        exit(1);
    }

    printf("blks %lld, blksize %d\n", st.st_blocks, st.st_blksize);
    size_t size = st.st_size;

    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (fd2 < 0) {
        perror("open failed");
        exit(1);
    }

    off_t curr = 0;

    while (size) {

        off_t start = lseek(fd1, curr, SEEK_DATA);

        if (start > curr) {
            curr = start;
        }

        off_t end = lseek(fd1, curr, SEEK_HOLE);

        printf("start %lld, end %lld\n", start, end);

        ssize_t res = copy(fd1, fd2, start, end - start);
        if (res < 0) {
            printf("copy failed.\n");
            return -1;
        }

        size -= res;
        curr += res;
    }

    printf("copy completed.\n");
    return 0;
}
