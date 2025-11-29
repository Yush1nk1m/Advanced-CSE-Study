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
