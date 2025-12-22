# OSTEP Chapter 20 Homework

## Problem 1

    먼저 무작위 할당과 해제를 생성하기 위하여 -n 10 -H 0 -p BEST -s 0 플래그를 주고 실행시켜라. alloc()과 free()가 무엇을 반환할지 예측할 수 있는가? 각 요청 후의 빈 공간 리스트의 상태를 추측할 수 있는가? 시간이 지남에 따라 빈 공간 리스트에 대해 무엇을 알 수 있는가?

**malloc.py output**

```bash
$ python3 malloc.py -n 10 -H 0 -p BEST -s 0 -c
seed 0
size 100
baseAddr 1000
headerSize 0
alignment -1
policy BEST
listOrder ADDRSORT
coalesce False
numOps 10
range 10
percentAlloc 50
allocList
compute True

ptr[0] = Alloc(3) returned 1000 (searched 1 elements)
Free List [ Size 1 ]: [ addr:1003 sz:97 ]

Free(ptr[0])
returned 0
Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1003 sz:97 ]

ptr[1] = Alloc(5) returned 1003 (searched 2 elements)
Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1008 sz:92 ]

Free(ptr[1])
returned 0
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:92 ]

ptr[2] = Alloc(8) returned 1008 (searched 3 elements)
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

Free(ptr[2])
returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[3] = Alloc(8) returned 1008 (searched 4 elements)
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

Free(ptr[3])
returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[4] = Alloc(2) returned 1000 (searched 4 elements)
Free List [ Size 4 ]: [ addr:1002 sz:1 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[5] = Alloc(7) returned 1008 (searched 4 elements)
Free List [ Size 4 ]: [ addr:1002 sz:1 ][ addr:1003 sz:5 ][ addr:1015 sz:1 ][ addr:1016 sz:84 ]
```

`alloc()` 함수는 할당된 공간의 시작 주소를 반환, `free()` 함수는 메모리 해제의 성공 여부를 반환한다. 현재 병합(Coalescing)이 설정되어 있지 않기 때문에 메모리가 할당되고 해제됨에 따라 각 청크의 크기는 작아지고 청크의 개수는 그만큼 늘어난다. 이에 따라 외부 단편화(External Fragmentation) 가능성이 올라간다.

## Problem 2

    빈 공간 리스트를 탐색하기 위하여 최악 적합 정책을 사용할 때 결과는 어떻게 달라지는가 (-p WORST)? 무엇이 바뀌는가?

**malloc.py output**

```bash
$ python3 malloc.py -n 10 -H 0 -p WORST -s 0 -c
seed 0
size 100
baseAddr 1000
headerSize 0
alignment -1
policy WORST
listOrder ADDRSORT
coalesce False
numOps 10
range 10
percentAlloc 50
allocList
compute True

ptr[0] = Alloc(3) returned 1000 (searched 1 elements)
Free List [ Size 1 ]: [ addr:1003 sz:97 ]

Free(ptr[0])
returned 0
Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1003 sz:97 ]

ptr[1] = Alloc(5) returned 1003 (searched 2 elements)
Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1008 sz:92 ]

Free(ptr[1])
returned 0
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:92 ]

ptr[2] = Alloc(8) returned 1008 (searched 3 elements)
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

Free(ptr[2])
returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[3] = Alloc(8) returned 1016 (searched 4 elements)
Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1024 sz:76 ]

Free(ptr[3])
returned 0
Free List [ Size 5 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:8 ][ addr:1024 sz:76 ]

ptr[4] = Alloc(2) returned 1024 (searched 5 elements)
Free List [ Size 5 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:8 ][ addr:1026 sz:74 ]

ptr[5] = Alloc(7) returned 1026 (searched 5 elements)
Free List [ Size 5 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:8 ][ addr:1033 sz:67 ]
```

최악 적합(worst-fit) 정책을 사용하면 빈 공간 리스트의 크기는 더 커진다. 왜냐하면 어떤 빈 공간을 분할할 필요 없이 재사용 가능한 상황에도 항상 가장 큰 빈 공간을 찾아 분할하기 때문이다. 하지만 최적 적합(best-fit) 정책에 비해 어느 정도 균등하게 메모리 공간이 사용된다는 것은 장점이라 할 수 있다. 만약 병합이 설정되어 있다면 나름 합리적인 정책일 수 있다.

## Problem 3

    최초 적합을 사용한 경우는 어떤가 (-p FIRST)? 최초 적합을 사용하면 속도 향상은 어떻게 되는가?

**malloc.py output**

```bash
$ python3 malloc.py -n 10 -H 0 -p FIRST -s 0 -c
seed 0
size 100
baseAddr 1000
headerSize 0
alignment -1
policy FIRST
listOrder ADDRSORT
coalesce False
numOps 10
range 10
percentAlloc 50
allocList
compute True

ptr[0] = Alloc(3) returned 1000 (searched 1 elements)
Free List [ Size 1 ]: [ addr:1003 sz:97 ]

Free(ptr[0])
returned 0
Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1003 sz:97 ]

ptr[1] = Alloc(5) returned 1003 (searched 2 elements)
Free List [ Size 2 ]: [ addr:1000 sz:3 ][ addr:1008 sz:92 ]

Free(ptr[1])
returned 0
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:92 ]

ptr[2] = Alloc(8) returned 1008 (searched 3 elements)
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

Free(ptr[2])
returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[3] = Alloc(8) returned 1008 (searched 3 elements)
Free List [ Size 3 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1016 sz:84 ]

Free(ptr[3])
returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[4] = Alloc(2) returned 1000 (searched 1 elements)
Free List [ Size 4 ]: [ addr:1002 sz:1 ][ addr:1003 sz:5 ][ addr:1008 sz:8 ][ addr:1016 sz:84 ]

ptr[5] = Alloc(7) returned 1008 (searched 3 elements)
Free List [ Size 4 ]: [ addr:1002 sz:1 ][ addr:1003 sz:5 ][ addr:1015 sz:1 ][ addr:1016 sz:84 ]
```

최초 적합(first-fit) 정책을 사용하면 빈 공간 리스트를 탐색하는 시간 복잡도가 최선의 경우 $O(1)$ 이다. 이는 언제나 $O(N)$ 의 시간 복잡도를 갖는 나머지 정책들의 할당 방식보다 성능상 압도적인 이점이 있다.

## Problem 4

    위 질문에 대해, 몇몇 정책의 경우 리스트의 정렬 순서가 빈 위치를 찾는 데 걸리는 시간에 영향을 준다. 다른 빈 공간 리스트의 순서 (-l ADDRSORT, -l SIZESORT+, -l SIZESORT-)를 사용하여 정책과 리스트 순서가 서로 어떤 영향을 주는지 관찰하라.

만약 빈 공간 리스트가 특정 기준으로 정렬되어 있다면 최초 적합(first-fit) 정책이란 곧 최적 적합(best-fit) 또는 최악 적합(worst-fit)을 의미하는 것일 수 있다. 예를 들어 빈 공간의 크기에 대해 오름차순으로 정렬된 리스트는 처음부터 순회하면서 가장 처음 발견한 적합 공간이 곧 최적 적합(best-fit)이 된다. 반대로 빈 공간의 크기에 대해 내림차순으로 정렬된 리스트는 가장 처음 발견한 적합 공간이 곧 최악 적합(worst-fit)이 된다. 주소를 기준으로 정렬하는 것은 병합이 적용될 경우 병합을 더 자주 발생하게 하여 외부 단편화의 가능성을 낮춘다.

하지만 빈 공간 리스트에 정렬 기준을 추가하는 것은 `alloc()` 함수의 최선의 시간 복잡도를 $O(1)$ 으로 만들어주는 대신 `free()` 함수의 최악의 시간 복잡도를 $O(N)$ 으로 만들게 된다. 그러므로 이러한 결정은 힙 메모리의 할당/해제 패턴을 고려하여 내려야 한다.

## Problem 5

     빈 공간의 합병은 매우 중요할 수 있다. 무작위 할당의 발수를 늘려라 (-n 1000). 시간이 지날수록 요청의 개수가 많아지면 어떤 현상이 발생하는가? 병합이 있는 채로 그리고 없는 채로 실행하라 (-C 플래그가 있거나 있거나). 출력에 어떤 차이를 볼 수 있는가? 각각의 경우의 시간이 지남에 따라 빈 공간 리스트의 크기는 얼마나 커지는가? 이 경우 리스트의 정렬 순서가 중요한가?

병합이 없는 경우 외부 단편화는 필연적으로 발생한다. 언젠가 크기가 1인 공간에 대한 요청 빼고는 어떤 요청도 처리할 수 없게 된다. 충분히 큰 시행 횟수에 대해 빈 공간 리스트의 원소 개수는 주소 값의 크기만큼이 될 것이다. 병합이 없는 경우 리스트의 정렬 순서는 중요하지 않고, 병합이 있는 경우엔 앞서 4번 문제에서 기술했듯 빈 공간 리스트를 주소 기준으로 정렬하는 것이 병합 발생 가능성을 더 높이는 선택이 될 수 있다.

## Problem 6

    할당 비율 -P를 50 이상으로 변경하면 어떤 일이 일어날까? 100에 가까울수록 할당에는 어떤 일이 생기는가? 0에 가까워진다면?

`alloc()` 함수의 비율이 100에 가까워질수록 `free()` 함수보다는 `alloc()` 함수의 효율을 생각하고 병합이 발생하지 않는 상황을 가정하는 것이 좋다. 그러므로 이 경우 최적 적합 정책이라면 빈 공간 리스트를 빈 공간 크기에 대해 오름차순 정렬, 최악 적합 정책이라면 내림차순 정렬하겠다.

반대로 `alloc()` 함수의 비율이 0에 가까워질수록 `free()` 함수의 효율을 생각하는 것이 좋다. 이 경우에는 빈 공간 리스트를 어떤 기준으로도 정렬하지 않고 항상 헤더에 원소를 추가하는 것이 좋을 수 있다. 병합이 있다면 빈 공간 리스트의 크기도 거의 1을 유지할 것이므로 빈 공간 할당에 있어 어떤 정책을 쓰든 큰 성능의 차이나 효율의 차이는 없을 것이며 외부 단편화 문제도 발생하지 않을 가능성이 높다. 하지만 이는 매우 비현실적인 가정이다.

## Problem 7

    심하게 단편화된 빈 공간을 만들어 내려면 어떤 종류의 요구를 생성할 수 있는가? 단편화된 리스트를 생성하기 위하여 -A 플래그를 사용하라. 다른 정책과 옵션이 빈 공간 리스트의 구조에 어떤 영향을 주는지 관찰하라.

**malloc.py output**

```bash
$ python3 malloc.py -S 10 -H 0 -p FIRST -A +9,-0,+8,-0,+7,-0,+6,-0,+5,-0,+4,-0,+3,-0,+2,-0,+1,-0 -c
seed 0
size 10
baseAddr 1000
headerSize 0
alignment -1
policy FIRST
listOrder ADDRSORT
coalesce False
numOps 10
range 10
percentAlloc 50
allocList +9,-0,+8,-0,+7,-0,+6,-0,+5,-0,+4,-0,+3,-0,+2,-0,+1,-0
compute True

ptr[0] = Alloc(9) returned 1000 (searched 1 elements)
Free List [ Size 1 ]: [ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 2 ]: [ addr:1000 sz:9 ][ addr:1009 sz:1 ]

ptr[1] = Alloc(8) returned 1000 (searched 1 elements)
Free List [ Size 2 ]: [ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 3 ]: [ addr:1000 sz:8 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[2] = Alloc(7) returned 1000 (searched 1 elements)
Free List [ Size 3 ]: [ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 4 ]: [ addr:1000 sz:7 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[3] = Alloc(6) returned 1000 (searched 1 elements)
Free List [ Size 4 ]: [ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 5 ]: [ addr:1000 sz:6 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[4] = Alloc(5) returned 1000 (searched 1 elements)
Free List [ Size 5 ]: [ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 6 ]: [ addr:1000 sz:5 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[5] = Alloc(4) returned 1000 (searched 1 elements)
Free List [ Size 6 ]: [ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 7 ]: [ addr:1000 sz:4 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[6] = Alloc(3) returned 1000 (searched 1 elements)
Free List [ Size 7 ]: [ addr:1003 sz:1 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 8 ]: [ addr:1000 sz:3 ][ addr:1003 sz:1 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[7] = Alloc(2) returned 1000 (searched 1 elements)
Free List [ Size 8 ]: [ addr:1002 sz:1 ][ addr:1003 sz:1 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 9 ]: [ addr:1000 sz:2 ][ addr:1002 sz:1 ][ addr:1003 sz:1 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

ptr[8] = Alloc(1) returned 1000 (searched 1 elements)
Free List [ Size 9 ]: [ addr:1001 sz:1 ][ addr:1002 sz:1 ][ addr:1003 sz:1 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]

Free(ptr[0])
returned 0
Free List [ Size 10 ]: [ addr:1000 sz:1 ][ addr:1001 sz:1 ][ addr:1002 sz:1 ][ addr:1003 sz:1 ][ addr:1004 sz:1 ][ addr:1005 sz:1 ][ addr:1006 sz:1 ][ addr:1007 sz:1 ][ addr:1008 sz:1 ][ addr:1009 sz:1 ]
```

위 출력은 빈 공간 할당 정책보다는 병합 메커니즘의 부재가 외부 단편화 발생에 얼마나 큰 영향을 미칠 수 있는지를 단적으로 보여주는 할당 및 해제 패턴이다. 이 외 현실적으로 외부 단편화는 공간이 연속적으로 할당되어 있지 않고 분산되어 할당되는 경우 발생 가능성이 올라간다. 그러므로 메모리 사용 패턴이 중요한데, 주소 순으로 할당하자니 메모리를 할당받은 객체마다 해당 메모리의 점유 시간이 상이하기 때문에 언제나 유리한 방법은 아니다. 최적 적합 정책도 메모리 사용 패턴에 따라 언제든 메모리가 분산되어 할당될 수 있기 때문에 이 문제에 대한 최적의 답은 없다.
