#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define LIMIT 35

int main()
{
    int _;
    int p[2];
    pipe(p);
    // printf("im main\n");
    if (fork())
    {
        close(p[0]);

        for (int i = 3; i < LIMIT; i++)
        {
            printf("sent i:%d\n", i);
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        // printf("done sending\n");
        wait(&_);
        // printf("main is gone\n");
        exit(0);
    }
    else
    {
        sleep(10);
        close(p[1]);
        // printf("Lets go sieve!\n");
        int i;
        while (read(p[0], &i, sizeof(int)))
        {
            if (i == 0)
            {
                printf("fuck\n");
                exit(0);
            }
            printf("i:%d\n", i);
        }
        close(p[0]);
    }
    exit(0);
}