1. Write a program that calls `fork()`.
   Before calling `fork()`,have the `main` process access a variable (e.g., `x`) and set its value to something (e.g., `100`). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of `x`?

The child process inherits a copy of the value from the parent process.

Changing the value in either process is not reflected in the other process.

```sh
hello world (pid:41304)
hello, I'm parent process of 41306 (pid:41304). x=100
hello, I'm parent process of 41306 (pid:41304). x=50
hello, I'm child process (pid:41306). x=100
hello, I'm child process (pid:41306). x=10
```

2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

Yes, both the processes can access the same file descriptor.

If both processes write on the same file descriptor concurrently, then they should overwrite each other. Thus, there is a race condition to write bytes.

3. Write another program using `fork()`. The child process should print `“hello”`; the parent process should print `“goodbye”`. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

No, I cannot think of a way to implement this logic without calling `wait()`. If there is a way to do so, I'd be happy to learn.

4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

All of these sys calls are front-ends to the same underlying function `execve` on MacOS. Essentially, they are just different API to expose the same behaviour. Not sure the need. Need to read up about this.

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

The `wait()` system call returns the PID of the child process that is being waited on.

`wait()` returns -1 when used in the child process as there is nothing to wait on.

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

The `waitpid()` system call can be useful when we spawn more than 1 child processes.
With this system call, we can deterministically track each individual child process.

7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT FILENO`).
   What happens if the child calls `printf()` to print some output after closing the descriptor?

Nothing gets printed to standard output from the child as expected.
However, the parent process continues to write to STDOUT.

8. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the `pipe()` system call.
