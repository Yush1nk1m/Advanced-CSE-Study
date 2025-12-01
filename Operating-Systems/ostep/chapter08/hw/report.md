# OSTEP Chapter 08 Homework

## Problem 1

    fork()를 호출하는 프로그램을 작성하라. fork()를 호출하기 전에 메인 프로세스는 변수에 접근하고 (예, x) 변수에 값을 지정하라 (예, 100). 자식 프로세스에서 그 변수의 값은 무엇인가? 부모와 자식이 변수 x를 변경한 후에 변수는 어떻게 변했는가?

**p1.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    // declare a variable
    int x = 100;

    // create a child process
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("x of child process(pid: %d): %d\n", (int) getpid(), x);
        x = 200;
        printf("value of x has reassigned from child process (x: %d)\n", x);
    } else {
        int waitPid = wait(NULL);
        printf("x of parent process(pid: %d): %d\n", (int) getpid(), x);
    }
    return 0;
}
```

**p1.c output**

```bash
$ ./a.out
x of child process(pid: 492124): 100
value of x has reassigned from child process (x: 200)
x of parent process(pid: 492123): 100
```

`fork()` 호출 시 생성되는 자식 프로세스는 말 그대로 새로운 프로세스이기 때문에 자신만의 고유한 가상 주소 공간(virtual address space)을 획득한다. 기존에 부모 프로세스의 스택 메모리에 저장되어 있던 변수 `x`도 새로운 주소 공간에 복사되므로 자식 프로세스의 주소 공간에 있는 변수 `x`의 값을 변경하는 것은 부모 프로세스의 주소 공간에 어떤 영향도 줄 수 없다.

## Problem 2

    open() 시스템 콜을 사용하여 파일을 여는 프로그램을 작성하고 새 프로세스를 생성하기 위하여 fork()를 호출하라. 자식과 부모가 open()에 의해 반환된 파일 디스크립터에 접근할 수 있는가? 부모와 자식 프로세스가 동시에 파일에 쓰기 작업을 할 수 있는가?

**p2.c**

```c
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
```

**p2.output**

```bash
$ ./a.out
$ cat p2.output
PPPPPPPPPPPPPPPPPPPPPPPCPCCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCPCCCCCCCCCCCCCCCCCCCCCC
$ grep -o 'C' p2.output | wc -l
100
$ grep -o 'P' p2.output | wc -l
100
```

자식 프로세스가 생성되면서 부모 프로세스가 생성한 파일 디스크립터가 함께 복사되는 동작을 확인할 수 있다. 결과를 보면 알 수 있듯이 두 프로세스가 공유하는 파일 디스크립터(`fd`)를 통해 동일한 파일에 동시에 쓰기(`write()`)를 수행할 수 있다.

부모 프로세스는 **P**, 자식 프로세스는 **C** 문자를 100번씩 출력하도록 하였는데 두 문자 모두 100개씩 파일에 출력되었음을 확인할 수 있다. 그러나 두 프로세스가 파일에 문자열을 기록하는 과정에서 경쟁 조건(Race condition)이 발생하기 때문에 문자열이 기록되는 순서는 비결정적(non-deterministic)이다.

운영체제의 커널은 가상 파일 시스템(VFS; Virtual File System) 또는 실제 파일 시스템 레벨에서 뮤텍스 락(Mutex lock), 스핀 락(Spinlock) 등의 내부적인 락 메커니즘으로 두 개 이상의 스레드가 한 순간 파일에 동시에 쓰기를 수행하는 행위를 차단한다. 그러므로 문자열의 기록 순서는 비결정적이어도 P, C가 언제나 각각 100개씩 기록된다는 사실은 결정적이라고 할 수 있다.

## Problem 3

    fork()를 사용하는 다른 프로그램을 작성하라. 자식 프로세스는 “hello”를 출력하고 부모 프로세스는 “goodbye”를 출력해야 한다. 항상 자식 프로세스가 먼저 출력하게 하라. 부모가 wait()를 호출하지 않고 할 수 있는가?

**p3.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("hello");
    } else {
        sleep(1);
        printf("goodbye");
    }
    return 0;
}
```

**p3.c output**

```bash
$ ./a.out
hellogoodbye
```

부모 프로세스가 `wait()` 시스템 콜 호출로 출력 순서를 제어할 수 없다면, `sleep()` 시스템 콜을 호출하도록 하여 문자열을 출력하기 전 강제로 I/O 대기 상태에 빠지게 만든다. 위 프로그램에서는 1초 동안 I/O 대기 상태에 빠지고 유휴 상태가 된 CPU는 자식 프로세스를 먼저 디스패치하여 실행한다.

실제 시스템에서는 `sleep()` 시스템 콜을 호출했다고 해도 언제나 자식 프로세스의 출력이 부모 프로세스의 출력보다 우선한다는 것이 보장되지는 않는다. 부모 프로세스가 I/O 대기 상태에 빠진 1초 동안 CPU는 자식 프로세스의 문자열 출력 작업보다 더 중요한 일을 해야 할 수도 있고, 1초 이후 자식 프로세스보다 부모 프로세스를 먼저 스케줄링하는 전략을 채택할 수도 있다.

그러므로 `wait()` 시스템 콜을 통하지 아니하고서는 부모 프로세스의 실행 순서와 자식 프로세스의 실행 순서를 결정적(deterministic)으로 제어하는 방법은 존재하지 않는다. 이것은 `wait()` 시스템 콜이 설계된 의도 그 자체이다.

## Problem 4

    fork()를 호출하고 /bin/ls를 실행하기 위하여 exec() 계열의 함수를 호출하는 프로그램을 작성하라. exec()의 변형 execl(), execle(), execlp(), execv(), execvp(), execve() 모두를 사용할 수 있는지 시도해 보라. 기본적으로는 동일한 기능을 수행하는 시스템 콜에 여러 변형이 있는 이유를 생각해 보라.

**p4.c**

```c
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
```

이 모든 함수들은 공통적으로 `execve()` 시스템 콜을 호출한다.

`execl()`, `execv()` 함수의 차이점은 호출 시 가변 인자를 나열(`l`, list)할 것인지 배열(`v`, vector)로 전달할 것인지에 있다. 그리고 이 함수들은 기본적으로 환경 변수를 고려하지 않는다.

그리고 `execlp()`, `execvp()` 함수는 다른 함수들과 달리 프로그램 실행 시 파일 검색을 위해 `PATH` 환경 변수에 정의된 경로를 참조한다. 함수 이름의 접미사 `p`가 의미하는 것이 바로 **path**이다.

마지막으로 `execle()`, `execve()` 함수는 사용자가 직접 환경 변수를 인자로 전달할 수 있다. 함수 이름의 접미사 `e`가 의미하는 것이 바로 **environment**이다.

그러므로 활용에 있어 유연성 자체는 `execve()` 함수가 가장 뛰어나고, `v` 계열의 함수가 `l` 계열의 함수보다는 인자를 배열의 형태로 전달한다는 점에서 확장성이 더 뛰어나다고 할 수 있다. 하지만 같은 `execve()` 시스템 콜을 호출하는 함수가 이렇게 다양하게 정의되어 있는 것은 사용자의 편의성과 프로그램의 유연성을 위한 것이다. 예를 들어 단순한 프로그램 호출을 위해 `execve()` 함수를 사용하고 인자에 `NULL` 값을 전달하는 것은 불필요한 코드의 증가를 유발한다.

|     함수     |   인자 형식   | 파일 검색 방식 |      환경 변수 처리      |
| :----------: | :-----------: | :------------: | :----------------------: |
| **execl()**  |  List (나열)  |  명시적 경로   |      현재 환경 상속      |
| **execv()**  | Vector (배열) |  명시적 경로   |      현재 환경 상속      |
| **execlp()** |  List (나열)  | **PATH 검색**  |      현재 환경 상속      |
| **execvp()** | Vector (배열) | **PATH 검색**  |      현재 환경 상속      |
| **execle()** |  List (나열)  |  명시적 경로   | **명시적 지정 (`envp`)** |
| **execve()** | Vector (배열) |  명시적 경로   | **명시적 지정 (`envp`)** |

## Problem 5

    wait()를 사용하여 자식 프로세스가 종료되기를 기다리는 프로그램을 작성하라. wait()가 반환하는 것은 무엇인가? 자식 프로세스가 wait()를 호출하면 어떤 결과가 발생하는가?

**p5.c**

```c
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
```

**p5.c output**

```bash
$ ./a.out
child has reaped(?) (pid: -1)
parent has reaped child (pid: 1404615)
```

`wait()` 시스템 콜에 대한 설명은 다음과 같다.

```
pid_t wait(int *__stat_loc)
Wait for a child to die. When one does, put its status in *STAT_LOC and return its process ID. For errors, return (pid_t) -1.

This function is a cancellation point and therefore not marked with
__THROW.
```

`wait()` 시스템 콜은 기본적으로 부모 프로세스가 회수 및 정리(Reaping)한 자식 프로세스의 프로세스 식별자를 반환한다. 만약 프로세스 그래프의 말단에 있는 자식 프로세스가 `wait()`을 호출한다면 해당 프로세스의 자식 프로세스는 존재하지 않기 때문에 운영체제는 이를 오류로 판단하고 `-1`을 반환한다.

다음은 [Linux](https://github.com/torvalds/linux/blob/master/include/linux/sched.h)에서 프로세스 테이블을 나타내는 `task_struct` 자료구조의 내용 일부이다.

**linux/include/linux/sched.h**

```c
struct task_struct {
    ...
	struct list_head		children;
	struct list_head		sibling;
	struct task_struct		*group_leader;
    ...
}
```

운영체제는 이처럼 프로세스 테이블에 자식 프로세스들의 정보를 연결 리스트 자료구조로 관리하고 있다. 그러므로 `wait()` 시스템 콜 호출 시 `children` 헤드 포인터가 `NULL`인지 확인함으로써 해당 프로세스가 프로세스 그래프에서 말단에 있는지를 즉시 파악할 수 있다.

## Problem 6

    위 문제에서 작성한 프로그램을 수정하여 wait() 대신에 waitpid()를 사용하라. 어떤 경우에 waitpid()를 사용하는 것이 좋은가?

**p6.c**

```c
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
```

`waitpid()`는 부모 프로세스가 여러 개의 자식 프로세스를 생성했을 때 이들 각각의 실행 순서나 결과에 의존하는 경우, 또는 좀비 프로세스가 생기는 것을 방지하기 위해 모든 자식 프로세스들을 안전하게 회수 및 정리해야 할 필요가 있을 경우에 사용될 수 있다.

## Problem 7

    자식 프로세스를 생성하고 자식 프로세스가 표준 출력 (STDOUT_FILENO)을 닫는 프로그램을 작성하라. 자식이 설명자를 닫은 후에 아무거나 출력하기 위하여 printf()를 호출하면 무슨 일이 생기는가?

**p7.c**

```c
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
```

자식 프로세스의 입장에서는 표준 출력이 닫혔다. 즉, `STDOUT_FILENO` 상수에 지정된 번호(`1`)의 파일 디스크립터는 사용할 수 없게 되었다. 때문에 `printf()` 함수를 호출 시 해당 번호의 파일 디스크립터에 `write()` 시스템 콜 호출을 시도하고 표준 출력(터미널)에는 아무것도 출력되지 못한다.

**practices/p4.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // child: standard output redirection to file
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("p4.c");
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
    } else {
        int waitPid = wait(NULL);
    }
    return 0;
}
```

이 장의 예제 중 표준 출력 파일 디스크립터를 닫고 새로운 파일을 여는 프로그램이 있다. 이 프로그램은 닫힌 1번 파일 디스크립터를 재사용하여 표준 출력을 향한 출력이 새로이 열린 파일로 리다이렉션되는 현상을 의도한 것이다.

`open()` 시스템 콜은 항상 사용 가능한 파일 디스크립터 번호 중 가장 작은 것을 할당하기 때문에 시스템에서 하나의 프로세스만 실행되고 있다(즉, 동시성 문제가 없다)고 가정한다면 위 프로그램의 동작은 일관될 것이다.

## Problem 8

    두 개의 자식 프로세스를 생성하고 pipe() 시스템 콜을 사용하여 한 자식의 표준 출력을 다른 자식의 입력으로 연결하는 프로그램을 작성하라.

**p8.c**

```c
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
```

**p8.c output**

```bash
$ ./a.out
parent (pid: 1424414) sending a message ["Message from the parent process"]
child (pid: 1424415) received a message ["Message from the parent process"]
parent process has reaped child (pid: 1424415)
```

처음에 표준 출력과 표준 입력을 닫고 `pipe()` 시스템 콜을 호출하여 파일 디스크립터를 재사용했다. 코드를 보면 알 수 있듯 시스템 콜 호출 결과가 저장된 `fd` 변수가 아닌 `STDIN_FILENO`, `STDOUT_FILENO` 파일 디스크립터를 통해 `read()`, `write()` 시스템 콜을 호출하고 있다.

두 프로세스 모두 표준 출력과 표준 입력이 닫힌 상태이기 때문에 표준 에러에서 동작을 검증하였다. 하지만 이 방식은 부모 프로세스와 자식 프로세스 모두 표준 입출력을 사용할 수 없다는 결점이 존재한다.

**p8_dup.c**

```c
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
```

`dup()` 시스템 콜을 활용하면 원래의 표준 출력에 대한 파일 디스크립터를 캐싱하여 표준 에러가 아닌 표준 출력에 출력하는 것이 가능하다. 하지만 중요한 점은 이때 사용하고 있는 `original_stdout` 파일 디스크립터는 1번 파일 디스크립터가 아니라는 것이다. 그러므로 `dprintf()` 함수를 통해 출력을 수행해야 한다.
