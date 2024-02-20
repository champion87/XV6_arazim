#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define LIMIT 35
#define W 1
#define R 0

int main_sieve(LM_FILE p[]);
int get_next_prime(LM_FILE source, int prime);
void send_sieve(LM_FILE source, LM_FILE dest, int prime);

void send_sieve(LM_FILE source, LM_FILE dest, int prime)
{
    int i;
    while (read(source, &i, sizeof(int)))
    {
        if (i % prime)
        {
            // printf("send_sieve I am %d and I sent %d\n", prime, i);
            write(dest, &i, sizeof(int));
        }
    }
}


int main()
{
    int _;
    int p[2];
    pipe(p);
    // printf("im main\n");
    if (fork())
    {
        close(p[R]);

        for (int i = 2; i < LIMIT; i++)
        {
            // printf("i:%d\n", i);
            write(p[W], &i, sizeof(int));
        }
        close(p[W]);
        // printf("done sending\n");
        wait(&_);
        // printf("main is gone\n");
        exit(0);
    }
    else
    {
        // printf("Lets go sieve!\n");
        main_sieve(p);
    }
    exit(0);
}

int get_next_prime(LM_FILE source, int prime)
{
    int i;
    int res;

    // sleep(10);
    // printf("I am %d read:%d\n", prime, read(source, &i, sizeof(int)));
    // printf("i:%d\n", i);


    while ((res = read(source, &i, sizeof(int))))
    {
        // printf("next_prime I am %d and I got res:%d\n", prime, res);
        if (i % prime)
        {
            printf("prime %d\n", i);
            return i;
        }
    }

    return 0;
}

int main_sieve(LM_FILE p[])
{
    int _;

    int current_pipe_read;
    close(p[W]);

    int next_pipe_read = p[R];
    int next_pipe_write;
    int tmp_pipe[2];

    int next_prime = 2;
    int current_prime;


    while (TRUE)
    { // assuming only next_pipe_read is open
        // sleep(10);
        // printf("-------------------------\n");
        // printf("loop. pid:%d next_prime:%d\n",getpid(), next_prime);
        current_pipe_read = next_pipe_read;
        
        

        // printf("pid: %d current pipe: %d\n", getpid(), current_pipe_read);

        current_prime = next_prime;
        next_prime = get_next_prime(current_pipe_read, current_prime);

    

        // printf("proccess of prime %d\n", current_prime);

        // sleep(100);

        if (next_prime == 0)
        {
            exit(0);
        }

        pipe(tmp_pipe);
        next_pipe_write = tmp_pipe[W];
        next_pipe_read = tmp_pipe[R];

        if (fork())
        {
            close(next_pipe_read);
            send_sieve(current_pipe_read, next_pipe_write, current_prime);
            close(next_pipe_write);
            close(current_pipe_read);
            wait(&_);
            exit(0);
        }
        close(current_pipe_read);
        close(next_pipe_write);
    }

    exit(0);
}