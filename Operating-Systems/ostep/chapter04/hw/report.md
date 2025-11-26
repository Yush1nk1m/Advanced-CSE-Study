# OSTEP Chapter 04 Homework

숙제로 주어진 시뮬레이터 프로그램인 **process-run.py**는 간단한 CPU의 스케줄링 동작을 시뮬레이션하면서 표준 출력으로 시각화한다.

프로그램의 실행 매개변수로 개별 프로세스를 `X:Y`와 같은 형식으로 나타내는데, `X`는 프로세스가 실행해야 하는 명령어의 개수를 의미하고 `Y`는 명령어 중 CPU 작업이 필요한 명령어의 비율을 의미한다. 즉, `Y`의 값이 0에 가까울수록 정의한 프로세스는 I/O-bounded task가 되고 100에 가까울 수록 CPU-bounded task가 된다.

## Problem 1

    다음과 같이 플래그를 지정하고 프로그램을 실행시키시오: ./process-run.py -l 5:100, 5:100. CPU 이용률은 얼마가 되어야 하는가 (예, CPU가 사용 중인 시간의 퍼센트?) 그러한 이용률을 예측한 이유는 무엇인가? -c 플래그를 지정하여 예측이 맞는지 확인하시오.

```bash
$ python3 process-run.py -l 5:100,5:100 -c
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5        RUN:cpu         READY             1
  6           DONE       RUN:cpu             1
  7           DONE       RUN:cpu             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1
```

두 프로세스 모두 CPU 작업이 발생할 확률이 100%이기 때문에 CPU 이용률은 100%가 된다.

## Problem 2

    이제 다음과 같이 플래그를 지정하고 실행시키시오 : ./process-run.py -l 4:100, 1:0. 이 플래그는 4발의 명령어를 실행하고 모두 CPU만 사용하는 하나의 프로세스와 오직 입출력을 요청하고 완료되기를 기다리는 하나의 프로세스를 명시한다. 두 프로세스가 모두 종료되는 데 얼마의 시간이 걸리는가? -c 플래그를 사용하여 예측한 것이 맞는지 확인하시오.

```bash
$ python3 process-run.py -l 4:100,1:0 -c
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5           DONE        RUN:io             1
  6           DONE       BLOCKED                           1
  7           DONE       BLOCKED                           1
  8           DONE       BLOCKED                           1
  9           DONE       BLOCKED                           1
 10           DONE       BLOCKED                           1
 11*          DONE   RUN:io_done             1
```

일단 시뮬레이터는 다음과 같이 I/O 작업에 필요한 단위 시간의 기본 값을 5로 설정하고 있다.

```python
parser.add_option('-L', '--iolength', default=5, help='how long an IO takes', action='store', type='int', dest='io_length')
```

0번 프로세스는 4 단위 시간 동안 CPU-bounded task를 실행하고 종료한다. 핵심은 1번 프로세스의 동작이다. 1 단위 시간 동안 I/O 작업을 실행할 것을 명령, 5 단위 시간 동안 I/O 작업을 실행, 1 단위 시간 동안 마무리된 I/O 작업을 정리한다. 그러므로 총 11 단위 시간이 소요된다.

실제 프로세스에서 I/O 작업이 발생할 때는 시스템 콜을 호출하고 시스템 콜의 결과를 활용하는 부분이 있을 것이다. 이 시뮬레이터는 이런 상황을 잘 모방하고 있기 때문에 프로세스 옵션을 `1:0`처럼 주더라도 2 단위 시간의 CPU 작업은 필연적으로 발생하게 된다.

## Problem 3

    옵션으로 지정된 프로세스의 순서를 바꾸시오 : ./process-run.py -l 1:0, 4:100. 이제 어떤 결과가 나오는가? 실행 순서를 교환하는 것은 중요한가? 이유는 무엇인가? (언제나처럼 -c 플래그를 사용하여 예측이 맞는지 확인하시오.)

```bash
$ python3 process-run.py -l 1:0,4:100 -c
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1
```

I/O 작업이 수행되는 동안 CPU는 유휴 상태가 된다. 이때 다른 프로세스에게 CPU를 할당할 수 있으므로 프로세스의 실행 순서를 교환하는 것은 매우 중요하다. `1:0` 옵션의 프로세스는 1 단위 시간 동안 I/O 작업을 요청(시스템 콜 호출)하고 5 단위 시간 동안 대기 상태(Blocked)에 접어드는데, 이때 `4:100` 옵션의 프로세스가 CPU를 점유할 수 있다. 이 경우 두 프로세스가 모두 작업을 마치는 데 7 단위 시간이 소요되지만 반대로 실행할 경우엔 Problem 2와 같이 11 단위 시간이 소요된다.

## Problem 4

    자, 다른 플래그에 대해서도 알아보자. 중요한 플래그 중 하나는 -S로서 프로세 스가 입출력을 요청했을 때 시스템이 어떻게 반응하는지를 결정한다. 이 플래그가 SWITCH_ON_END로 지정되면 시스템은 요청 프로세스가 입출력을 하는 동안 다른 프로세스로 전환하지 않고 대신 요청 프로세스가 종료될 때까지 기다린다. 입출력만 수행하는 프로세스와 CPU 작업만 하는 프로세스 두 발을 실행시키면 어떤 결과가 발생하는가?(-l 1:0,4:100 -c -S SWITCH_ON_END)

```bash
$ python3 process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED         READY                           1
  3        BLOCKED         READY                           1
  4        BLOCKED         READY                           1
  5        BLOCKED         READY                           1
  6        BLOCKED         READY                           1
  7*   RUN:io_done         READY             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1
 11           DONE       RUN:cpu             1
```

`SWITCH_ON_END` 옵션과 같은 동작을 하는 시스템은 매우 비효율적이다. 이는 CPU 가상화가 존재하지 않는 시스템, 즉 컨텍스트 스위칭이 없는 시스템을 표현할 수 있다. 그런 시스템은 I/O 작업이 수행되는 동안 CPU 사용률이 0%로 방치되고 현대 운영체제가 제공하는 동시성을 전혀 제공하지 못할 것이다. 이 옵션이 있는 경우 프로세스의 실행 순서와 상관없이 같은 프로세스 집합은 언제나 같은 단위 시간 동안 수행된다.

## Problem 5

    이번에는 프로세스가 입출력을 기다릴 때마다 다른 프로세스로 전환하도록 플래그를 지정하여 같은 프로세스를 실행시켜 보자 (-l 1:0,4:100 -c -S SWITCH_ON_IO). 이제 어떤 결과가 발생하는가? -c를 사용하여 예측이 맞는지 확인하시오.

```bash
$ python3 process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1
```

다시 Problem 3과 같은 결과를 확인할 수 있다. 시뮬레이터는 현대 운영체제의 CPU 가상화 방식을 잘 반영하고 있기 때문에 Problem 4의 결과와 비교하여 CPU가 가능한 유휴 상태에 방치되지 않도록 하는 것이 얼마나 중요한 것인지 다시 한번 확인할 수 있다.

## Problem 6

    또 다른 중요한 행동은 입출력이 완료되었을 때 무엇을 하느냐이다. -I IO_RUN_LATER 가 지정되면 입출력이 완료되었을 때 입출력을 요청한 프로세스가 바로 실행될 필요가 없다. 완료 시점에 실행 중이던 프로세스가 계속 실행된다. 다음과 같은 조합의 프로세스를 실행시키면 무슨 결과가 나오는가? (./process-run.py -l 3:0, 5:100, 5:100, 5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p) 시스템 자원은 효과적으로 활용되는가?

```bash
$ python3 process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p
Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1
  2        BLOCKED       RUN:cpu         READY         READY             1             1
  3        BLOCKED       RUN:cpu         READY         READY             1             1
  4        BLOCKED       RUN:cpu         READY         READY             1             1
  5        BLOCKED       RUN:cpu         READY         READY             1             1
  6        BLOCKED       RUN:cpu         READY         READY             1             1
  7*         READY          DONE       RUN:cpu         READY             1
  8          READY          DONE       RUN:cpu         READY             1
  9          READY          DONE       RUN:cpu         READY             1
 10          READY          DONE       RUN:cpu         READY             1
 11          READY          DONE       RUN:cpu         READY             1
 12          READY          DONE          DONE       RUN:cpu             1
 13          READY          DONE          DONE       RUN:cpu             1
 14          READY          DONE          DONE       RUN:cpu             1
 15          READY          DONE          DONE       RUN:cpu             1
 16          READY          DONE          DONE       RUN:cpu             1
 17    RUN:io_done          DONE          DONE          DONE             1
 18         RUN:io          DONE          DONE          DONE             1
 19        BLOCKED          DONE          DONE          DONE                           1
 20        BLOCKED          DONE          DONE          DONE                           1
 21        BLOCKED          DONE          DONE          DONE                           1
 22        BLOCKED          DONE          DONE          DONE                           1
 23        BLOCKED          DONE          DONE          DONE                           1
 24*   RUN:io_done          DONE          DONE          DONE             1
 25         RUN:io          DONE          DONE          DONE             1
 26        BLOCKED          DONE          DONE          DONE                           1
 27        BLOCKED          DONE          DONE          DONE                           1
 28        BLOCKED          DONE          DONE          DONE                           1
 29        BLOCKED          DONE          DONE          DONE                           1
 30        BLOCKED          DONE          DONE          DONE                           1
 31*   RUN:io_done          DONE          DONE          DONE             1
```

이 시뮬레이터는 컨텍스트 스위칭의 비용까지는 시뮬레이션하지 않고 있다. 그러므로 `IO_RUN_LATER` 옵션을 사용하지 않아도 같은 단위 시간에 모든 프로세스의 실행이 완료될 것이다. 하지만 실제 하나의 CPU 코어만 있는 시스템에서 I/O 작업이 완료될 때마다 CPU 선점(preemption)이 발생한다고 생각해 보자.

장점으로는 CPU-bounded task가 장시간 실행되어야 하는 경우 그것들이 모두 마무리된 후 I/O 작업을 처리하는 것보다 중간에 선점 후 I/O 작업을 잠깐 처리하도록 허용함으로써 처리 시간(Turnaround time)이 단축될 수 있다. 그러므로 I/O-bounded task에게 서비스를 받는 사용자는 빠른 응답을 기대할 수 있다. 하지만 단점으로는 컨텍스트 스위칭으로 인한 오버헤드를 무시할 수 없기 때문에 어쩌면 시스템 전체적으로 장점이 상쇄될 정도의 오버헤드가 발생할 가능성이 있다.

그러므로 I/O 작업이 완료되었을 때 즉시 CPU를 선점할지에 관한 결정은 근본적으로 그 시스템에서 I/O 작업이 의미하는 바가 무엇인지를 생각하여 내려야 한다. 예를 들어 웹 서버가 있는 싱글 코어 시스템이라면 사용자에게 신속히 응답하기 위해 I/O 작업 완료 시 즉시 CPU를 선점해야 할 것이다.

## Problem 7

    같은 프로세스 조합을 실행시킬 때 -I IO_RUN_IMMEDIATE를 지정하고 실행시키시오. 이 플래그는 입출력이 완료되었을 때 요청 프로세스가 곧바로 실행되는 동작을 의미한다. 이 동작은 어떤 결과를 만들어 내는가? 방금 입출력을 완료한 프로세스를 다시 실행시키는 것이 좋은 생각일 수 있는 이유는 무엇인가?

Problem 6의 답변이 Problem 7에 대한 답변도 포함하고 있다.

## Problem 8

    이제 다음과 같이 무작위로 생성된 프로세스를 실행시켜 보자. 예를 들면, -s 1 -l 3:50,3:50, -s 2 -l 3:50,3:50, -s 3 -l 3:50,3:50. 어떤 양상을 보일지 예측할 수 있는지 생각해 보시오. -I IO_RUN_IMMEDIATE를 지정했을 때와 -I IO_RUN_LATER를 지정했을 때 어떤 결과가 나오는가? -S SWITCH_ON_IO 대 -S SWITCH_ON_END의 경우에는 어떤 결과가 나오는가?

앞서 정리한 것과 같이 `IO_RUN_IMMEDIATE` 옵션은 I/O 작업의 처리 시간을 단축함으로써 해당 작업의 결과에 의존하는 사용자가 빠른 응답을 얻게 할 수 있다는 장점이 있지만 시스템 전체적으로 컨텍스트 스위칭으로 인한 비용을 증가시킬 수 있다는 단점도 존재한다.

그리고 `SWITCH_ON_IO`는 기본적으로 현대 운영체제의 CPU 가상화 메커니즘을 활용하는 것이기 때문에 위 실험을 진행하지 않아도 `SWITCH_ON_END` 옵션과 비교했을 때 '우월하다'고 표현할 만하다.
