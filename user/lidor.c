#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define LIMIT 35

char str[10];

int main()
{
    free(LM_NULL);
    exit(0);
}