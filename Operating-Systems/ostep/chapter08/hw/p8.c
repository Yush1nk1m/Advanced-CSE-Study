#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
    int fd[2];
    pid_t pid, wid;
    char buffer[BUFFER_SIZE];
    const char* message = "Message from the parent process";
    int bytes_read;

    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(STDOUT_FILENO);

        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            fprintf(stderr, "child (pid: %d) received a message [\"%s\"]\n", (int) getpid(), buffer);
        } else {
            perror("read failed");
        }
    } else {
        close(STDIN_FILENO);

        fprintf(stderr, "parent (pid: %d) sending a message [\"%s\"]\n", (int) getpid(), message);
        write(STDOUT_FILENO, message, strlen(message));

        close(STDOUT_FILENO);

        wid = wait(NULL);
        fprintf(stderr, "parent process has reaped child (pid: %d)\n", wid);
    }
    return 0;
}