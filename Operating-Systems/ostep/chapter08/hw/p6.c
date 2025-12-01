#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pid1, pid2, wid;
    
    pid1 = fork();
    if (pid1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid1 == 0) {
        printf("child process (pid: %d) has started and need to be reaped ...\n", (int) getpid());
    } else {
        pid2 = fork();
        if (pid2 < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (pid2 == 0) {
            printf("child process (pid: %d) has started and need to be reaped ...\n", (int) getpid());
        } else {
            wid = waitpid(pid2, NULL, 0);
            printf("parent process has reaped the second child (pid: %d)\n", wid);
            wid = waitpid(pid1, NULL, 0);
            printf("parent process has reaped the first child (pid: %d)\n", wid);
        }
    }
}