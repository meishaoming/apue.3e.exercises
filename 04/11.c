#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <errno.h>
#include <limits.h>

#ifdef	PATH_MAX
static long	pathmax = PATH_MAX;
#else
static long	pathmax = 0;
#endif

static long	posix_version = 0;
static long	xsi_version = 0;

/* If PATH_MAX is indeterminate, no guarantee this is adequate */
#define	PATH_MAX_GUESS	1024

char *
path_alloc(size_t *sizep) /* also return allocated size, if nonnull */
{
	char	*ptr;
	size_t	size;

	if (posix_version == 0)
		posix_version = sysconf(_SC_VERSION);

	if (xsi_version == 0)
		xsi_version = sysconf(_SC_XOPEN_VERSION);

	if (pathmax == 0) {		/* first time through */
		errno = 0;
		if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
			if (errno == 0)
				pathmax = PATH_MAX_GUESS;	/* it's indeterminate */
			else {
				printf("pathconf error for _PC_PATH_MAX");
                exit(1);
            }
		} else {
			pathmax++;		/* add one since it's relative to root */
		}
	}

	/*
	 * Before POSIX.1-2001, we aren't guaranteed that PATH_MAX includes
	 * the terminating null byte.  Same goes for XPG3.
	 */
	if ((posix_version < 200112L) && (xsi_version < 4))
		size = pathmax + 1;
	else
		size = pathmax;

	if ((ptr = malloc(size)) == NULL) {
		printf("malloc error for pathname");
        exit(1);
    }

	if (sizep != NULL)
		*sizep = size;
	return(ptr);
}

enum {
    FTW_F = 1,
    FTW_D,
    FTW_DNR,
    FTW_NS,
};

typedef int Myfunc(const char *pathname, const struct stat *st, int type);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

static int myfunc(const char *pathname, const struct stat *st, int type)
{
    switch (type) {
    case FTW_F:
        switch (st->st_mode & S_IFMT) {
        case S_IFREG:   nreg++;     break;
        case S_IFBLK:   nblk++;     break;
        case S_IFCHR:   nchr++;     break;
        case S_IFIFO:   nfifo++;    break;
        case S_IFLNK:   nslink++;   break;
        case S_IFSOCK:  nsock++;    break;
        case S_IFDIR:   printf("for S_IFDIR for %s", pathname); exit(1);
        }
        break;
    case FTW_D:
        break;
    case FTW_DNR:
        break;
    case FTW_NS:
        break;
    default:
        printf("unknown type %d for pathname %s", type, pathname);
        exit(1);
    }
    return 0;
}

static int dopath(const char *filepath, Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret;

    if (lstat(filepath, &statbuf) < 0) {
        return func(filepath, &statbuf, FTW_NS);
    }
    if (!S_ISDIR(statbuf.st_mode)) {
        return func(filepath, &statbuf, FTW_F);
    }

    if ((ret = func(filepath, &statbuf, FTW_D)) != 0) {
        return ret;
    }

    if (!(dp = opendir(filepath))) {
        return func(filepath, &statbuf, FTW_DNR);
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
            continue;
        }
        chdir(filepath);
        if ((ret = dopath(dirp->d_name, func)) != 0) {
            break;
        }
        chdir("..");
    }

    if (closedir(dp) < 0) {
        perror("closedir failed");
        exit(1);
    }

    return ret;
}

static int myftw(char *pathname, Myfunc *func)
{
    return dopath(pathname, func);
}

int main(int argc, char *argv[])
{
    int ret;

    if (argc != 2) {
        printf("usage: %s <starting-pathname>", argv[0]);
        exit(1);
    }

    ret = myftw(argv[1], myfunc);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0) {
        ntot = 1;
    }

    printf("regular files  = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
    printf("directories    = %7ld, %5.2f %%\n", ndir, ndir*100.0/ntot);
    printf("block special  = %7ld, %5.2f %%\n", nblk, nblk*100.0/ntot);
    printf("char special   = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot);
    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot);
    printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);

    exit(ret);
}

#if 0
➜  04 time ./11 ~
regular files  =  456981, 98.36 %
directories    =       0,  0.00 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       0,  0.00 %
symbolic links =    7595,  1.63 %
sockets        =       8,  0.00 %
./11 ~  1.26s user 15.45s system 59% cpu 28.133 total


➜  test time ./test ~
regular files  =  456982, 98.36 %
directories    =       0,  0.00 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       0,  0.00 %
symbolic links =    7595,  1.63 %
sockets        =       8,  0.00 %
./test ~  0.89s user 15.96s system 60% cpu 27.737 total
#endif
