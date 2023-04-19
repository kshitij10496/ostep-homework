#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", getpid());

    int x = 100;
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
        printf("hello, I'm child process (pid:%d). x=%d\n", getpid(), x);
        x = 10;
        printf("hello, I'm child process (pid:%d). x=%d\n", getpid(), x);
    }
    else
    {
        // parent process
        printf("hello, I'm parent process of %d (pid:%d). x=%d\n", rc, getpid(), x);
        x = 50;
        printf("hello, I'm parent process of %d (pid:%d). x=%d\n", rc, getpid(), x);
    }
    return 0;
}