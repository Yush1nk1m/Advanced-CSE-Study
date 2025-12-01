#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    int pid, ret;

    // execl()
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("child process (pid: %d) will execute execl()\n", (int) getpid());
        ret = execl(
            "/bin/ls",      // path to the executable file
            "ls",           // the name the executable program is invoked with
            (char*) NULL    // command-line arguments
        );
        if (ret == -1) {
            fprintf(stderr, "exec failed\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("reaping has completed\n");
    }

    // execle()

    // environment variables
    char* const env[] = {
        "HOME=/home/yushinkim",
        "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
        NULL
    };

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("child process (pid: %d) will execute execle()\n", (int) getpid());
        ret = execle(
            "/bin/ls",      // path to the executable file
            "ls",           // the name the executable program is invoked with
            "-l",           // command-line arguments
            (char*) NULL,
            env             // environment variables
        );
        if (ret == -1) {
            fprintf(stderr, "exec failed\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("reaping has completed\n");
    }

    // execlp()
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("child process (pid: %d) will execute execlp()\n", (int) getpid());
        ret = execlp(
            "/bin/ls",      // path to the executable file
            "ls",           // the name the executable program is invoked with
            "-l",           // command-line arguments
            (char*) NULL
        );
        if (ret == -1) {
            fprintf(stderr, "exec failed\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("reaping has completed\n");
    }

    // execv()
    
    // command-line arguments
    char* const new_argv[] = {"ls", "-l", NULL};
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("child process (pid: %d) will execute execv()\n", (int) getpid());
        ret = execv(
            "/bin/ls",      // path to the executable file
            new_argv        // command-line arguments
        );
        if (ret == -1) {
            fprintf(stderr, "exec failed\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("reaping has completed\n");
    }

    // execvp()
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("child process (pid: %d) will execute execvp()\n", (int) getpid());
        ret = execvp(
            "/bin/ls",      // path to the executable file
            new_argv        // command-line arguments
        );
        if (ret == -1) {
            fprintf(stderr, "exec failed\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("reaping has completed\n");
    }

    // execve()

    // environment variables
    char* const new_env[] = {
        "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
        NULL
    };
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("child process (pid: %d) will execute execve()\n", (int) getpid());
        ret = execve(
            "/bin/ls",      // path to the executable file
            new_argv,       // command-line arguments
            new_env
        );
        if (ret == -1) {
            fprintf(stderr, "exec failed\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("reaping has completed\n");
    }

    return 0;
}