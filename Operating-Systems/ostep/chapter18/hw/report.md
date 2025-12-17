# OSTEP Chapter 18 Homework

## Problem 1

    시드 1, 2 및 3을 가지고 실행하고 프로세스에 의해 생성된 각 가상 주소가 범위 내에 있는지 계산하라. 바운드 안에 있다면 주소 변환을 수행하라.

**랜덤 시드 1, 2, 3에 대한 계산**

```bash
$ python3 relocation.py -s 1 -c

ARG seed 1
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x0000363c (decimal 13884)
  Limit  : 290

Virtual Address Trace
  VA  0: 0x0000030e (decimal:  782) --> SEGMENTATION VIOLATION
  VA  1: 0x00000105 (decimal:  261) --> VALID: 0x00003741 (decimal: 14145)
  VA  2: 0x000001fb (decimal:  507) --> SEGMENTATION VIOLATION
  VA  3: 0x000001cc (decimal:  460) --> SEGMENTATION VIOLATION
  VA  4: 0x0000029b (decimal:  667) --> SEGMENTATION VIOLATION

$ python3 relocation.py -s 2 -c

ARG seed 2
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00003ca9 (decimal 15529)
  Limit  : 500

Virtual Address Trace
  VA  0: 0x00000039 (decimal:   57) --> VALID: 0x00003ce2 (decimal: 15586)
  VA  1: 0x00000056 (decimal:   86) --> VALID: 0x00003cff (decimal: 15615)
  VA  2: 0x00000357 (decimal:  855) --> SEGMENTATION VIOLATION
  VA  3: 0x000002f1 (decimal:  753) --> SEGMENTATION VIOLATION
  VA  4: 0x000002ad (decimal:  685) --> SEGMENTATION VIOLATION

$ python3 relocation.py -s 3 -c

ARG seed 3
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x000022d4 (decimal 8916)
  Limit  : 316

Virtual Address Trace
  VA  0: 0x0000017a (decimal:  378) --> SEGMENTATION VIOLATION
  VA  1: 0x0000026a (decimal:  618) --> SEGMENTATION VIOLATION
  VA  2: 0x00000280 (decimal:  640) --> SEGMENTATION VIOLATION
  VA  3: 0x00000043 (decimal:   67) --> VALID: 0x00002317 (decimal: 8983)
  VA  4: 0x0000000d (decimal:   13) --> VALID: 0x000022e1 (decimal: 8929)
```

`Limit`(바운드 레지스터 값) 범위 안에 있는 가상 주소가 유효하고, `Base`(베이스 레지스터 값) 값을 가상 주소에 더해주면 물리 메모리 주소로 변환된다.

## Problem 2

    다음과 같은 플래그를 주고 실행하라 : -s 0 -n 10. 생성된 모든 가상 주소가 범위 안에 있는 것을 보장하기 위해서는 -l을 어떤 값으로 설정해야 하는가?

**옵션 -s 0 -n 10으로 실행**

```bash
$ python3 relocation.py -s 0 -n 10

ARG seed 0
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x000001ae (decimal:  430) --> PA or segmentation violation?
  VA  1: 0x00000109 (decimal:  265) --> PA or segmentation violation?
  VA  2: 0x0000020b (decimal:  523) --> PA or segmentation violation?
  VA  3: 0x0000019e (decimal:  414) --> PA or segmentation violation?
  VA  4: 0x00000322 (decimal:  802) --> PA or segmentation violation?
  VA  5: 0x00000136 (decimal:  310) --> PA or segmentation violation?
  VA  6: 0x000001e8 (decimal:  488) --> PA or segmentation violation?
  VA  7: 0x00000255 (decimal:  597) --> PA or segmentation violation?
  VA  8: 0x000003a1 (decimal:  929) --> PA or segmentation violation?
  VA  9: 0x00000204 (decimal:  516) --> PA or segmentation violation?

For each virtual address, either write down the physical address it translates to
OR write down that it is an out-of-bounds address (a segmentation violation). For
this problem, you should assume a simple virtual address space of a given size.
```

가상 주소의 최댓값은 0x000003a1$_{(2)}$(929$_{(10)}$)이다. 그러므로 바운드 레지스터의 값도 최소 930$_{(10)}$이어야 segmentation violation이 발생하지 않을 것이다.

## Problem 3

    다음과 같은 플래그를 주고 실행하라 : -s 1 -n 10 -l 100. 주소 공간 전체가 여전히 물리 메모리에 들어가려면 설정할 수 있는 바운드 레지스터의 최댓값은 얼마인가?

**옵션 -s 1 -n 10 -l 100으로 실행**

```bash
$ python3 relocation.py -s 1 -n 10 -l 100

ARG seed 1
ARG address space size 1k
ARG phys mem size 16k

Base-and-Bounds register information:

  Base   : 0x00000899 (decimal 2201)
  Limit  : 100

Virtual Address Trace
  VA  0: 0x00000363 (decimal:  867) --> PA or segmentation violation?
  VA  1: 0x0000030e (decimal:  782) --> PA or segmentation violation?
  VA  2: 0x00000105 (decimal:  261) --> PA or segmentation violation?
  VA  3: 0x000001fb (decimal:  507) --> PA or segmentation violation?
  VA  4: 0x000001cc (decimal:  460) --> PA or segmentation violation?
  VA  5: 0x0000029b (decimal:  667) --> PA or segmentation violation?
  VA  6: 0x00000327 (decimal:  807) --> PA or segmentation violation?
  VA  7: 0x00000060 (decimal:   96) --> PA or segmentation violation?
  VA  8: 0x0000001d (decimal:   29) --> PA or segmentation violation?
  VA  9: 0x00000357 (decimal:  855) --> PA or segmentation violation?

For each virtual address, either write down the physical address it translates to
OR write down that it is an out-of-bounds address (a segmentation violation). For
this problem, you should assume a simple virtual address space of a given size.
```

가상 주소 공간의 크기가 별도로 정의되어 있지 않다고 가정하자. 물리 메모리의 크기는 16KB이므로 $(\text{Base}) + (\text{VA}) < 16384$를 만족해야 한다. 베이스 레지스터의 값이 2,201이므로 물리 메모리의 경계를 넘지 않기 위한 바운드 레지스터의 최댓값은 14,183이다.

## Problem 4

    더 큰 주소 공간(-a)과 물리 메모리(-p)를 설정하여 위 문제와 동일하게 실행시켜 보아라.

**옵션 -s 1 -n 10 -a 16k -p 32k -c으로 실행**

```bash
$ python3 relocation.py -s 1 -n 10 -a 16k -p 32k -c

ARG seed 1
ARG address space size 16k
ARG phys mem size 32k

Base-and-Bounds register information:

  Base   : 0x00006c78 (decimal 27768)
  Limit  : 4646

Virtual Address Trace
  VA  0: 0x000030e1 (decimal: 12513) --> SEGMENTATION VIOLATION
  VA  1: 0x00001053 (decimal: 4179) --> VALID: 0x00007ccb (decimal: 31947)
  VA  2: 0x00001fb5 (decimal: 8117) --> SEGMENTATION VIOLATION
  VA  3: 0x00001cc4 (decimal: 7364) --> SEGMENTATION VIOLATION
  VA  4: 0x000029b3 (decimal: 10675) --> SEGMENTATION VIOLATION
  VA  5: 0x0000327a (decimal: 12922) --> SEGMENTATION VIOLATION
  VA  6: 0x00000601 (decimal: 1537) --> VALID: 0x00007279 (decimal: 29305)
  VA  7: 0x000001d0 (decimal:  464) --> VALID: 0x00006e48 (decimal: 28232)
  VA  8: 0x0000357d (decimal: 13693) --> SEGMENTATION VIOLATION
  VA  9: 0x00001bb2 (decimal: 7090) --> SEGMENTATION VIOLATION
```

이 경우에는 프로세스의 가상 주소 공간을 모두 동시에 물리 메모리에 적재하는 것이 불가능하다(데이터가 선형적으로 저장된다는 가정). 왜냐하면 베이스 레지스터 값으로부터 선형으로 16KB만큼 떨어진 공간은 물리 메모리의 경계 밖에 있다.

이 경우 바운드 레지스터의 최댓값은 5,000이고 이를 통해 프로그램을 물리 메모리에 선형적으로 저장하는 것은 매우 비효율적이며 공간을 온전히 활용하지 못하게 될 수 있다는 것을 알 수 있다. 그렇다고 이를 방지하기 위해 프로그램의 가상 주소 공간 크기만큼 베이스 레지스터의 값을 작게 할당하기에는 메모리 단편화 문제가 생기거나 큰 가상 주소 공간을 가진 프로그램은 적재가 불가능할 수 있다.

그래서 현대 운영체제는 프로그램을 파편화하여 물리적 메모리에 분산하여 적재하고 필요한 부분만 적재하는 기술을 사용한다. 주소 공간을 논리적 단위(코드, 데이터, 스택)로 나누어 각각 베이스/바운드 쌍을 할당하는 세그멘테이션(Segmentation), 주소 공간을 고정된 크기의 페이지(Page)로 나누어 물리 메모리의 빈 프레임(Frame)에 분산 저장하는 페이징(Paging)이 이에 해당한다.

## Problem 5

    바운드 레지스터의 값이 변함에 따라 임의로 생성된 가상 주소 중 유효한 주소의 비율은 얼마인가? 다른 랜덤 시드를 가지고 실행한 결과를 그래프로 나타내시오. 값의 범위는 0부터 주소 공간의 최대 크기로 한다.

모든 변수가 무작위하게 생성된다고 가정하면 평균적으로 $((\text{Bound}) - (\text{Base})) \div (\text{Virtual Address Space})$ 의 확률로 임의로 생성된 가상 주소가 유효한 주소로 판별될 것이다.
