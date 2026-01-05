# OSTEP Chapter 29 Homework

## Problem 1

    우선 “loop.s”라는 간단한 프로그램을 먼저 살펴보자. cat loop.s 명령어로 프로그램을 먼저 읽어보고 이해할 수 있는지 보자. 그리고 다음의 인자 값을 사용하여 실행해 보자.
    ./x86.py −p loop.s −t 1 −i 100 −R dx
    위 명령어의 옵션은 한 개의 쓰레드, 100개의 명령어를 실행할 때마다 인터럽트 발생, %dx의 값을 추적하라는 옵션이다. 실행 중에 %dx의 값이 무엇일지 알 수 있겠는가? 그 값을 예상했다면, -c 플래그와 함께 다시 실행하여 답을 확인해 보자. 오른편의 명령어가 실행되면 왼편에 레지스터의 값 (또는 메모리 값)이 나타난다.

**loop.s**

```
$ cat loop.s
.main
.top
sub  $1,%dx
test $0,%dx
jgte .top
halt
```

위 어셈블리 코드는 특정 변수의 값이 0 이상일 경우 1씩 빼는 동작을 나타내고 있다. C 방식의 의사코드로는 다음과 같이 나타낼 수 있을 것이다.

**loop.s pseudocode**

```c
// .main
do {    // .top
    --i;
} while (i >= 0);

exit(0);
```

주어진 옵션으로 시뮬레이터를 실행한 결과는 다음과 같다.

**x86.py output**

```bash
$ python x86.py -p loop.s -t 1 -i 100 -R dx -c
ARG seed 0
ARG numthreads 1
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0
    0
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
```

쓰레드가 하나뿐이기 때문에 의도한 대로 %dx 레지스터는 최초의 값에서 실행된 `sub` 명령의 횟수만큼 감소한다.

## Problem 2

    이제 다음과 같은 플래그를 사용하여 똑같은 코드를 실행해 보자.
    ./x86.py −p loop.s −t 2 −i 100 −a dx=3,dx=3 −R dx
    이 명령어는 두 개의 쓰레드를 지정하고 각각 %dx 레지스터의 값을 3으로 만든다. 어떤 값이 %dx에 저장되겠는가? -c 플래그와 함께 실행하여 답을 확인해 보자. 쓰레드가 여러 개라는 사실이 당신의 추측에 영향을 주는가? 이 코드에 경쟁 조건이 있는가?

**x86.py output**

```bash
$ python x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx -c
ARG seed 0
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1                            1002 jgte .top
   -1                            1003 halt
```

두 쓰레드가 각자의 레지스터를 갖는다는 점이 가장 중요하다. 즉, 공유 자원으로부터 가져온 값이 레지스터에 저장될 수는 있겠으나 레지스터의 값 자체는 공유되는 자원이 아니다. 그러므로 인터럽트의 발생 주기가 얼마이든 간에 두 쓰레드는 결정론적(deterministic)인 동작을 수행하며 경쟁 조건은 존재하지 않는다.

## Problem 3

    다음의 조건으로 실행해 보자.
    ./x86.py −p loop.s −t 2 −i 3 −r −a dx=3,dx=3 −R dx
    위의 명령어는 인터럽트가 짧은 간격으로 아무 때나 발생하도록 지정한다. 다른 순서의 쓰레드 실행을 보고 싶다면 -s 옵션으로 새로운 시드를 사용하라. 인터럽트 실행 빈도를 변경하는 것이 프로그램의 결과에 영향을 주는가?

**x86.py output**

```bash
$ python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -c
ARG seed 0
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 3
ARG interrupt randomness True
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    3   ------ Interrupt ------  ------ Interrupt ------
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    2   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 sub  $1,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1   1001 test $0,%dx
   -1   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1001 test $0,%dx
    0                            1002 jgte .top
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1   1003 halt
    0   ----- Halt;Switch -----  ----- Halt;Switch -----
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------
   -1                            1002 jgte .top
   -1                            1003 halt
```

앞서 2번 문제에 대한 답변에서 서술한 바와 같이 각각의 쓰레드가 독립적인 레지스터를 갖고 있는 상황에서 경쟁 조건 자체가 성립할 수 없다.

%dx 레지스터는 공유되는 자원이 아닌 각각의 쓰레드가 갖고 있는 독립적인 자원이다. 같은 코어에서 두 쓰레드가 실행된다고 하더라도 컨텍스트 스위칭 시 TCB에 %dx 값이 저장되고 새롭게 실행될 쓰레드의 %dx 값이 복원된다.

그러므로 인터럽트의 발생 빈도는 실행 흐름을 비결정론적(non-deterministic)이게 만들 수 없다.

## Problem 4

    다음으로 looping-race-nolock.s 라는 다른 프로그램을 살펴보자. 이 프로그램은 2000번지의 메모리 주소에 위치한 공유 변수에 접근한다. 간단하게 하기 위해서 이 변수를 x라고 하겠다. 다음과 같이 단일 쓰레드로 실행하고, 어떤 일을 하는지 확실하게 이해하자.
    ./x86.py −p looping−race−nolock.s −t 1 −M 2000
    전체 실행 시간 동안 x 의 값은 무엇인가 (즉, 메모리 2000번지의 값)? -c를 사용하여 답을 확인해 보자.

**x86.py output**

```bash
$ python3 x86.py -p looping-race-nolock.s -t 1 -M 2000 -c
ARG seed 0
ARG numthreads 1
ARG program looping-race-nolock.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt
```

**looping-race-nolock.s**

```
# assumes %bx has loop count in it

.main
.top
# critical section
mov 2000, %ax  # get 'value' at address 2000
add $1, %ax    # increment it
mov %ax, 2000  # store it back

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

위 어셈블리 프로그램은 앞서 살펴본 **loop.s** 프로그램의 루프 안에 `LOAD` - `ADD` - `STORE` 연산을 추가한 것이다. 의사코드로는 다음과 같이 나타낼 수 있다.

**looping-race-nolock.s pseudocode**

```c
do {
    volatile_variable = volatile_variable + 1;
    --i;
} while (i >= 0);

exit(0);
```

이 코드는 기본적으로 경쟁 조건을 유발하지만 주어진 옵션은 단일 쓰레드 실행을 정의하기 때문에 경쟁 조건을 확인할 수는 없다.

## Problem 5

    이번에는 쓰레드의 개수를 늘리고 반복 횟수도 늘려서 실행해 보자.
    ./x86.py −p looping−race−nolock.s −t 2 −a bx=3 −M 2000
    각 쓰레드의 코드가 왜 세 번씩 반복하는지 이해되는가? x의 최종 값은 무엇이 되겠는가?

**x86.py output**

```bash
$ python3 x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000 -c
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv bx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1000 mov 2000, %ax
    1   1001 add $1, %ax
    2   1002 mov %ax, 2000
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   1000 mov 2000, %ax
    2   1001 add $1, %ax
    3   1002 mov %ax, 2000
    3   1003 sub  $1, %bx
    3   1004 test $0, %bx
    3   1005 jgt .top
    3   1006 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----
    3                            1000 mov 2000, %ax
    3                            1001 add $1, %ax
    4                            1002 mov %ax, 2000
    4                            1003 sub  $1, %bx
    4                            1004 test $0, %bx
    4                            1005 jgt .top
    4                            1000 mov 2000, %ax
    4                            1001 add $1, %ax
    5                            1002 mov %ax, 2000
    5                            1003 sub  $1, %bx
    5                            1004 test $0, %bx
    5                            1005 jgt .top
    5                            1000 mov 2000, %ax
    5                            1001 add $1, %ax
    6                            1002 mov %ax, 2000
    6                            1003 sub  $1, %bx
    6                            1004 test $0, %bx
    6                            1005 jgt .top
    6                            1006 halt
```

한 쓰레드가 종료되기 전까지는 인터럽트가 발생하지 않으므로 임계 영역에 두 쓰레드가 동시에 진입하는 일이 발생하지 않는다. 그러므로 실행 흐름은 결정론적이고 메모리 2000번지에는 최종적으로 6이 저장된다.

## Problem 6

    랜덤 간격으로 인터럽트가 발생하도록 하여 실행해 보자.
    ./x86.py −p looping−race−nolock.s −t 2 −M 2000 −i 4 −r −s 0
    실행한 후에 랜덤 함수의 시드를 -s 1 로 해보고 -s 2 등으로 변경해 보자. 쓰레드 실행 순서를 보고 x의 최종 값이 무엇인지 알 수 있겠는가? 인터럽트의 정확한 발생 위치가 중요한가? 인터럽트가 발생해도 안전한 위치는 어디인가? 인터럽트가 문제를 일으키는 위치는 어디인가? 다시 말해 정확하게 어디가 임계 영역인가?

**x86.py output**

```bash
$ python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1 -c
ARG seed 1
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness True
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1005 jgt .top
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1006 halt
```

시드를 1로 설정했을 때 경쟁 조건으로 인한 동기화 문제가 발생한다. 메모리에서 값을 읽어와 레지스터에 저장한 순간 인터럽트가 발생하고, 다른 쓰레드가 메모리로부터 변경되지 않은 값을 읽어와 변경 후 저장한 후 또 다시 인터럽트가 발생한다. 이때 두 쓰레드는 메모리로부터 같은 값을 읽어왔기 때문에 `ADD` 연산은 두 번 수행되었음에도 값은 1만큼만 증가하게 된다.

그러므로 임계 영역은 `mov`, `add`, `mov`의 세 명령어가 수행되는 구간이다.

## Problem 7

    이번에는 인터럽트가 일정 간격으로 발생하도록 지정하고 프로그램의 동작을 살펴보자.
    ./x86.py −p looping−race nolock.s −a bx=1 −t 2 −M 2000 −i 1
    공유 변수 x의 최종 값을 예측할 수 있는지 한 번 해 보라. -i 2 나 -i 3 와 같이 변경했을 때는 가능하겠는가? 인터럽트의 간격을 얼마로 했을 때 “올바른” 최종 정답을 얻을 수 있는가?

**x86.py output**

```bash
$ python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 3 -c
ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 3
ARG interrupt randomness False
ARG argv bx=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace
ARG cctrace False
ARG printstats False
ARG verbose False

 2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2                            1003 sub  $1, %bx
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1006 halt
```

**looping-race-nolock.s** 프로그램은 6개의 명령어로 구성되어 있다. 처음 3개의 명령어는 임계 영역의 로직을 구성하고, 나머지 3개의 명령어는 독립적인 로직을 구성한다. 그러므로 인터럽트가 명령어 3회 수행 시마다, 또는 3의 배수 회만큼 수행 시마다 발생한다면 프로그램은 결정론적인 실행 흐름을 가질 수 있다.

## Problem 8

    반복 횟수를 늘려서 (예, -a bx=100) 위의 명령어를 실행해 보자. -i 플래그로 설정되는 인터럽트의 간격을 얼마로 정할 때 “올바른” 결과를 나타내는가? 간격이 얼마일 때 예기치 못한 결과가 나오는가?

7번 문제에서 서술한 바와 같이 `-i` 플래그로 설정되는 인터럽트의 간격을 3의 배수로 설정할 때 결정론적이며 올바른 결과를 나타낼 수 있다.

## Problem 9

    이 숙제의 마지막으로 wait-for-me.s 프로그램을 살펴보려고 한다. 다음과 같이 코드를 실행해 보자.
    ./x86.py −p wait−for−me.s −a ax=1, ax=0 −R ax −M 2000
    쓰레드 0의 %ax 레지스터는 1로 설정하고 쓰레드 1의 레지스터는 0으로 설정한 후에 프로그램이 실행하는 동안 %ax의 값과 메모리 2000번지에 있는 값을 주시하라. 코드는 어떤 행동을 보여야 하는가? 쓰레드들은 2000번지의 값을 어떻게 사용하는가? 최종 값은 무엇이 되겠는가?

**x86.py output**

```bash
$ python3 x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000 -c
ARG seed 0
ARG numthreads 2
ARG program wait-for-me.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv ax=1,ax=0
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace ax
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      ax          Thread 0                Thread 1
    0       1
    0       1   1000 test $1, %ax
    0       1   1001 je .signaller
    1       1   1006 mov  $1, 2000
    1       1   1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----
    1       0                            1000 test $1, %ax
    1       0                            1001 je .signaller
    1       0                            1002 mov  2000, %cx
    1       0                            1003 test $1, %cx
    1       0                            1004 jne .waiter
    1       0                            1005 halt
```

**wait-for-me.s**

```
.main
test $1, %ax     # ax should be 1 (signaller) or 0 (waiter)
je .signaller

.waiter
mov  2000, %cx
test $1, %cx
jne .waiter
halt

.signaller
mov  $1, 2000
halt
```

위 어셈블리 프로그램은 두 경로로 분기된다. %ax 레지스터의 값이 1인 쓰레드는 기다리는 쓰레드에게 실행 조건을 알리는 역할(.signaller), 0인 쓰레드는 기다리는 역할(.waiter)을 한다. waiter는 메모리 2000번지의 값이 1로 바뀌기 전까지는 busy waiting을 하도록 설계되어 있다. 그러므로 프로그램 종료 시 메모리 2000번지의 값은 1인 것이 보장된다.

## Problem 10

    입력 인자를 다음과 같이 바꿔보자.
    ./x86.py −p wait−for−me.s −a ax=0,ax=1 −R ax −M 2000
    쓰레드들은 어떤 행동을 보이는가? 쓰레드 0은 무엇을 하는가? 인터럽트 간격을 변경하면 (예, -i 1000 또는 랜덤 간격으로) 실행 추적 결과는 어떻게 달라지는가? 프로그램이 CPU를 효율적으로 사용하고 있는가?

**x86.py output**

```bash
$ python3 x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000 -c
ARG seed 0
ARG numthreads 2
ARG program wait-for-me.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv ax=0,ax=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace ax
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      ax          Thread 0                Thread 1
    0       0
    0       0   1000 test $1, %ax
    0       0   1001 je .signaller
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       1   ------ Interrupt ------  ------ Interrupt ------
    0       1                            1000 test $1, %ax
    0       1                            1001 je .signaller
    1       1                            1006 mov  $1, 2000
    1       1                            1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----
    1       0   1002 mov  2000, %cx
    1       0   1003 test $1, %cx
    1       0   1004 jne .waiter
    1       0   1005 halt
```

이 프로그램의 효율성은 signaller가 얼마나 빠르게 CPU 제어권을 획득하냐에 따라 갈린다. waiter는 busy waiting 방식으로 메모리 값의 변경을 기다리고 있기 때문에 만약 waiter가 CPU 제어권을 먼저 획득하게 되면 인터럽트가 발생하기 전까지 불필요하게 CPU 자원을 낭비하게 된다. 하지만 signaller가 먼저 수행되어 단 3회의 명령어만 먼저 수행한다면 waiter는 즉시 종료할 수 있다.

만약 인터럽트가 너무 자주 발생한다면 현실적인 컴퓨터 시스템에서는 컨텍스트 스위칭의 비용이 커질 것이다. 그렇다고 너무 적게 발생한다면 waiter 프로세스는 busy waiting 루프로 CPU 자원을 낭비하게 된다. 그러므로 이 경우 가장 최적의 인터럽트 발생 주기는 명령어 3회 실행 시마다가 될 것이다.
