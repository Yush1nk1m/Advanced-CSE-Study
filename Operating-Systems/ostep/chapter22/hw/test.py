import subprocess
import re
import matplotlib.pyplot as plt

# 설정
EXECUTABLE = "./a.out"     # 컴파일된 C 프로그램 경로
ITERATIONS = 100000000     # 루프 횟수 고정
PAGE_COUNTS = [2**i for i in range(1, 16)]  # 2, 4, 8, ..., 32768
results = []

print(f"{'Pages':>10} | {'Avg Cycles':>15}")
print("-" * 30)

for count in PAGE_COUNTS:
    # 프로그램 실행 (taskset으로 0번 코어 고정 권장)
    try:
        cmd = ["taskset", "-c", "0", EXECUTABLE, str(count), str(ITERATIONS)]
        output = subprocess.check_output(cmd, universal_newlines=True)
        
        # 출력 결과에서 "평균 사이클: 숫자" 부분 추출
        match = re.search(r"평균 사이클: (\d+)", output)
        if match:
            avg_cycle = int(match.group(1))
            results.append(avg_cycle)
            print(f"{count:10d} | {avg_cycle:15d}")
            
    except Exception as e:
        print(f"Error running for {count} pages: {e}")
        break

# 2. 결과 시각화
plt.figure(figsize=(10, 6))
plt.plot(PAGE_COUNTS, results, marker='o', linestyle='-', color='b')

# X축을 로그 스케일로 설정 (2의 배수이므로 보기 편함)
plt.xscale('log', base=2)
plt.xlabel('Number of Pages (Log Scale)')
plt.ylabel('Average Cycles per Access')
plt.title('TLB Miss Latency Test by Page Count')
plt.grid(True, which="both", ls="-", alpha=0.5)

# TLB 임계점 예상선 표시 (예: L1 TLB 64, L2 TLB 1024)
plt.axvline(x=64, color='r', linestyle='--', label='L1 TLB Boundary (approx)')
plt.axvline(x=1024, color='g', linestyle='--', label='L2 TLB Boundary (approx)')
plt.legend()

plt.show()