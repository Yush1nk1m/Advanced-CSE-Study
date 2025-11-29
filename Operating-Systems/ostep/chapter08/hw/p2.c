#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    int fd = open("./p2.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        char buf[1] = "C";
        for (int _ = 0; _ < 100; ++_)
            write(fd, buf, 1);
    } else {
        char buf[1] = "P";
        for (int _ = 0; _ < 100; ++_)
            write(fd, buf, 1);
        wait(NULL);
    }
    close(fd);
    return 0;
}