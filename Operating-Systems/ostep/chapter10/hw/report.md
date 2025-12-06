# OSTEP Chapter 10 Homework

## Problem 1

    길이가 200인 세 개의 작업을 SJF와 FIFO 스케줄링 방식으로 실행할 경우 응답 시간과 반환 시간을 계산하시오.

세 개의 작업 모두 동일한 시간에 도착한다고 가정한다.

- **`SJF`**
  - **응답 시간**: $(0 + 200 + 400) / 3 = 200$
  - **반환 시간**: $(200 + 400 + 600) / 3 = 400$
- **`FIFO`**
  - **응답 시간**: $(0 + 200 + 400) / 3 = 200$
  - **반환 시간**: $(200 + 400 + 600) / 3 = 400$

## Problem 2

    같은 조건이지만 작업의 길이가 각각 100, 200 및 300일 경우에 대해 계산하시오.

- **`SJF`**
  - **응답 시간**: $(0 + 100 + 300) / 3 = 133.3$
  - **반환 시간**: $(100 + 300 + 600) / 3 = 333.3$
- **`FIFO`**
  - **응답 시간**: $(0 + 100 + 300) / 3 = 133.3$
  - **반환 시간**: $(100 + 300 + 600) / 3 = 333.3$

## Problem 3

    2번과 같은 조건으로 타임 슬라이스가 1인 RR 스케줄러에 대해서도 계산하시오.

- **응답 시간**: $(0 + 1 + 2) / 3 = 1$
- **반환 시간**: $(298 + 499 + 600) / 3 = 465.6$

## Problem 4

    SJF와 FIFO가 같은 반환 시간을 보이는 워크로드의 유형은 무엇인가?

모든 작업이 같은 실행 시간을 가지거나 실행 시간에 대해 오름차순으로 준비 큐에 등록되어 디스패치될 경우 SJF, FIFO 스케줄링 알고리즘이 같은 반환 시간을 보이게 된다.

## Problem 5

    SJF가 RR과 같은 응답 시간을 보이기 위한 워크로드와 타임 퀀텀의 길이는 무엇인가?

SJF는 비선점형 스케줄링 알고리즘, RR은 선점형 스케줄링 알고리즘이기 때문에 두 알고리즘이 같은 응답 시간을 보이기 위해서는 모든 작업이 타임 퀀텀보다 짧은 실행 시간을 가지거나, 타임 퀀텀이 작업들의 실행 시간의 최대 값 이상이어야 한다. 그러므로 이 조건은 선점형 알고리즘이 사실상 비선점형 알고리즘처럼 동작하는 조건이다.

## Problem 6

    작업의 길이가 증가하면 SJF의 응답 시간은 어떻게 되는가? 변화의 추이를 보이기 위해서 시뮬레이터를 사용할 수 있는가?

**scheduler.py output**

```bash
$ ./scheduler.py -p SJF -j 3 -l 100,100,100 -c
ARG policy SJF
ARG jlist 100,100,100

Here is the job list, with the run time of each job:
  Job 0 ( length = 100.0 )
  Job 1 ( length = 100.0 )
  Job 2 ( length = 100.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 100.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 2 for 100.00 secs ( DONE at 300.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 200.00  Wait 100.00
  Job   2 -- Response: 200.00  Turnaround 300.00  Wait 200.00

  Average -- Response: 100.00  Turnaround 200.00  Wait 100.00

$ ./scheduler.py -p SJF -j 3 -l 1000,1000,1000 -c
ARG policy SJF
ARG jlist 1000,1000,1000

Here is the job list, with the run time of each job:
  Job 0 ( length = 1000.0 )
  Job 1 ( length = 1000.0 )
  Job 2 ( length = 1000.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 1000.00 secs ( DONE at 1000.00 )
  [ time 1000 ] Run job 1 for 1000.00 secs ( DONE at 2000.00 )
  [ time 2000 ] Run job 2 for 1000.00 secs ( DONE at 3000.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 1000.00  Wait 0.00
  Job   1 -- Response: 1000.00  Turnaround 2000.00  Wait 1000.00
  Job   2 -- Response: 2000.00  Turnaround 3000.00  Wait 2000.00

  Average -- Response: 1000.00  Turnaround 2000.00  Wait 1000.00
```

SJF은 비선점형 알고리즘이기 때문에 앞서 실행되는 작업의 실행 시간이 평균 응답 시간에 큰 영향을 미친다. 실행 시간이 긴 작업들은 그보다 실행 시간이 짧은 나머지 작업들의 실행 시간의 합만큼을 대기해야 하기 때문이다. 이는 기아(Starvation) 문제와도 연관된다.

## Problem 7

    타임 퀀텀의 길이가 증가하면 RR의 응답 시간은 어떻게 되는가? N개의 작업이 주어졌을 때, 최악의 응답 시간을 계산하는 식을 만들 수 있는가?

RR 알고리즘에서 타임 퀀텀의 길이가 증가함에 따라 일반적으로 평균 응답 시간도 증가하게 되지만 응답 시간과 상충 관계에 있는 평균 반환 시간은 감소할 수 있다. FIFO 알고리즘은 타임 퀀텀이 무한대인 RR 알고리즘과 언제나 같은 성능 지표를 보인다.
