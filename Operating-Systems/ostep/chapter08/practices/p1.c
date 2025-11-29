#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid: %d)\n", (int) getpid());
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process (new process)
        printf("hello. I am child (pid: %d)\n", (int) getpid());
    } else {
        printf("hello. I am parent of %d (pid: %d)\n", pid, (int) getpid());
    }
    return 0;
}