#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", getpid());

    char *msg = "hello from child1 process";
    char buf[strlen(msg)];

    int p[2];

    if (pipe(p) < 0)
    {
        // pipe failed
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    int rc1 = fork();
    int rc2 = fork();
    if (rc1 < 0 || rc2 < 0)
    {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc1 == 0)
    {
        // child: redirect standard output to a file.
        close(STDOUT_FILENO);

        write(p[1], msg, strlen(msg));
    }
    else if (rc2 == 0)
    {
        close(STDIN_FILENO);

        long n = read(p[0], buf, strlen(msg));
        printf("read in child2 process: %s\n", buf);
    }
    else
    {
        // parent process
        int status1, status2;
        int rc_wait1 = waitpid(rc1, &status1, 0);
        int rc_wait2 = waitpid(rc2, &status2, 0);
        printf("hello, I'm parent process of %d and %d (rc_wait1:%d, rc_wait2: %d) (pid:%d)\n", rc1, rc2, rc_wait1, rc_wait2, getpid());
    }
    return 0;
}