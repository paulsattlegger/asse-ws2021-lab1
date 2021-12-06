#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
__attribute__((constructor)) static void constructor();

// https://man7.org/linux/man-pages/man3/dlopen.3.html
void constructor()
{
    printf("PAWNED\n");
    char *filepath = "/home/privileged/0xBADC0DED";
    if (open(filepath, O_CREAT | O_EXCL) == -1)
    {
        if (errno != EEXIST)
        {
            printf("%m\n");
            exit(errno);
        }
    }
}