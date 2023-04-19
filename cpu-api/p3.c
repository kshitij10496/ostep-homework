#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", getpid());

    int rc = fork();
    if (rc < 0)
    {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child (new process)
        printf("hello, I'm child process (pid:%d)\n", getpid());
    }
    else
    {
        // parent process
        printf("goodbye, I'm parent process of %d (pid:%d)\n", rc, getpid());
    }
    return 0;
}