#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256
#define IN  0
#define OUT 1

int main(int argc, char* argv[]) {
    int fd[2];
    pid_t pid, wid;
    char buffer[BUFFER_SIZE];
    const char* message = "Message from the parent process";
    int bytes_read;
    int original_stdout;

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    original_stdout = dup(STDOUT_FILENO);
    if (original_stdout == -1) {
        perror("dup failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        dup2(fd[IN], STDIN_FILENO);

        close(fd[IN]);
        close(fd[OUT]);
        close(original_stdout);

        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("child (pid: %d) received a message [\"%s\"]\n", (int) getpid(), buffer);
        } else {
            perror("read failed");
        }
    } else {
        dup2(fd[OUT], STDOUT_FILENO);

        close(fd[IN]);
        close(fd[OUT]);

        dprintf(original_stdout, "parent (pid: %d) sending a message [\"%s\"]\n", (int) getpid(), message);
        write(STDOUT_FILENO, message, strlen(message));

        wid = wait(NULL);
        dprintf(original_stdout, "parent process has reaped child (pid: %d)\n", wid);
    }
    return 0;
}