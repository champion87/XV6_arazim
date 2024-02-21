#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define LIMIT 35

int main()
{
    printf("%d\n", !strcmp(".", "."));
    exit(0);
}