/*
 * > In Section 4.12, we said that a file size of 0 is valid for a regular file.
 * > We also said that the st_size field is defined for directories and symbolic links.
 * > Should we ever see a file size of 0 for a directory or a symbolic link?
 * 
 * 目录和符号链接的 st_size 不可能为 0，因为：
 * 
 *   一个目录里至少会包含一个目录里至少会包含 . 和 ..  。创建一个空目录，并用本程序打印其 st_size 值即可看到
 *   一个符号链接至少有一个链接目标，否则符号链接创建不成功。`ln -s` 无法创建一个这样的「空链接」
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    struct stat statbuf;

    if (lstat(argv[1], &statbuf) < 0) {
        printf("stat failed");
        return -1;
    }

    printf("size %lld\n", statbuf.st_size);
    return 0;
}
