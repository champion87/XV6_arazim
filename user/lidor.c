
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fd1 = dup(1);
    int fd2 = dup(1);
    int fd3 = dup(fd1);

    printf("%d\n", fd1);
    printf("%d\n", fd2);
    printf("%d\n", fd3);

    return 0;
}