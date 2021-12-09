#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

// https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
static void constructor() __attribute__((constructor));

// https://man7.org/linux/man-pages/man3/dlopen.3.html
void constructor()
{
    printf("Shared library loaded\n");
    char *filepath = "/home/privileged/0xBADC0DED";
    printf("Create file %s: ", filepath);
    if (open(filepath, O_CREAT | O_EXCL) == -1)
        printf("%m\n");
    else
        printf("Done\n");
}
