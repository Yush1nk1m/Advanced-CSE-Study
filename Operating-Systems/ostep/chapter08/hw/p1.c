#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    // declare a variable
    int x = 100;

    // create a child process
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("x of child process(pid: %d): %d\n", (int) getpid(), x);
        x = 200;
        printf("value of x has reassigned from child process (x: %d)\n", x);
    } else {
        int waitPid = wait(NULL);
        printf("x of parent process(pid: %d): %d\n", (int) getpid(), x);
    }
    return 0;
}