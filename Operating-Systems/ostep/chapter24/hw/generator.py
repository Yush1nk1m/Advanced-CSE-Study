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