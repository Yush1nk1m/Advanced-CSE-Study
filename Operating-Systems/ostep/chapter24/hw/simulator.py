import matplotlib.pyplot as plt
from collections import deque

def simulate_lru(trace_file, cache_size, page_size_bits=12):
    """
    LRU 캐시 시뮬레이터
    - page_size_bits: 12 (4KB 페이지 기준)
    """
    cache = deque()
    hits = 0
    total = 0
    
    with open(trace_file, 'r') as f:
        for line in f:
            # Lackey 출력 형식: " I  04015f60, 2" 또는 " L  04201c10, 4"
            parts = line.split()
            if len(parts) < 2: continue
            
            try:
                # 16진수 주소 추출 및 VPN 변환 (오프셋 제거를 위해 비트 시프트)
                addr = int(parts[1].split(',')[0], 16)
                vpn = addr >> page_size_bits
                
                total += 1
                
                if vpn in cache:
                    hits += 1
                    # 사용된 페이지를 가장 최근(오른쪽)으로 이동
                    cache.remove(vpn)
                    cache.append(vpn)
                else:
                    if len(cache) >= cache_size:
                        cache.popleft() # 가장 오래된 페이지 제거
                    cache.append(vpn)
            except ValueError:
                continue
                
    return (hits / total) * 100 if total > 0 else 0

# --- 시뮬레이션 실행 ---
trace_path = 'trace.txt'  # Valgrind 추출 파일 경로
cache_sizes = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024] # 페이지 프레임 개수
hit_rates = []

print("시뮬레이션 시작...")
for size in cache_sizes:
    rate = simulate_lru(trace_path, size)
    hit_rates.append(rate)
    print(f"Cache Size: {size:4} pages | Hit Rate: {rate:.2f}%")

# --- 결과 시각화 ---
plt.figure(figsize=(10, 6))
plt.plot(cache_sizes, hit_rates, marker='o', linestyle='-', color='b')
plt.title('Cache Hit Rate vs. Cache Size (Working Set Analysis)')
plt.xlabel('Cache Size (Number of Page Frames)')
plt.ylabel('Hit Rate (%)')
plt.xscale('log', base=2) # X축을 로그 스케일로 설정하여 가독성 증대
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.show()