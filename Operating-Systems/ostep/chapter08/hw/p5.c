#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int waitPid;

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        waitPid = wait(NULL);
        printf("child has reaped(?) (pid: %d)\n", waitPid);
    } else {
        waitPid = wait(NULL);
        printf("parent has reaped child (pid: %d)\n", waitPid);
    }
    return 0;
}