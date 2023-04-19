#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", getpid());

    char buf_child[12] = "hello child";
    char buf_parent[13] = "hello parent";

    int fd = open("./test", O_CREAT | O_RDWR, S_IRWXU);
    if (fd < 0)
    {
        fprintf(stderr, "opening file failed\n");
        exit(1);
    }

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
        printf("hello, I'm child process (pid:%d). fd=%d\n", getpid(), fd);
        write(fd, buf_child, strlen(buf_child));
    }
    else
    {
        // parent process
        printf("hello, I'm parent process of %d (pid:%d). fd=%d\n", rc, getpid(), fd);
        write(fd, buf_parent, strlen(buf_parent));
    }
    close(fd);
    return 0;
}