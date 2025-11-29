#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid: %d)\n", (int) getpid());
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork faied\n");
        exit(1);
    } else if (pid == 0) {
        printf("hello. I am child (pid: %d)\n", (int) getpid());
    } else {
        int waitPid = wait(NULL);
        printf("hello. I am parent of %d (waitPid: %d) (pid: %d)\n", pid, waitPid, (int) getpid());
    }
    return 0;
}