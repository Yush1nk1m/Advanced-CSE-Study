# OSTEP Chapter 31 Homework

## Problem 1

    x86.py에 -p flag.s라는 플래그를 주고 실행할 준비를 하자. 단일 메모리 플래그를 사용하여 락을 구현하였다. 어셈블리 코드가 무엇을 하려고 하는지 알겠는가?

**flag.s**

```
.var flag
.var count

.main
.top

.acquire
mov  flag, %ax      # get flag
test $0, %ax        # if we get 0 back: lock is free!
jne  .acquire       # if not, try again
mov  $1, flag       # store 1 into flag

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, flag       # clear the flag now

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

`flag`는 락이 점유되었는지 나타내는 플래그 변수이다. 이 값이 1이면 `.acquire` 블럭에서 무한 루프를 수행하고 0이면 1으로 설정한 후 임계 영역으로 진입한 후 `count` 값을 1 증가시키고 플래그 값을 다시 0으로 설정해 락을 반환한다. 이러한 과정을 `%bx` 레지스터에 저장된 값만큼 반복하므로 `%bx` 레지스터는 쓰레드마다 증가시켜야 할 `count` 값의 양이라고 생각할 수 있다.

## Problem 2

    기본 값을 사용하여 실행하면 flag.s 가 의도한 것처럼 동작하는가? 올바른 결과를 만들어 내는가? -M과 -R 플래그를 사용하여 변수와 레지스터의 값의 흐름을 살펴보라 (-c를 플래그를 사용하여 값을 확인할 수 있다). 코드가 실행되면 최종적으로 flag 변수가 갖는 값을 예측할 수 있겠는가?

**x86.py** 프로그램에서 새롭게 정의한 변수들은 메모리 주소 100번지부터 저장된다. 그러므로 **flag.s** 프로그램에 있는 변수 `flag`, `count`는 각각 100, 104번지에 저장된다. 그러므로 `-M` 옵션으로는 이 주소를 참조할 수 있다.

**x86.py output**

```bash
$ python x86.py -p flag.s -R ax,bx -M 100,104 -c
ARG seed 0
ARG numthreads 2
ARG program flag.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG procsched
ARG argv
ARG load address 1000
ARG memsize 128
ARG memtrace 100,104
ARG regtrace ax,bx
ARG cctrace False
ARG printstats False
ARG verbose False


  100   104      ax    bx          Thread 0                Thread 1

    0     0       0     0
    0     0       0     0   1000 mov  flag, %ax
    0     0       0     0   1001 test $0, %ax
    0     0       0     0   1002 jne  .acquire
    1     0       0     0   1003 mov  $1, flag
    1     0       0     0   1004 mov  count, %ax
    1     0       1     0   1005 add  $1, %ax
    1     1       1     0   1006 mov  %ax, count
    0     1       1     0   1007 mov  $0, flag
    0     1       1    -1   1008 sub  $1, %bx
    0     1       1    -1   1009 test $0, %bx
    0     1       1    -1   1010 jgt .top
    0     1       1    -1   1011 halt
    0     1       0     0   ----- Halt;Switch -----  ----- Halt;Switch -----
    0     1       0     0                            1000 mov  flag, %ax
    0     1       0     0                            1001 test $0, %ax
    0     1       0     0                            1002 jne  .acquire
    1     1       0     0                            1003 mov  $1, flag
    1     1       1     0                            1004 mov  count, %ax
    1     1       2     0                            1005 add  $1, %ax
    1     2       2     0                            1006 mov  %ax, count
    0     2       2     0                            1007 mov  $0, flag
    0     2       2    -1                            1008 sub  $1, %bx
    0     2       2    -1                            1009 test $0, %bx
    0     2       2    -1                            1010 jgt .top
    0     2       2    -1                            1011 halt
```

기본 옵션에서는 프로그램이 의도한 대로 순차적으로 락을 획득하고 값을 갱신한다. 하지만 이는 쓰레드 안전(thread-safe)하게 작성된 프로그램은 아니다. 플래그가 0인 상태에서 어떤 쓰레드가 맨 처음 `mov` 명령을 수행하자마자 인터럽트가 발생하여 다른 쓰레드가 똑같은 값을 `mov` 명령으로 불러오게 되면 두 쓰레드가 락을 동시에 획득하는 일이 발생할 수 있기 때문이다.

## Problem 3

    %bx 레지스터의 값을 -a 플래그를 사용하여 변경해 보라. 두 개의 쓰레드를 사용하고 있다면 -a bx=2,bx=2를 사용하면 된다. 이 코드는 어떤 동작을 하는가? 위의 질문을 여기도 적용해 보면 답은 무엇인가?

**x86.py output**

```bash
$ python x86.py -p flag.s -R ax,bx -M 100,104 -a bx=2,bx=2 -c
ARG seed 0
ARG numthreads 2
ARG program flag.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG procsched
ARG argv bx=2,bx=2
ARG load address 1000
ARG memsize 128
ARG memtrace 100,104
ARG regtrace ax,bx
ARG cctrace False
ARG printstats False
ARG verbose False


  100   104      ax    bx          Thread 0                Thread 1

    0     0       0     2
    0     0       0     2   1000 mov  flag, %ax
    0     0       0     2   1001 test $0, %ax
    0     0       0     2   1002 jne  .acquire
    1     0       0     2   1003 mov  $1, flag
    1     0       0     2   1004 mov  count, %ax
    1     0       1     2   1005 add  $1, %ax
    1     1       1     2   1006 mov  %ax, count
    0     1       1     2   1007 mov  $0, flag
    0     1       1     1   1008 sub  $1, %bx
    0     1       1     1   1009 test $0, %bx
    0     1       1     1   1010 jgt .top
    0     1       0     1   1000 mov  flag, %ax
    0     1       0     1   1001 test $0, %ax
    0     1       0     1   1002 jne  .acquire
    1     1       0     1   1003 mov  $1, flag
    1     1       1     1   1004 mov  count, %ax
    1     1       2     1   1005 add  $1, %ax
    1     2       2     1   1006 mov  %ax, count
    0     2       2     1   1007 mov  $0, flag
    0     2       2     0   1008 sub  $1, %bx
    0     2       2     0   1009 test $0, %bx
    0     2       2     0   1010 jgt .top
    0     2       2     0   1011 halt
    0     2       0     2   ----- Halt;Switch -----  ----- Halt;Switch -----
    0     2       0     2                            1000 mov  flag, %ax
    0     2       0     2                            1001 test $0, %ax
    0     2       0     2                            1002 jne  .acquire
    1     2       0     2                            1003 mov  $1, flag
    1     2       2     2                            1004 mov  count, %ax
    1     2       3     2                            1005 add  $1, %ax
    1     3       3     2                            1006 mov  %ax, count
    0     3       3     2                            1007 mov  $0, flag
    0     3       3     1                            1008 sub  $1, %bx
    0     3       3     1                            1009 test $0, %bx
    0     3       3     1                            1010 jgt .top
    0     3       0     1                            1000 mov  flag, %ax
    0     3       0     1                            1001 test $0, %ax
    0     3       0     1                            1002 jne  .acquire
    1     3       0     1                            1003 mov  $1, flag
    1     3       3     1                            1004 mov  count, %ax
    1     3       4     1                            1005 add  $1, %ax
    1     4       4     1                            1006 mov  %ax, count
    0     4       4     1                            1007 mov  $0, flag
    0     4       4     0                            1008 sub  $1, %bx
    0     4       4     0                            1009 test $0, %bx
    0     4       4     0                            1010 jgt .top
    0     4       4     0                            1011 halt
```

기본 옵션은 인터럽트가 50개의 명령어를 실행할 때마다 발생하도록 되어 있으므로 `%bx` 레지스터의 값을 2 정도로 설정하는 것은 동시성 문제를 관찰하기엔 충분하지 않다. 이는 마치 안전하지 않은 락 메커니즘의 위험성도 즉시 발견되지 않을 수 있다는 것을 시사한다.

## Problem 4

    %bx의 값을 큰 수로 변경한 후 -i 플래그를 사용하여 인터럽트 발생 빈도를 조정해보라. 어떤 값이 이상한 결과를 만들어 내는가? 정상적인 결과를 얻을 수 있는 값은 무엇인가?

**flag.s**

```
.var flag
.var count

.main
.top

.acquire
mov  flag, %ax      # get flag
test $0, %ax        # if we get 0 back: lock is free!
jne  .acquire       # if not, try again
mov  $1, flag       # store 1 into flag

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, flag       # clear the flag now

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

`%bx`의 값이 0보다 크다고 가정하고 락 획득의 성공/실패에 따라 한 번의 루프에서 실행되는 명령의 개수를 먼저 계산해 보겠다. 어떤 프로세스가 이번 루프에서 락 획득에 성공한다면 이 프로세스는 $11$ 회의 명령을 수행한 후 `.top` 위치로 복귀한다. 하지만 락 획득에 실패한다면 $3$ 회의 명령을 수행한 후 `.top` 위치로 복귀한다.

정상적인 결과를 얻으려면 인터럽트 발생 주기가 두 케이스에서 실행하는 명령 횟수의 공배수인 $3 \times 11 = 33$ 회로 설정되어야 한다. 이는 락 메커니즘의 성공/실패 여부가 인터럽트 주기에 의존적이게 되는 것이므로 락 메커니즘 자체가 유효하다고 말하기는 어렵다. 유효한 락 메커니즘은 인터럽트의 발생 주기와 상관없이 동시성 문제를 다룰 수 있어야 한다.

즉, 인터럽트 발생 주기를 통제할 수 있는 지금과 같은 환경에서 이상한 결과를 만들어내는 인터럽트 발생 빈도는 $33$ 의 배수를 제외한 모든 값들이고, 정상적인 결과를 만들어내는 인터럽트 발생 빈도는 $33$의 배수가 된다.

## Problem 5

    이제 Test-And-Set.s 프로그램을 살펴보자낮 먼저, xchg 명령어를 사용하여 간단한 락 기법을 구현한 코드를 이해해 보도록 하자. 락을 획득하기 위해 작성된 코드는 어떻게 작성되었는가? 락을 해제하는 부분의 코드는 어떻게 작성되었는가?

**test-and-set.s**

```
.var mutex
.var count

.main
.top

.acquire
mov  $1, %ax
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
jne  .acquire       # if not, try again

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, mutex

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

락 획득을 위해 `%ax` 레지스터에 1을 저장하고 `xchg` 단일 명령으로 `%ax` 레지스터와 `mutex`에 저장된 값을 교체한다. 이때 `%ax` 레지스터로 교체된 값(원래 `mutex`에 저장되어 있던 값)이 0이라면 락 획득에 성공한 것으로 간주하고 임계 영역에 접근한다. 락 반환 시에는 `xchg` 명령이 필요하지 않고 더 단순한 동작을 수행하는 `mov` 명령을 통해 `mutex`에 0을 저장한다.

만약 단일 어셈블리 명령이 원자적으로 수행되는 것이 보장된다면 이는 유효한 락 메커니즘이다. 왜냐하면 그러한 상황에서 `mutex`에 0이 저장되어 있을 때 이를 1로 변경하고 자신의 `%ax` 레지스터에 그 값을 가져올 수 있는 쓰레드는 단 하나뿐이기 때문이다.

## Problem 6

    이제 인터럽트 간격 (-i)을 변경해가면서 코드를 실행해 보자. 반복문이 여러 차례 수행될 수 있도록 해야 하는 것을 유의하자. 코드가 항상 의도한 대로 동작하는가? CPU를 비효율적으로 사용하는 경우가 가끔 발생하는가? 수치화할 수 있겠는가?

**x86.py output**

```bash
$ python x86.py -p test-and-set.s -R ax,bx -M 100,104 -a bx=3,bx=3 -i 1 -c
ARG seed 0
ARG numthreads 2
ARG program test-and-set.s
ARG interrupt frequency 1
ARG interrupt randomness False
ARG procsched
ARG argv bx=3,bx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 100,104
ARG regtrace ax,bx
ARG cctrace False
ARG printstats False
ARG verbose False


  100   104      ax    bx          Thread 0                Thread 1

    0     0       0     3
    0     0       1     3   1000 mov  $1, %ax
    0     0       0     3   ------ Interrupt ------  ------ Interrupt ------
    0     0       1     3                            1000 mov  $1, %ax
    0     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     3   1001 xchg %ax, mutex
    1     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     3                            1001 xchg %ax, mutex
    1     0       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     3   1002 test $0, %ax
    1     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     3                            1002 test $0, %ax
    1     0       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     3   1003 jne  .acquire
    1     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     3                            1003 jne  .acquire
    1     0       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     3   1004 mov  count, %ax
    1     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     3                            1000 mov  $1, %ax
    1     0       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     3   1005 add  $1, %ax
    1     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     3                            1001 xchg %ax, mutex
    1     0       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     3   1006 mov  %ax, count
    1     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     3                            1002 test $0, %ax
    1     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     3   1007 mov  $0, mutex
    0     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     3                            1003 jne  .acquire
    0     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     2   1008 sub  $1, %bx
    0     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     3                            1000 mov  $1, %ax
    0     1       1     2   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     2   1009 test $0, %bx
    0     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       0     3                            1001 xchg %ax, mutex
    1     1       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     2   1010 jgt .top
    1     1       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       0     3                            1002 test $0, %ax
    1     1       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     2   1000 mov  $1, %ax
    1     1       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       0     3                            1003 jne  .acquire
    1     1       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     2   1001 xchg %ax, mutex
    1     1       0     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     3                            1004 mov  count, %ax
    1     1       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     2   1002 test $0, %ax
    1     1       1     3   ------ Interrupt ------  ------ Interrupt ------
    1     1       2     3                            1005 add  $1, %ax
    1     1       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     2   1003 jne  .acquire
    1     1       2     3   ------ Interrupt ------  ------ Interrupt ------
    1     2       2     3                            1006 mov  %ax, count
    1     2       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       1     2   1000 mov  $1, %ax
    1     2       2     3   ------ Interrupt ------  ------ Interrupt ------
    0     2       2     3                            1007 mov  $0, mutex
    0     2       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       0     2   1001 xchg %ax, mutex
    1     2       2     3   ------ Interrupt ------  ------ Interrupt ------
    1     2       2     2                            1008 sub  $1, %bx
    1     2       0     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       0     2   1002 test $0, %ax
    1     2       2     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       2     2                            1009 test $0, %bx
    1     2       0     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       0     2   1003 jne  .acquire
    1     2       2     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       2     2                            1010 jgt .top
    1     2       0     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       2     2   1004 mov  count, %ax
    1     2       2     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       1     2                            1000 mov  $1, %ax
    1     2       2     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       3     2   1005 add  $1, %ax
    1     2       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     2       1     2                            1001 xchg %ax, mutex
    1     2       3     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       3     2   1006 mov  %ax, count
    1     3       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       1     2                            1002 test $0, %ax
    1     3       3     2   ------ Interrupt ------  ------ Interrupt ------
    0     3       3     2   1007 mov  $0, mutex
    0     3       1     2   ------ Interrupt ------  ------ Interrupt ------
    0     3       1     2                            1003 jne  .acquire
    0     3       3     2   ------ Interrupt ------  ------ Interrupt ------
    0     3       3     1   1008 sub  $1, %bx
    0     3       1     2   ------ Interrupt ------  ------ Interrupt ------
    0     3       1     2                            1000 mov  $1, %ax
    0     3       3     1   ------ Interrupt ------  ------ Interrupt ------
    0     3       3     1   1009 test $0, %bx
    0     3       1     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       0     2                            1001 xchg %ax, mutex
    1     3       3     1   ------ Interrupt ------  ------ Interrupt ------
    1     3       3     1   1010 jgt .top
    1     3       0     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       0     2                            1002 test $0, %ax
    1     3       3     1   ------ Interrupt ------  ------ Interrupt ------
    1     3       1     1   1000 mov  $1, %ax
    1     3       0     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       0     2                            1003 jne  .acquire
    1     3       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     3       1     1   1001 xchg %ax, mutex
    1     3       0     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       3     2                            1004 mov  count, %ax
    1     3       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     3       1     1   1002 test $0, %ax
    1     3       3     2   ------ Interrupt ------  ------ Interrupt ------
    1     3       4     2                            1005 add  $1, %ax
    1     3       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     3       1     1   1003 jne  .acquire
    1     3       4     2   ------ Interrupt ------  ------ Interrupt ------
    1     4       4     2                            1006 mov  %ax, count
    1     4       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       1     1   1000 mov  $1, %ax
    1     4       4     2   ------ Interrupt ------  ------ Interrupt ------
    0     4       4     2                            1007 mov  $0, mutex
    0     4       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       0     1   1001 xchg %ax, mutex
    1     4       4     2   ------ Interrupt ------  ------ Interrupt ------
    1     4       4     1                            1008 sub  $1, %bx
    1     4       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       0     1   1002 test $0, %ax
    1     4       4     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       4     1                            1009 test $0, %bx
    1     4       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       0     1   1003 jne  .acquire
    1     4       4     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       4     1                            1010 jgt .top
    1     4       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       4     1   1004 mov  count, %ax
    1     4       4     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       1     1                            1000 mov  $1, %ax
    1     4       4     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       5     1   1005 add  $1, %ax
    1     4       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     4       1     1                            1001 xchg %ax, mutex
    1     4       5     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       5     1   1006 mov  %ax, count
    1     5       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       1     1                            1002 test $0, %ax
    1     5       5     1   ------ Interrupt ------  ------ Interrupt ------
    0     5       5     1   1007 mov  $0, mutex
    0     5       1     1   ------ Interrupt ------  ------ Interrupt ------
    0     5       1     1                            1003 jne  .acquire
    0     5       5     1   ------ Interrupt ------  ------ Interrupt ------
    0     5       5     0   1008 sub  $1, %bx
    0     5       1     1   ------ Interrupt ------  ------ Interrupt ------
    0     5       1     1                            1000 mov  $1, %ax
    0     5       5     0   ------ Interrupt ------  ------ Interrupt ------
    0     5       5     0   1009 test $0, %bx
    0     5       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       0     1                            1001 xchg %ax, mutex
    1     5       5     0   ------ Interrupt ------  ------ Interrupt ------
    1     5       5     0   1010 jgt .top
    1     5       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       0     1                            1002 test $0, %ax
    1     5       5     0   ------ Interrupt ------  ------ Interrupt ------
    1     5       5     0   1011 halt
    1     5       0     1   ----- Halt;Switch -----  ----- Halt;Switch -----
    1     5       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       0     1                            1003 jne  .acquire
    1     5       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       5     1                            1004 mov  count, %ax
    1     5       5     1   ------ Interrupt ------  ------ Interrupt ------
    1     5       6     1                            1005 add  $1, %ax
    1     5       6     1   ------ Interrupt ------  ------ Interrupt ------
    1     6       6     1                            1006 mov  %ax, count
    1     6       6     1   ------ Interrupt ------  ------ Interrupt ------
    0     6       6     1                            1007 mov  $0, mutex
    0     6       6     1   ------ Interrupt ------  ------ Interrupt ------
    0     6       6     0                            1008 sub  $1, %bx
    0     6       6     0   ------ Interrupt ------  ------ Interrupt ------
    0     6       6     0                            1009 test $0, %bx
    0     6       6     0   ------ Interrupt ------  ------ Interrupt ------
    0     6       6     0                            1010 jgt .top
    0     6       6     0   ------ Interrupt ------  ------ Interrupt ------
    0     6       6     0                            1011 halt
```

인터럽트 발생 주기를 1로 설정하였지만 동시성 문제는 절대로 발생하지 않고 코드는 항상 의도한 대로 동작한다. 하지만 쓰레드가 락 획득을 위해 busy-waiting 방식으로 대기하기 때문에 CPU를 비효율적으로 사용할 수밖에 없게 된다.

**test-and-set.s** 코드에 의하면 락을 획득한 쓰레드는 `xchg` 명령으로 락을 획득한 이후부터 `mov` 명령으로 락을 반납할 때까지 총 6회의 명령을 수행한다. 즉, 락에 의해 보호되는 임계 영역의 명령은 6회이다.

총 $T$ 개의 쓰레드가 있고, 인터럽트 발생 주기는 $1$ 이라고 가정하겠다. 락을 획득한 $1$ 개의 쓰레드가 임계 영역의 $6$ 회 명령을 모두 실행하는 동안 락을 획득하지 못한 $T - 1$ 개의 쓰레드는 락 점유를 위해 각각 $6$ 회의 busy-waiting 방식의 루프 명령을 실행한다. 그러므로 전체 $6T$ 회의 명령 중 $6$ 회의 명령만이 유효한 임계 영역의 명령이다. 그러므로 CPU 효율은 $\frac{1}{T}$ 가 된다.

## Problem 7

    -P 플래그를 사용하여 락 코드를 테스트해 보자. 예를 들어, 첫 번째 쓰레드가 락을 획득하자마자 두 번째 쓰레드가 락 획득을 시도하게 스케줄을 짜보자. 올바르게 동작하는가? 무엇을 테스트해 보아야 하겠는가?

**x86.py output**

```bash
$ python x86.py -p flag.s -R ax,bx -M 100,104 -a bx=1,bx=1 -i 1 -P 1100 -c
...
  100   104      ax    bx          Thread 0                Thread 1

    0     0       0     1
    0     0       0     1                            1000 mov  flag, %ax
    0     0       0     1                            1001 test $0, %ax
    0     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    0     0       0     1   1000 mov  flag, %ax
    0     0       0     1   1001 test $0, %ax
    0     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    0     0       0     1                            1002 jne  .acquire
    1     0       0     1                            1003 mov  $1, flag
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     1   1002 jne  .acquire
    1     0       0     1   1003 mov  $1, flag
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     1                            1004 mov  count, %ax
    1     0       1     1                            1005 add  $1, %ax
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     1   1004 mov  count, %ax
    1     0       1     1   1005 add  $1, %ax
    1     0       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     1       1     1                            1006 mov  %ax, count
    0     1       1     1                            1007 mov  $0, flag
    0     1       1     1   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     1   1006 mov  %ax, count
    0     1       1     1   1007 mov  $0, flag
    0     1       1     1   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     0                            1008 sub  $1, %bx
    0     1       1     0                            1009 test $0, %bx
    0     1       1     1   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     0   1008 sub  $1, %bx
    0     1       1     0   1009 test $0, %bx
    0     1       1     0   ------ Interrupt ------  ------ Interrupt ------
    0     1       1     0                            1010 jgt .top
    0     1       1     0                            1011 halt
    0     1       1     0   ----- Halt;Switch -----  ----- Halt;Switch -----
    0     1       1     0   1010 jgt .top
    0     1       1     0   1011 halt

$ python x86.py -p test-and-set.s -R ax,bx -M 100,104 -a bx=1,bx=1 -i 1 -P 001111111 -c
...
  100   104      ax    bx          Thread 0                Thread 1

    0     0       0     1
    0     0       1     1   1000 mov  $1, %ax
    1     0       0     1   1001 xchg %ax, mutex
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     1                            1000 mov  $1, %ax
    1     0       1     1                            1001 xchg %ax, mutex
    1     0       1     1                            1002 test $0, %ax
    1     0       1     1                            1003 jne  .acquire
    1     0       1     1                            1000 mov  $1, %ax
    1     0       1     1                            1001 xchg %ax, mutex
    1     0       1     1                            1002 test $0, %ax
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     1   1002 test $0, %ax
    1     0       0     1   1003 jne  .acquire
...
```

**flag.s**의 경우 `mov` 명령 실행 직후 인터럽트가 발생하도록 실행 계획을 수립하면 두 쓰레드가 동시에 락을 획득한다. **test-and-set.s**의 경우 어떤 실행 계획을 수립하더라도 두 쓰레드가 동시에 락을 획득하는 일은 발생하지 않는다. 이는 락 획득이 단일 명령어로 이루어지냐, 다중 명령어로 이루어지냐에 따른 차이이다.

## Problem 8

    이제 본문의 여담에서 다뤘던 피터슨의 알고리즘을 구현한 peterson.s의 코드를 살펴보자. 이해할 수 있는지 코드를 분석해 보라.

**peterson.s**

```
# array of 2 integers (each size 4 bytes)
# load address of flag into fx register
# access flag[] with 0(%fx,%index,4)
# where %index is a register holding 0 or 1
# index reg contains 0 -> flag[0], if 1->flag[1]
.var flag   2

# global turn variable
.var turn

# global count
.var count

.main

# put address of flag into fx
lea flag, %fx

# assume thread ID is in bx (0 or 1, scale by 4 to get proper flag address)
mov %bx, %cx   # bx: self, now copies to cx
neg %cx        # cx: - self
add $1, %cx    # cx: 1 - self

.acquire
mov $1, 0(%fx,%bx,4)    # flag[self] = 1
mov %cx, turn           # turn       = 1 - self

.spin1
mov 0(%fx,%cx,4), %ax   # flag[1-self]
test $1, %ax
jne .fini               # if flag[1-self] != 1, skip past loop to .fini

.spin2                  # just labeled for fun, not needed
mov turn, %ax
test %cx, %ax           # compare 'turn' and '1 - self'
je .spin1               # if turn==1-self, go back and start spin again

# fall out of spin
.fini

# do critical section now
mov count, %ax
add $1, %ax
mov %ax, count

.release
mov $0, 0(%fx,%bx,4)    # flag[self] = 0


# end case: make sure it's other's turn
mov %cx, turn           # turn       = 1 - self
halt
```

이 알고리즘은 단 두 개의 쓰레드만 락을 두고 경쟁할 수 있다. 만약 쓰레드가 두 개보다 많으면 쓰레드 안전하지 않다. 왜냐하면 락 획득 자체는 `mov`, `test` 두 개의 명령어를 통해 이루어지기 때문이다.

이 알고리즘의 핵심 아이디어는 '양보'이다. `flag` 변수를 통해서는 현재 쓰레드가 락 획득을 희망하는지를 나타내고 `turn` 변수로는 현재 락을 가져갈 수 있는 쓰레드를 지정한다. `.acquire` 레이블 아래 두 명령이 이러한 양보 로직을 나타낸다. 일단 현재 쓰레드는 락 획득을 희망한다고 표기한 후 선택권을 다른 쓰레드에게 양보한다. 다른 쓰레드도 마찬가지일 것이고 동시적 실행으로 인해 `turn` 값이 덮여 쓰여지더라도 상관없다.

`.spin1` 레이블 아래의 명령들은 다른 쓰레드가 현재 락 획득을 희망하는지 조사하고 아닐 경우 즉시 임계 영역으로 진입한다. `.spin2` 레이블 아래의 명령들은 현재 누구에게 락 획득 권리가 있는지를 확인하고 현재 쓰레드에게 락 획득 권리가 있을 경우 임계 영역으로 진입한다.

락 반환 시에는 단순히 `flag` 값을 0으로 설정하면 다른 쓰레드가 이를 감지하고 즉시 임계 영역으로 진입할 수 있다.

## Problem 9

    이제 -i에 여러 값을 넣어서 코드를 실행해 보자. 어떤 차이점이 보이는가?

인터럽트 발생 주기를 아무리 세밀하게 설정하더라도 피터슨 알고리즘으로 작성된 코드는 동시성 문제가 발생하지 않는다. 그러나 앞서 서술했듯 이것은 락을 두고 단 2개의 쓰레드만 서로 경쟁하는 상황에서만 유효한 알고리즘이다. `turn` 변수는 0과 1 두 가지의 값만 가질 수 있기 때문이다.

## Problem 10

    코드가 동작하는 것을 “증명”하기 위해 스케줄 (-P 플래그를 사용)을 제어할 수 있겠는가?

`-P 01`, `-P 10` 어떤 옵션을 쓰더라도 피터슨 알고리즘은 '양보'를 기반으로 동작하기 때문에 일단 다른 쓰레드의 선택권을 더 우선시한다. 현재 쓰레드가 락을 먼저 획득하려 하지 않고 `turn` 변수에 의해 순서를 지켜 graceful하게 락을 획득하기 때문에 어떤 실행 계획 하에서도 동시성 문제가 발생할 여지는 없다.

## Problem 11

    이제 ticket.s의 티켓 락 코드를 살펴보자. 본문에 나와 있는 코드와 동일한가?

**ticket.s**

```
.var ticket
.var turn
.var count

.main
.top

.acquire
mov $1, %ax
fetchadd %ax, ticket  # grab a ticket
.tryagain
mov turn, %cx         # check if it's your turn
test %cx, %ax
jne .tryagain

# critical section
mov  count, %ax       # get the value at the address
add  $1, %ax          # increment it
mov  %ax, count       # store it back

# release lock
mov $1, %ax
fetchadd %ax, turn

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

어셈블리 레벨에서는 코드가 더욱 간단해지지만 근본적인 동작 방식은 바뀌지 않았다. 예를 들어 `FetchAndAdd()` 함수를 통해 전역 티켓 값을 증가시키고 스핀 락을 통해 차례를 기다리는 로직은 어셈블리 레벨에서는 티켓 전역 변수의 값을 증가시키고 `%ax` 레지스터에 저장한 후, 차례 전역 변수의 값을 `%cx` 레지스터에 저장하여 두 레지스터의 값을 비교하는 반복문으로 더욱 간단하게 표현되었다.

핵심은 각각의 쓰레드가 티켓 값을 우선 증가시킴으로써 자기 자신의 고유한 티켓 번호를 갖게 되고, 현재 락을 점유 중인 쓰레드가 락을 반환할 때 차례 값을 증가시킴으로써 티켓 값을 증가시킨 순서대로 락을 획득한다는 것이다.

## Problem 12

    코드를 실행할 때 -a bx=1000,bx=1000라는 플래그를 사용해 보자. 이 플래그는 각 쓰레드가 임계 영역을 1000번 반복하도록 한다. 시간이 지남에 따라 어떻게 바뀌는지 살펴보자. 락을 기다리기 위해 회전하는 시간이 길어지는가?

시간이 지남에 따라 각 쓰레드가 락을 기다리기 위해 회전하는 시간은 평균적으로 일정하게 유지된다. 왜냐하면 앞서 서술한 바와 같이 티켓 락의 설계는 기아 현상을 방지하기 때문이다.

## Problem 13

    쓰레드를 추가해가면 코드의 동작이 어떻게 바뀌는가?

쓰레드의 개수와 상관없이 티켓을 획득하는 과정은 원자적이면서 결정적이고 최소한의 경쟁 상태만을 유발한다. 어떤 쓰레드가 락을 점유하고 있을 때 티켓을 획득하기 위해 경쟁하는 쓰레드들은 티켓 값을 통해 언제 락을 점유할 수 있는지가 결정되므로 Bounded Waiting이 보장된다.

## Problem 14

    이제 yield 명령어가 CPU의 사용권을 다른 쓰레드에 양보한다고 가정하고 yield.s를 살펴보자(실제로는 이 명령어는 운영체제의 기법이지만, 시뮬레이션을 간단하게 하기 위해서 이 동작을 위한 명령어가 있다고 하자). test-and-set.s 가 회전을 하면서 사이클을 낭비하지만 yield.s는 낭비하지 않는 시나리오를 찾아보자. 몇 개의 명령어가 절약되는가? 어떤 시나리오에서 이런 절약이 가능한가?

**yield.s**

```
.var mutex
.var count

.main
.top

.acquire
mov  $1, %ax
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
je .acquire_done
yield               # if not, yield and try again
j .acquire
.acquire_done

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, mutex

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

**test-and-set.s**에서 `.acquire` 블록은 다음과 같이 정의되어 있다.

```
.acquire
mov  $1, %ax
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
jne  .acquire       # if not, try again
```

락을 획득하지 못한 쓰레드는 `.acquire` 블록을 무한히 순회하기 때문에 busy-waiting이 발생한다. 싱글 쓰레드 시스템에서 Round-Robin 방식으로 CPU 스케줄링이 발생한다고 가정하자. 이때 락을 점유 중인 쓰레드가 임계 영역에 진입한 후에 Time Quantum을 소진한다면 (락을 획득하기 위해 경쟁하는 쓰레드의 개수) $\times$ (Time Quantum) 만큼의 시간 동안 CPU 자원은 busy-waiting으로 인해 낭비될 것이다.

멀티 쓰레드 시스템에서도 이러한 문제가 발생할 수 있다. 락을 점유 중인 쓰레드가 임계 영역을 수행하는 동안 락을 획득하기 위해 경쟁 중인 쓰레드들이 다른 코어에 디스패치된다면 CPU의 코어 개수에 비례하여 자원 낭비를 초래할 수 있다.

현실적으로 이 프로세스 외에도 수천 개의 프로세스가 CPU 자원을 획득하기 위해 경쟁 중이기 때문에 이러한 busy-waiting은 시스템 성능에 매우 부정적인 영향을 미친다. 이러한 점으로 미루어볼 때 `yield` 연산은 매우 획기적인 것이다. 락 획득에 실패한다면 즉시 CPU 자원을 반납함으로써 각 쓰레드가 매번 CPU 자원을 획득할 때마다 단 한 번만 락을 획득할 수 있는지 검사함으로써 CPU 자원을 낭비하지 않는다.

하지만 부정적인 측면도 생각해볼 수 있다. 일단 이 로직에는 기아 현상을 방지하기 위한 어떤 조치도 마련되어 있지 않기 때문에 어떤 쓰레드가 다음에 락을 획득할 것인지에 대한 문제는 비결정적으로 남아 있다.

또한, 멀티 쓰레드 시스템에서 하나의 코어는 락을 점유 중인 쓰레드가 사용하고, 다른 하나의 코어는 락을 획득하기 위해 경쟁 중인 쓰레드들이 사용한다고 가정하자. `yield` 명령어로 인해 계속해서 컨텍스트 스위칭이 발생하게 될 것이고, 이 경우 락이 반납되고 다시 획득되는 데 소요되는 시간이 busy-waiting 방식에 비해 더 길어질 수도 있다.

## Problem 15

    마지막으로, test-and-test-and-set.s를 살펴보자낮 이 락은 어떤 동작을 하는가? test-and-set.s과 비교하여 무엇을 절약할 수 있도록 해주는가?

**test-and-test-and-set.s**

```
.var mutex
.var count

.main
.top

.acquire
mov  mutex, %ax
test $0, %ax
jne .acquire
mov  $1, %ax
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
jne .acquire        # if not, try again

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, mutex

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top

halt
```

이 락은 다른 쓰레드가 락을 점유 중일 때는 `mov`, `test`, `jne` 연산만 사용함으로써 **test-and-set.s** 로직보다 락을 획득하기 위한 반복문이 가벼워졌다. 이후 락이 해제될 시 일제히 `mov`, `xchg` 연산을 통해 자신이 락을 획득하는 데 성공했는지 확인한다.

`xchg` 연산은 원자성을 보장하기 위해 CPU의 다른 모든 코어에게 L1, L2 캐시를 무효화하라는 메시지를 브로드캐스팅하고, 모든 코어의 응답을 수신해야만 비로소 값 교환을 수행할 수 있다. 즉, CPU 전체에 영향을 미치는 매우 무거운 연산이다. `xchg` 연산이 불필요하게 반복될 경우 락 프로세스가 사용 중인 코어뿐만 아니라 다른 작업을 수행하고 있는 코어들의 성능에도 부정적인 영향을 미칠 수 있다.

그러므로 이 로직은 **test-and-set.s** 로직보다 시스템 전체적으로 훨씬 적은 부하를 유발한다.
