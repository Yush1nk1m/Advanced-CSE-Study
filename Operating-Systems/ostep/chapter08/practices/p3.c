#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid: %d)\n", (int) getpid());
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("hello. I am child (pid: %d)\n", (int) getpid());
        char *myargs[3];
        myargs[0] = strdup("wc");   // program: wc(word count)
        myargs[1] = strdup("p3.c"); // arguments: file
        myargs[2] = NULL;           // end of array
        execvp(myargs[0], myargs);  // execute wc
        printf("this shouldn't print out");
    } else {
        int waitPid = wait(NULL);
        printf("hello. I am parent of %d (waitPid: %d) (pid: %d)\n", pid, waitPid, (int) getpid());
    }
    return 0;
}