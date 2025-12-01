#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        close(STDOUT_FILENO);
        printf("hello world");
    } else {
        wait(NULL);
    }
    return 0;
}