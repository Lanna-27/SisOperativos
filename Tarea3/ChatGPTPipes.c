/*
This program creates a pipe using the pipe() function, which returns two file descriptors 
representing the read and write ends of the pipe. It then forks, creating a child process
that inherits a copy of the pipe file descriptors.

In the child process, the write end of the pipe is closed (p[1]), and the process reads 
from the read end of the pipe (p[0]) into a buffer (buf) using the read() function. 
It then writes a message (child_msg) back to the parent process through the pipe using the
write() function, and finally closes the read end of the pipe.

In the parent process, the read end of the pipe is closed (p[0]), and the process writes 
a message (parent_msg) to the child process through the pipe using the write() function. 
It then reads from the pipe into the buffer (buf) using the read() function to receive the
response from the child process. Finally, it closes the write end of the pipe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSIZE 16

int main()
{
    char parent_msg[MSGSIZE] = "Hello, child!";
    char child_msg[MSGSIZE] = "Hi, parent!";
    char buf[MSGSIZE];
    int p[2];
    pid_t pid;

    if (pipe(p) < 0) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(p[1]); // Close write end of pipe
        read(p[0], buf, MSGSIZE);
        printf("Child received: %s\n", buf);
        write(p[0], child_msg, MSGSIZE);
        close(p[0]); // Close read end of pipe
    } else {
        // Parent process
        close(p[0]); // Close read end of pipe
        write(p[1], parent_msg, MSGSIZE);
        read(p[1], buf, MSGSIZE);
        printf("Parent received: %s\n", buf);
        close(p[1]); // Close write end of pipe
    }

    return 0;
}
