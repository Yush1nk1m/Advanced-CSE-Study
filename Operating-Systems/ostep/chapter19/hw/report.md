# OSTEP Chapter 19 Homework

**segmentation.py**는 가상 주소 공간에 두 개의 세그먼트만 존재하는 단순한 상황을 가정한다. 여기엔 양의 방향으로 주소가 확장되는 세그먼트, 스택 세그먼트처럼 음의 방향으로 주소가 확장되는 세그먼트만 존재한다. 그러므로 전자의 세그먼트는 가상 주소 공간의 맨 앞부터, 후자의 세그먼트는 맨 뒤부터 확장되기 시작한다. 두 세그먼트를 구분하기 위해 최상위 비트 1개를 사용한다. 이는 두 세그먼트의 최대 크기가 같다는 의미이기도 하다.

## Problem 1

    우선 몇 주소를 변환하기 위해 작은 주소 공간을 사용하여 보자. 여기에 여러 가지 랜덤 시드와 함께 간단한 매개변수의 집합이 있다. 이때 주소를 변환할 수 있을까?

**segmentation.py output**

```bash
$ python3 segmentation.py -c
ARG seed 0
ARG address space size 1k
ARG phys mem size 16k

Segment register information:

  Segment 0 base  (grows positive) : 0x00001aea (decimal 6890)
  Segment 0 limit                  : 472

  Segment 1 base  (grows negative) : 0x00001254 (decimal 4692)
  Segment 1 limit                  : 450

Virtual Address Trace
  VA  0: 0x0000020b (decimal:  523) --> SEGMENTATION VIOLATION (SEG1)
  VA  1: 0x0000019e (decimal:  414) --> VALID in SEG0: 0x00001c88 (decimal: 7304)
  VA  2: 0x00000322 (decimal:  802) --> VALID in SEG1: 0x00001176 (decimal: 4470)
  VA  3: 0x00000136 (decimal:  310) --> VALID in SEG0: 0x00001c20 (decimal: 7200)
  VA  4: 0x000001e8 (decimal:  488) --> SEGMENTATION VIOLATION (SEG0)


```

가상 주소 공간의 크기는 1KB(1,024B)이다. 세그먼트 0의 경우 $[0, 472)$ 의 할당 공간을 가지며 세그먼트 1의 경우 $(573, 1023]$ 의 할당 공간을 가진다. 그러므로 $[472, 573]$ 에 포함되는 주소는 세그먼트 위반을 유발하며, $[472, 511]$ 범위에 포함되는 참조는 세그먼트 0의 범위를 위반, $[512, 573]$ 범위에 포함되는 참조는 세그먼트 1의 범위를 위반하는 것이 된다.

물리 주소로의 변환은 세그먼트 0 범위에 포함되는 참조는 베이스 레지스터 값에 참조 주소를 더함으로써, 세그먼트 1 범위에 포함되는 참조는 주소 값에 가상 주소 공간의 최대 크기인 1,024(B)를 뺀 후 베이스 레지스터 값에 더함으로써 가능하다.

## Problem 2

    이제 우리가 구축한 이 작은 주소 공간을 이해하고 있는지 알아보자 (위의 질문에서 사용한 매개변수를 사용하여). 세그멘트 0의 합법적인 가상 주소의 최댓값은 무엇인가? 세그멘트 1의 합법적인 가상 주소의 최저값은 무엇인가? 이 전체 주소 공간의 잘못된 주소 중 가장 큰 주소와 가장 작은 주소는 무엇인가? 마지막으로, 플래그 -A와 함께 실행한 segmentation.py의 정확성을 어떻게 확인할 수 있을까?

적법/위법한 주소의 범위는 1번 문제에서 설명했다. 정확성이라는 표현이 애매하므로 $[0, 1023]$ 범위에서 생성된 임의의 주소 값이 적법할 확률이라고 정의하겠다. 이는 $\text({(limit 1)} + \text{(limit 2)}) \div \text{(Size of VA)}$ 과 같다. 즉, 두 세그먼트가 할당된 공간이 전체 가상 주소 공간에서 차지하는 비율과 같다.

## Problem 3

    16바이트 주소 공간과 128바이트의 물리 메모리가 있다고 하자. 시뮬레이터가 지정된 주소 스트림에 대해 다음과 같은 변환 결과를 생성하기 위해서는 베이스와 바운드를 어떤 값으로 설정해야 하는가 : 유효, 유효, 위반, ..., 위반, 유효, 유효? 다음과 같은 매개변수를 가정한다.

    segmentation.py −a 16 −p 128
        −A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
        −−b0 ? −−l0 ? −−b1 ? −−l1 ?

- `b0`: 0
- `l0`: 2
- `b1`: 15
- `l1`: 2

**segmentation.py output**

```bash
$ python3 segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 2 --b1 15 --l1 2 -c
ARG seed 0
ARG address space size 16
ARG phys mem size 128

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 2

  Segment 1 base  (grows negative) : 0x0000000f (decimal 15)
  Segment 1 limit                  : 2

Virtual Address Trace
  VA  0: 0x00000000 (decimal:    0) --> VALID in SEG0: 0x00000000 (decimal:    0)
  VA  1: 0x00000001 (decimal:    1) --> VALID in SEG0: 0x00000001 (decimal:    1)
  VA  2: 0x00000002 (decimal:    2) --> SEGMENTATION VIOLATION (SEG0)
  VA  3: 0x00000003 (decimal:    3) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x00000004 (decimal:    4) --> SEGMENTATION VIOLATION (SEG0)
  VA  5: 0x00000005 (decimal:    5) --> SEGMENTATION VIOLATION (SEG0)
  VA  6: 0x00000006 (decimal:    6) --> SEGMENTATION VIOLATION (SEG0)
  VA  7: 0x00000007 (decimal:    7) --> SEGMENTATION VIOLATION (SEG0)
  VA  8: 0x00000008 (decimal:    8) --> SEGMENTATION VIOLATION (SEG1)
  VA  9: 0x00000009 (decimal:    9) --> SEGMENTATION VIOLATION (SEG1)
  VA 10: 0x0000000a (decimal:   10) --> SEGMENTATION VIOLATION (SEG1)
  VA 11: 0x0000000b (decimal:   11) --> SEGMENTATION VIOLATION (SEG1)
  VA 12: 0x0000000c (decimal:   12) --> SEGMENTATION VIOLATION (SEG1)
  VA 13: 0x0000000d (decimal:   13) --> SEGMENTATION VIOLATION (SEG1)
  VA 14: 0x0000000e (decimal:   14) --> VALID in SEG1: 0x0000000d (decimal:   13)
  VA 15: 0x0000000f (decimal:   15) --> VALID in SEG1: 0x0000000e (decimal:   14)
```

## Problem 4

    무작위로 생성된 가상 주소 중 약 90%가 유효한 시스템에서 (즉, 세그멘테이션 위반이 아닌) 문제를 만들고 싶다고 가정하자. 그런 결과를 내려면 시뮬레이터를 어떻게 설정해야 하는가? 어떤 매개변수가 중요한가?

`l0`, `l1` 옵션이 가장 중요하다. 앞서 2번 문제의 답변에서 서술했듯 임의로 생성된 가상 주소의 정확성은 전체 가상 주소 공간 중 사용되고 있는 공간이 차지하고 있는 비율에 의해 결정된다. 그러므로 `l0`, `l1`을 합한 크기가 전체 가상 주소 공간 크기의 90% 이상이 되어야 한다.

## Problem 5

    모든 가상 주소가 유효하지 않도록 시뮬레이터를 실행할 수 있는가? 어떻게?

`l0`, `l1` 값을 0으로 설정하면 임의로 생성된 가상 주소는 100% 위법한 주소가 된다. 이 역시 2번 문제에서 서술한 정확성에 대한 수식으로 계산할 수 있다.
