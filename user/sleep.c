#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // sys_sleep();
    if (argc != 2 || !is_number(argv[1]))
    {
        printf("Usage: sleep <ticks>\n");
        return 0;
    }

    int ticks = atoi(argv[1]);

    sleep(ticks);

    return 0;
}