# OSTEP Chapter 24 Homework

## Problem 1

    다음 인자를 주고 랜덤 주소를 생성하라 : -s 0 -n 10, -s 1 -n 10, 그리고 -s 2 -n 10. FIFO 정책에서 LRU 정책으로 그리고 OPT 정책으로 변경하라. 생성된 주소 흐름의 모든 주소에 대해 히트인지 미스인지를 판별하라.

**paging-policy.py output**

```bash
# -s 0 -p FIFO
Access: 8  MISS FirstIn ->          [8] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 7  MISS FirstIn ->       [8, 7] <- Lastin  Replaced:- [Hits:0 Misses:2]
Access: 4  MISS FirstIn ->    [8, 7, 4] <- Lastin  Replaced:- [Hits:0 Misses:3]
Access: 2  MISS FirstIn ->    [7, 4, 2] <- Lastin  Replaced:8 [Hits:0 Misses:4]
Access: 5  MISS FirstIn ->    [4, 2, 5] <- Lastin  Replaced:7 [Hits:0 Misses:5]
Access: 4  HIT  FirstIn ->    [4, 2, 5] <- Lastin  Replaced:- [Hits:1 Misses:5]
Access: 7  MISS FirstIn ->    [2, 5, 7] <- Lastin  Replaced:4 [Hits:1 Misses:6]
Access: 3  MISS FirstIn ->    [5, 7, 3] <- Lastin  Replaced:2 [Hits:1 Misses:7]
Access: 4  MISS FirstIn ->    [7, 3, 4] <- Lastin  Replaced:5 [Hits:1 Misses:8]
Access: 5  MISS FirstIn ->    [3, 4, 5] <- Lastin  Replaced:7 [Hits:1 Misses:9]

# -s 0 -p LRU
Access: 8  MISS LRU ->          [8] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 7  MISS LRU ->       [8, 7] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 4  MISS LRU ->    [8, 7, 4] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 2  MISS LRU ->    [7, 4, 2] <- MRU Replaced:8 [Hits:0 Misses:4]
Access: 5  MISS LRU ->    [4, 2, 5] <- MRU Replaced:7 [Hits:0 Misses:5]
Access: 4  HIT  LRU ->    [2, 5, 4] <- MRU Replaced:- [Hits:1 Misses:5]
Access: 7  MISS LRU ->    [5, 4, 7] <- MRU Replaced:2 [Hits:1 Misses:6]
Access: 3  MISS LRU ->    [4, 7, 3] <- MRU Replaced:5 [Hits:1 Misses:7]
Access: 4  HIT  LRU ->    [7, 3, 4] <- MRU Replaced:- [Hits:2 Misses:7]
Access: 5  MISS LRU ->    [3, 4, 5] <- MRU Replaced:7 [Hits:2 Misses:8]

# -s 0 -p OPT
Access: 8  MISS Left  ->          [8] <- Right Replaced:- [Hits:0 Misses:1]
Access: 7  MISS Left  ->       [8, 7] <- Right Replaced:- [Hits:0 Misses:2]
Access: 4  MISS Left  ->    [8, 7, 4] <- Right Replaced:- [Hits:0 Misses:3]
Access: 2  MISS Left  ->    [7, 4, 2] <- Right Replaced:8 [Hits:0 Misses:4]
Access: 5  MISS Left  ->    [7, 4, 5] <- Right Replaced:2 [Hits:0 Misses:5]
Access: 4  HIT  Left  ->    [7, 4, 5] <- Right Replaced:- [Hits:1 Misses:5]
Access: 7  HIT  Left  ->    [7, 4, 5] <- Right Replaced:- [Hits:2 Misses:5]
Access: 3  MISS Left  ->    [4, 5, 3] <- Right Replaced:7 [Hits:2 Misses:6]
Access: 4  HIT  Left  ->    [4, 5, 3] <- Right Replaced:- [Hits:3 Misses:6]
Access: 5  HIT  Left  ->    [4, 5, 3] <- Right Replaced:- [Hits:4 Misses:6]
```

나머지 시드에 대한 결과 및 계산 과정은 생략한다.

## Problem 2

    캐시 크기가 5인 경우에 대해서 FIFO, LRU 그리고 MRU 정책을 사용할 때 최악의 주소 참조 스트림을 생성하라 (최악의 참조 스트림은 가능한 최대 개수의 미스를 발생시킨다). 최악의 참조 스트림의 경우 성능을 극적으로 향상시켜 OPT의 성능에 근접하게 만들려면 캐시의 크기는 얼마나 커야 하는가?

**FIFO 정책의 최악의 주소 참조 스트림**

```bash
$ python paging-policy.py -a 1,2,3,4,5,6,1,2,3,4,5,6 -C 5 -c
ARG addresses 1,2,3,4,5,6,1,2,3,4,5,6
ARG addressfile
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS FirstIn ->          [1] <- Lastin  Replaced:- [Hits:0 Misses:1]
Access: 2  MISS FirstIn ->       [1, 2] <- Lastin  Replaced:- [Hits:0 Misses:2]
Access: 3  MISS FirstIn ->    [1, 2, 3] <- Lastin  Replaced:- [Hits:0 Misses:3]
Access: 4  MISS FirstIn -> [1, 2, 3, 4] <- Lastin  Replaced:- [Hits:0 Misses:4]
Access: 5  MISS FirstIn -> [1, 2, 3, 4, 5] <- Lastin  Replaced:- [Hits:0 Misses:5]
Access: 6  MISS FirstIn -> [2, 3, 4, 5, 6] <- Lastin  Replaced:1 [Hits:0 Misses:6]
Access: 1  MISS FirstIn -> [3, 4, 5, 6, 1] <- Lastin  Replaced:2 [Hits:0 Misses:7]
Access: 2  MISS FirstIn -> [4, 5, 6, 1, 2] <- Lastin  Replaced:3 [Hits:0 Misses:8]
Access: 3  MISS FirstIn -> [5, 6, 1, 2, 3] <- Lastin  Replaced:4 [Hits:0 Misses:9]
Access: 4  MISS FirstIn -> [6, 1, 2, 3, 4] <- Lastin  Replaced:5 [Hits:0 Misses:10]
Access: 5  MISS FirstIn -> [1, 2, 3, 4, 5] <- Lastin  Replaced:6 [Hits:0 Misses:11]
Access: 6  MISS FirstIn -> [2, 3, 4, 5, 6] <- Lastin  Replaced:1 [Hits:0 Misses:12]

FINALSTATS hits 0   misses 12   hitrate 0.00
```

**LRU 정책의 최악의 주소 참조 스트림**

```bash
$ python paging-policy.py -a 1,2,3,4,5,6,1,2,3,4,5,6 -C 5 -p LRU -c
ARG addresses 1,2,3,4,5,6,1,2,3,4,5,6
ARG addressfile
ARG numaddrs 10
ARG policy LRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 2  MISS LRU ->       [1, 2] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  MISS LRU ->    [1, 2, 3] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 4  MISS LRU -> [1, 2, 3, 4] <- MRU Replaced:- [Hits:0 Misses:4]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:- [Hits:0 Misses:5]
Access: 6  MISS LRU -> [2, 3, 4, 5, 6] <- MRU Replaced:1 [Hits:0 Misses:6]
Access: 1  MISS LRU -> [3, 4, 5, 6, 1] <- MRU Replaced:2 [Hits:0 Misses:7]
Access: 2  MISS LRU -> [4, 5, 6, 1, 2] <- MRU Replaced:3 [Hits:0 Misses:8]
Access: 3  MISS LRU -> [5, 6, 1, 2, 3] <- MRU Replaced:4 [Hits:0 Misses:9]
Access: 4  MISS LRU -> [6, 1, 2, 3, 4] <- MRU Replaced:5 [Hits:0 Misses:10]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:6 [Hits:0 Misses:11]
Access: 6  MISS LRU -> [2, 3, 4, 5, 6] <- MRU Replaced:1 [Hits:0 Misses:12]

FINALSTATS hits 0   misses 12   hitrate 0.00
```

**MRU 정책의 최악의 주소 참조 스트림**

```bash
$ python3 paging-policy.py -a 1,2,3,4,5,6,7,8,9,10,6,7,8,9,10 -C 5 -p MRU -c
ARG addresses 1,2,3,4,5,6,7,8,9,10,6,7,8,9,10
ARG addressfile
ARG numaddrs 10
ARG policy MRU
ARG clockbits 2
ARG cachesize 5
ARG maxpage 10
ARG seed 0
ARG notrace False

Solving...

Access: 1  MISS LRU ->          [1] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 2  MISS LRU ->       [1, 2] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 3  MISS LRU ->    [1, 2, 3] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 4  MISS LRU -> [1, 2, 3, 4] <- MRU Replaced:- [Hits:0 Misses:4]
Access: 5  MISS LRU -> [1, 2, 3, 4, 5] <- MRU Replaced:- [Hits:0 Misses:5]
Access: 6  MISS LRU -> [1, 2, 3, 4, 6] <- MRU Replaced:5 [Hits:0 Misses:6]
Access: 7  MISS LRU -> [1, 2, 3, 4, 7] <- MRU Replaced:6 [Hits:0 Misses:7]
Access: 8  MISS LRU -> [1, 2, 3, 4, 8] <- MRU Replaced:7 [Hits:0 Misses:8]
Access: 9  MISS LRU -> [1, 2, 3, 4, 9] <- MRU Replaced:8 [Hits:0 Misses:9]
Access: 10  MISS LRU -> [1, 2, 3, 4, 10] <- MRU Replaced:9 [Hits:0 Misses:10]
Access: 6  MISS LRU -> [1, 2, 3, 4, 6] <- MRU Replaced:10 [Hits:0 Misses:11]
Access: 7  MISS LRU -> [1, 2, 3, 4, 7] <- MRU Replaced:6 [Hits:0 Misses:12]
Access: 8  MISS LRU -> [1, 2, 3, 4, 8] <- MRU Replaced:7 [Hits:0 Misses:13]
Access: 9  MISS LRU -> [1, 2, 3, 4, 9] <- MRU Replaced:8 [Hits:0 Misses:14]
Access: 10  MISS LRU -> [1, 2, 3, 4, 10] <- MRU Replaced:9 [Hits:0 Misses:15]

FINALSTATS hits 0   misses 15   hitrate 0.00
```

첫 참조열의 경우 반복문에서 빈번히 발생하는 참조 패턴이다. 만약 캐시의 크기보다 조금이라도 더 많은 개수의 페이지가 반복문의 패턴으로 참조될 경우 히트율은 0%이다. 이때 참조되는 페이지의 종류 수를 $P$, 캐시의 크기를 $C$ 이라고 하면 $C \ge P$ 를 만족해야만 OPT의 성능에 근접할 수 있다. 이 패턴의 경우 FIFO와 LRU은 같은 방식으로 가장 오래 전에 참조된 페이지를 제거하지만 이는 곧 다시 참조될 페이지이기 때문에 캐시의 크기를 비약적으로 늘리는 것 외에 해결책은 없다.

마지막 참조열의 경우 처음 캐시에 적재된 페이지들은 지역성을 갖지 않지만 나머지 페이지 참조가 지역성을 띠고 있는 형태이다. 충분히 발생할 법한 참조 패턴이다. 하지만 MRU는 캐시에 적재된 페이지들이 한 번쯤은 히트되기를 기대한다. 그것도 지역성을 갖고 히트되는 것이 아니라 조금은 무작위하게 히트되기를 기대하는 것이다. 그렇게 하면 최소한 캐시 크기 정도의 히트 횟수는 보장될 수 있고, 캐시에 적재된 페이지들이 한 번씩은 히트되는 것을 보장한다(끝의 교체가 빈번하게 발생하는 슬롯 제외). 하지만 지역성을 가진 위 참조 패턴에서는 LRU 또는 OPT 알고리즘의 동작이 훨씬 정확하다. 그리고 이 경우에는 캐시 크기를 늘리더라도 참조 패턴에 따라서는 그것이 아무런 소용이 없을 수도 있다. MRU 알고리즘의 가정인 "최근에 참조된 페이지는 앞으로 참조될 가능성이 낮을 것이다."가 깨지는 순간 캐시의 한 슬롯에서만 반복적으로 페이지 교체가 발생할 것이며, 또한 그 가정은 쉽게 깨진다.

## Problem 3

    랜덤한 추적 기록을 생성하라 (python 또는 perl을 사용하라). 이런 추적 기록을 적용할 때 여러 정책이 어떻게 동작할 것이라고 예상하는가?

정책별로 무작위한 참조열에 대한 성능에 대해 논해 보겠다.

일단 이에 대해 어떤 페이지 정책이 우수한지 비교하는 것은 의미가 없다. 수많은 참조 패턴들 중 특정 정책이 이득을 볼 수 있는 참조 패턴이 생성될 확률은 극히 낮기 때문이다. 그리고 과거의 정보가 미래의 정보를 예측하기 위한 척도가 될 수 없다.

하지만 이 경우에도 OPT 알고리즘은 최선의 페이지 교체 전략을 보장한다. 이때의 OPT 알고리즘의 동작은 가장 나중에 참조될 페이지를 제거함으로써 나머지 페이지가 참조될 가능성을 높이는 것이다.

위 알고리즘들은 모두 결정론적이지만 유일하게 비결정론적인 RANDOM 알고리즘이 유일하게 OPT 알고리즘보다 평균적으로 낮은 성능을 보이면서도 나머지 현실적인 알고리즘보다 좋은 성능을 보일 가능성이 있다. 특정 패턴에 의해 편향되지 않고 일관된 성능을 보장하기 때문이다.

## Problem 4

    얼마간 지역성을 보이는 추적 기록을 생성하라. 그런 추적 기록을 어떻게 생성할 수 있을까? 생성된 추적 기록대로 메모리 참조가 일어날 때 LRU의 성능은 어떤가? RAND는 LRU에 비해서 얼마나 더 좋은 성능을 보이는가? CLOCK은 어떤 성능을 보이는가? 시계 비트의 수를 변경하였을 때 CLOCK은 어떤 성능을 보이는가?

**generator.py**

```py
import random

def generate_locality_trace(total_pages, hot_pages_count, length, hot_ratio=0.8):
    hot_pages = list(range(1, hot_pages_count + 1))
    cold_pages = list(range(hot_pages_count + 1, total_pages + 1))

    trace = []
    for _ in range(length):
        if random.random() < hot_ratio:
            trace.append(random.choice(hot_pages))
        else:
            trace.append(random.choice(cold_pages))
    return trace

# 1. 참조 패턴 생성
CACHE_SIZE = 6
trace_length = 1000
generated_trace = generate_locality_trace(total_pages=20, hot_pages_count=5, length=trace_length)

# 2. 파일로 출력 (comma-separated)
file_name = "memory_trace.txt"

try:
    with open(file_name, "w", encoding="utf-8") as f:
        # 리스트의 모든 요소를 문자열로 바꾸고 콤마로 연결
        trace_string = "\n".join(map(str, generated_trace))
        f.write(trace_string)

    print(f"성공적으로 '{file_name}' 파일이 생성되었습니다.")
    print(f"내용 샘플: {trace_string[:50]}...")
except IOError as e:
    print(f"파일 생성 중 오류 발생: {e}")
```

얼마 간 지역성을 보이는 추적 기록을 생성하기 위해 사용된 아이디어는 간단하다. 참조가 자주 발생될 핫 페이지와 그렇지 않은 콜드 페이지를 1:3 정도의 비율로 구분하고 80:20 법칙에 따라 80% 정도의 참조가 핫 페이지에 의해 발생하게 하였다.

알고리즘별 페이지 히트율은 다음과 같다.

- `OPT`: 82.40%
- `LRU`: 67.40%
- `RAND`: 61.90%
-
