#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PING {0x69}


int main()
{
    int _;
    char ping[] = PING;
    int p[2];
    char buf[1];
    pipe(p);
    if (fork() != 0) // I'm the parent
    {
        write(p[0], ping, 1);
        wait(&_);
        if (read(p[1], buf, 1))
        {
            printf("%d: received pong\n", getpid());
        }
        close(p[0]);
        close(p[1]);
        exit(0);
    }
    else             // I'm the child
    {
        if (read(p[1], buf, 1))
        {
            printf("%d: received ping\n", getpid());
            write(p[0], ping, 1);
        }
        close(p[0]);
        close(p[1]);
        exit(0);
    }
}