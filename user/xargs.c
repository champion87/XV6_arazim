#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define MAX_LEN_OF_ARG 512

void destroy_argv(char **dest, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (dest[i] != LM_NULL)
        {
            free(dest[i]);
        }
    }
}

void copy_ptrs(char *dest[], char *src[], int len)
{
    for (int i = 0; i < len; i++)
    {
        dest[i] = src[i];
    }
}

void print_arr(char *dest[], int len)
{
    for (int i = 0; i < len ; i++)
    {
        printf("%s\n", dest[i]);
    }
}

void read_lines_of_args(char **dest, int max_num_of_args, int arg_len /*includes null terminator*/)
{
    int arg_num = 0;
    int i = 0;
    char c;
    while (1 == read(LM_STDIN, &c, 1))
    {
        if (c == ' ' || c == '\n') 
        {
            dest[arg_num][i] = '\0';
            arg_num++;
            i = 0;
            if (arg_num >= max_num_of_args)
            {
                while (1 == read(LM_STDIN, &c, 1) && c!='\n'); // flush stdin
                break;
            }
        }
        else
        {
            if (i < MAX_LEN_OF_ARG - 1)
            {
                dest[arg_num][i] = c;
                i++;
            }
        }

    }
    dest[arg_num][i] = '\0';
    arg_num++;

    free(dest[arg_num]);
    dest[arg_num] = LM_NULL;
}


char total_arguments[MAXARG + 1][MAX_LEN_OF_ARG]; // +1 is for the null terminator


int allocate_argv(char **dest, int len)
{
    
    for (int i = 0; i < MAXARG + 1; i++)
    {
        dest[i] = (char *) malloc(sizeof(char) * MAX_LEN_OF_ARG);
        if (!dest[i])
        {
            destroy_argv(dest, i);
            return 1;
        }
    }

    return 0;
}



int main(int argc, char *argv[])
{
    char *total_arguments[MAXARG + 1];

    allocate_argv(total_arguments, MAXARG + 1);

    copy_ptrs(total_arguments, argv + 1, argc - 1);

    read_lines_of_args(total_arguments + argc - 1, MAXARG - (argc - 1), MAX_LEN_OF_ARG);
    

    int _;
    if (fork())
    {
        wait(&_);
        destroy_argv(total_arguments, MAXARG + 1);
    }
    else
    {
        exec((total_arguments)[0], total_arguments);
    }

    exit(0);
}