# OSTEP Chapter 17 Homework

## Problem 1

    null.c라는 간단한 프로그램을 작성하라. 이 프로그램은 정수를 가리키는 포인터를 만들고 NULL로 초기화하고 그것을 역참조하려고 한다. null이라는 실행 파일이 생성되도록 컴파일하라. 프로그램을 실행시키면 어떤 일이 발생하는가?

**null.c**

```c
#include <stdio.h>

int main(int argc, char* argv[]) {
    int* x = NULL;

    printf("%d\n", *x);

    return 0;
}
```

**null.c output**

```c
$ ./a.out
세그멘테이션 오류 (코어 덤프됨)
```

`NULL`은 값 0을 나타내는 매크로일 뿐이다. 그러므로 위 코드는 주소 0에 대해 역참조를 수행하는 것인데 이는 유효하지 않은 메모리 주소에 대한 역참조이기 때문에 segmentation fault가 발생한다.

그리고 보통 주소 0과 같은 영역에는 커널 코드가 위치하여 사용자 프로그램이 직접 접근할 수 없도록 권한이 엄격하게 제한되어 있다.

## Problem 2

    다음에 이 프로그램을 심볼 정보가 포함되도록 컴파일하라 (-g 플래그 사용). 실행 파일에 더 많은 정보를 추가하여 디버거가 변수 이름 등의 유용한 정보를 접근할 수 있게 한다. 디버거에서 프로그램을 실행하라 (gdb null). gdb가 실행되면, run 명령어를 입력하라. gdb는 무엇을 보여주는가?

**bash**

```bash
$ gcc -g null.c
$ gdb ./a.out
(gdb) run
Starting program: /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Program received signal SIGSEGV, Segmentation fault.
0x0000555555555168 in main (argc=1, argv=0x7fffffffdab8) at p1.c:6
6	    printf("%d\n", *x);
```

gdb 디버거는 정확히 몇 번 라인의 코드 실행 중 SIGSEGV 시그널이 발생했는지 보여주고 있다.

## Problem 3

    마지막으로, 이 프로그램에 valgrind 도구를 사용하라. 우리는 valgrind의 일부인 memcheck 도구를 사용하여 어떤 일이 일어나는지 분석할 것이다. 다음과 같은 명령어를 실행시켜라 : valgrind --leak-check=yes null. 이를 실행하면 어떻게 되는가? 출력을 해석할 수 있는가?

**bash**

```bash
$ valgrind --leak-check=yes ./a.out
==276544== Memcheck, a memory error detector
==276544== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==276544== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==276544== Command: ./a.out
==276544==
==276544== Invalid read of size 4
==276544==    at 0x109168: main (p1.c:6)
==276544==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==276544==
==276544==
==276544== Process terminating with default action of signal 11 (SIGSEGV)
==276544==  Access not within mapped region at address 0x0
==276544==    at 0x109168: main (p1.c:6)
==276544==  If you believe this happened as a result of a stack
==276544==  overflow in your program's main thread (unlikely but
==276544==  possible), you can try to increase the size of the
==276544==  main thread stack using the --main-stacksize= flag.
==276544==  The main thread stack size used in this run was 8388608.
==276544==
==276544== HEAP SUMMARY:
==276544==     in use at exit: 0 bytes in 0 blocks
==276544==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==276544==
==276544== All heap blocks were freed -- no leaks are possible
==276544==
==276544== For lists of detected and suppressed errors, rerun with: -s
==276544== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
세그멘테이션 오류 (코어 덤프됨)
```

valgrind는 gdb에 비해 설명이 더 상세하다. 로그를 통해 다음과 같은 내용을 확인할 수 있다.

- 유효하지 않은 4 바이트(`int`) 읽기가 시도됨
- 주소 공간의 0x0 번째 주소는 스택, 힙에도 할당되지 않았고 심지어 최근 해제된 공간도 아님
- SIGSEGV 시그널과 함께 프로그램이 종료됨

## Problem 4

    malloc()을 사용하여 메모리를 할당하지만 종료 전에 해제하는 것을 잊어버리는 간단한 프로그램을 작성하라. 이 프로그램을 실행시키면 무슨 일이 일어나는가? 이 프로그램의 문제를 gdb를 사용하여 발견할 수 있는가? valgrind는 어떤가 (다시 --leak-check=yes 플래그를 주고 실행)?

**no_free.c**

```c
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int* x = (int*) malloc(sizeof(int));
    *x = 10;

    return 0;
}
```

**no_free output**

```bash
$ valgrind --leak-check=yes ./a.out
==644247== Memcheck, a memory error detector
==644247== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==644247== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==644247== Command: ./a.out
==644247==
==644247==
==644247== HEAP SUMMARY:
==644247==     in use at exit: 4 bytes in 1 blocks
==644247==   total heap usage: 1 allocs, 0 frees, 4 bytes allocated
==644247==
==644247== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
==644247==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==644247==    by 0x109165: main (in /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out)
==644247==
==644247== LEAK SUMMARY:
==644247==    definitely lost: 4 bytes in 1 blocks
==644247==    indirectly lost: 0 bytes in 0 blocks
==644247==      possibly lost: 0 bytes in 0 blocks
==644247==    still reachable: 0 bytes in 0 blocks
==644247==         suppressed: 0 bytes in 0 blocks
==644247==
==644247== For lists of detected and suppressed errors, rerun with: -s
==644247== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

서버 프로그램과 같이 무중단 프로그램이 아니라면 main 함수가 반환되면서 가상 주소 공간의 힙도 정리됨에 따라 `free()` 함수를 호출하지 않는 것이 크게 문제가 되지는 않는다.

하지만 valgrind의 `--leak-check` 옵션을 활성화하면 이러한 상황에서 발생하는 메모리 누수도 감지하고 알려주게 된다. 로그에는 4 바이트 크기의 블록 1개가 완전하게 소실되었다고 출력되었다.

## Problem 5

    크기가 100인 data라는 이름의 정수 배열을 만드는 프로그램을 작성하라. 그런 후 data[100]을 0으로 클리어 하라. 이 프로그램을 실행하면 어떤 일이 벌어지는가? valgrind를 사용하여 프로그램을 실행하면 어떤 일이 일어나는가? 프로그램은 정확한가?

**array.c**

```c
#include <stdio.h>

int main(int argc, char* argv[]) {
    int data[100];
    data[100] = 0;

    return 0;
}
```

**array.c output**

```bash
$ ./a.out
$ valgrind --leak-check=yes ./a.out
==672308== Memcheck, a memory error detector
==672308== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==672308== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==672308== Command: ./a.out
==672308==
==672308==
==672308== HEAP SUMMARY:
==672308==     in use at exit: 0 bytes in 0 blocks
==672308==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==672308==
==672308== All heap blocks were freed -- no leaks are possible
==672308==
==672308== For lists of detected and suppressed errors, rerun with: -s
==672308== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

배열의 크기가 100이므로 유효한 인덱스는 99까지이지만 `data[100] = 0`과 같이 100번째 인덱스를 참조해도 아무런 문제가 발생하지 않는다. 이것은 버퍼 오버플로우 공격과도 연관된 문제로 스택의 경계를 넘는 것이기 때문에 힙 경계를 모니터링하는 valgrind와 같은 툴로는 추적되지 않는다.

**array.c**

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int data[100];
    data[100] = 0;

    int* data2 = (int*) malloc(sizeof(int) * 100);
    data2[100] = 0;
    free(data2);

    return 0;
}
```

**array.c output**

```bash
$ valgrind --leak-check=yes ./a.out
==707513== Memcheck, a memory error detector
==707513== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==707513== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==707513== Command: ./a.out
==707513==
==707513== Invalid write of size 4
==707513==    at 0x1091D9: main (in /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out)
==707513==  Address 0x4a8b1d0 is 0 bytes after a block of size 400 alloc'd
==707513==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==707513==    by 0x1091C4: main (in /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out)
==707513==
==707513==
==707513== HEAP SUMMARY:
==707513==     in use at exit: 0 bytes in 0 blocks
==707513==   total heap usage: 1 allocs, 1 frees, 400 bytes allocated
==707513==
==707513== All heap blocks were freed -- no leaks are possible
==707513==
==707513== For lists of detected and suppressed errors, rerun with: -s
==707513== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

크기 100의 `int`형 배열과 똑같이 다룰 수 있는 `data2` 변수를 위한 공간을 `malloc()` 함수로 요청했다. 그리고 valgrind를 활용해 실행하면 유효하지 않는 메모리 참조를 인식한다. 그러므로 힙 공간에서의 버퍼 오버플로우 문제는 valgrind를 사용해 탐지할 수 있다.

C언어보다 고수준의 언어로 취급되는 Java와 같은 언어들은 정적으로 할당해둔 배열의 크기를 초과하는 인덱스 참조에 대한 예외가 런타임 중에 발생하지만 C는 그렇지 않다. 그래서 사용자는 프로그램을 작성할 때 각별한 주의를 기울여야 한다.

## Problem 6

    정수 배열을 할당하고 (위와 동일), 다시 해제하고, 배열의 원소 하나의 값을 출력하려고 시도하는 프로그램을 작성하라. 프로그램이 실행되는가? valgrind를 사용하여 실행하면 어떤 일이 발생하는가?

**free_and_reference.c**

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int* data = (int*) malloc(sizeof(int) * 100);
    free(data);
    printf("%d\n", data[0]);

    return 0;
}
```

**free_and_reference.c output**

```bash
$ ./a.out
-1180477781
$ valgrind --leak-check=yes ./a.out
==729981== Memcheck, a memory error detector
==729981== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==729981== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==729981== Command: ./a.out
==729981==
==729981== Invalid read of size 4
==729981==    at 0x1091BA: main (in /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out)
==729981==  Address 0x4a8b040 is 0 bytes inside a block of size 400 free'd
==729981==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==729981==    by 0x1091B5: main (in /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out)
==729981==  Block was alloc'd at
==729981==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==729981==    by 0x1091A5: main (in /home/yushinkim/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter17/hw/a.out)
==729981==
0
==729981==
==729981== HEAP SUMMARY:
==729981==     in use at exit: 0 bytes in 0 blocks
==729981==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
==729981==
==729981== All heap blocks were freed -- no leaks are possible
==729981==
==729981== For lists of detected and suppressed errors, rerun with: -s
==729981== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

valgrind를 사용하지 않으면 쓰레기 값이 출력되고, valgrind 사용 시에는 유효하지 않은 읽기가 발생했다는 로그가 출력된다. 이처럼 C로 작성된 프로그램은 해제된 메모리 공간도 참조할 수 있다. 이러한 C 프로그램 특유의 자율성이 때로는 큰 문제로 연결되기도 하기 때문에 주의해야 한다.

## Problem 7

    free에 위에서 할당한 배열의 중간을 가리키는 포인터와 같은 황당한 값을 전달하라. 어떤 일이 벌어지는가? 이런 종류의 문제를 발견하기 위해 도구가 필요한가?

**invalid_free.c**

```c
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int* x = (int*) malloc(sizeof(int) * 10);
    x += 5;
    free(x);

    return 0;
}
```

**invalid_free.c output**

```bash
$ gcc invalid_free.c
invalid_free.c: In function ‘main’:
invalid_free.c:6:5: warning: ‘free’ called on pointer ‘x’ with nonzero offset 20 [-Wfree-nonheap-object]
    6 |     free(x);
      |     ^~~~~~~
invalid_free.c:4:21: note: returned from ‘malloc’
    4 |     int* x = (int*) malloc(sizeof(int) * 10);
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~
$ ./a.out
free(): invalid pointer
중지됨 (코어 덤프됨)
```

유효하지 않은 포인터를 `free()` 함수에 인자로 전달하는 것은 컴파일 시점에도 경고가 발생하고, 실제 실행 시에도 정확히 문제를 진단한다.

실제 힙 메모리 요청 시 정확히 요청한 공간만큼만 반환되는 것이 아니라 메모리 블록에 메타데이터를 저장하는 헤더(Header)와 푸터(Footer)가 포함되어 반환된다(푸터는 선택적). 메타데이터는 다음과 같이 구성된다.

- **블록 크기(Size)**: 헤더(및 푸터)를 포함한 현재 블록의 총 크기
- **할당 상태(A/M bit)**: 현재 블록이 할당 중인지 해제되었는지 표시하는 플래그
- **이웃 블록 포인터(Free List Pointers)**: 블록이 현재 사용 가능한 상태일 경우 이웃한 블록을 가리키는 포인터(이중 연결 리스트)

이러한 메타데이터를 통해 힙 메모리를 유효하지 않게 관리할 경우 런타임에 이를 감지할 수 있다. 그러므로 이 문제를 해결하기 위해 별도의 도구가 필요하지는 않다.

## Problem 8

    메모리를 할당할 때 다른 인터페이스를 사용해 보라. 예를 들어, 간단한 벡터-비슷한 자료 구조를 만들고 벡터를 관리하기 위하여 realloc()을 사용하는 루틴을 만들어라. 벡터 원소를 저장하기 위하여 배열을 사용하라. 사용자가 벡터에 항목을 추가할 때, realloc()을 사용하여 필요 공간을 확보하라. 이 벡터 프로그램은 얼마나 잘 동작하는가? 연결 리스트와 비교하면 어떠한가? 버그를 발견하기 위하여 valgrind를 사용하라.

**vector.c**

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size, i;
    int* array;
} Vector;

Vector* createVector(int size) {
    Vector* vector = (Vector*) malloc(sizeof(Vector));
    vector->size = size;
    vector->i = 0;
    vector->array = (int*) malloc(sizeof(int) * size);
}

void addElement(Vector* vector, int element) {
    if (vector->i >= vector->size) {
        vector->size <<= 1;
        vector->array = realloc(vector->array, vector->size * sizeof(int));
    }
    vector->array[vector->i++] = element;
}

void printElements(Vector* vector) {
    printf("vector: [ ");
    for (int j = 0; j < vector->i; ++j)
        printf("%d ", vector->array[j]);
    printf("]\n");
}

void freeVector(Vector* vector) {
    free(vector->array);
    free(vector);
}

int main(int argc, char* argv[]) {
    Vector* vector = createVector(1);
    for (int i = 0; i < 10; ++i)
        addElement(vector, i);
    printElements(vector);
    freeVector(vector);

    return 0;
}
```

**vector.c output**

```c
$ valgrind --leak-check=yes ./a.out
==855904== Memcheck, a memory error detector
==855904== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==855904== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==855904== Command: ./a.out
==855904==
vector: [ 0 1 2 3 4 5 6 7 8 9 ]
==855904==
==855904== HEAP SUMMARY:
==855904==     in use at exit: 0 bytes in 0 blocks
==855904==   total heap usage: 7 allocs, 7 frees, 1,164 bytes allocated
==855904==
==855904== All heap blocks were freed -- no leaks are possible
==855904==
==855904== For lists of detected and suppressed errors, rerun with: -s
==855904== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

구조체를 활용하여 간단한 벡터 자료 구조를 구현했다. valgrind 사용 시에도 힙 관련 오류가 발생하지 않는다. 중요한 점은 구조체 포인터 해제 전 구조체가 갖고 있는 포인터 타입의 멤버를 해제해 주어야 한다는 점이다.

이 벡터 프로그램은 단순하게 벡터에 원소를 추가하는 연산만 구현했지만, 일반적인 벡터에 적용할 수 있는 다른 연산들을 고려할 때 연결 리스트와 비교하여 배열에 랜덤 액세스가 가능하다는 장점이 있으나 초기 설정한 용량을 넘어가서 `realloc()` 사용 시 매번 $O(N)$ 정도의 시간 복잡도를 갖는 비효율성이 야기된다는 단점이 있다.
