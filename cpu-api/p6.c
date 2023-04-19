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
        int status;
        int rc_wait = waitpid(rc, &status, 0);
        printf("hello, I'm parent process of %d (rc_wait:%d) (pid:%d) status=%d\n", rc, rc_wait, getpid(), status);
    }
    return 0;
}