#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <x86intrin.h>
#include <unistd.h>

#define PAGE_SIZE 4096

void pin_to_core(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    // 현재 스레드를 지정 코어에 고정
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity");
    } else {
        printf("스레드가 %d번 코어에 고정되었습니다.\n", core_id);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    pin_to_core(0); // 0번 코어에 고정

    int num_pages = atoi(argv[1]);
    int iterations = atoi(argv[2]);

    // 1. 메모리 할당 및 물리 페이지 매핑 (Touch)
    size_t alloc_size = (size_t) num_pages * PAGE_SIZE;
    char* memory = (char*) malloc(alloc_size);
    if (!memory) {
        perror("Memory allocation failed");
        return 1;
    }

    for (int i = 0; i < num_pages; ++i) {
        memory[i * PAGE_SIZE] = 1;  // 실제 물리 메모리에 페이지 할당 강제
    }

    unsigned long long start, end;
    volatile char dummy;

    // 2. 실험: 랜덤 페이지 접근

    // 직렬화를 위해 lfence 사용 (이전 메모리 작업 완료 후 프로시저 실행 보장)
    _mm_lfence();
    start = __rdtsc();

    for (int i = 0; i < iterations; ++i) {
        // 매번 다른 페이지의 오프셋 0번지에 접근
        // 컴파일러가 rand()를 미리 계산하지 못하도록 루프 안에 배치
        int target = (rand() % num_pages) * PAGE_SIZE;
        dummy = memory[target];
    }

    _mm_lfence();
    end = __rdtsc();

    unsigned long long total_cycles = end - start;
    printf("--- 실험 결과 ---\n");
    printf("총 접근 페이지: %d개\n", num_pages);
    printf("총 시도 횟수: %d회\n", iterations);
    printf("총 소요 사이클: %llu\n", total_cycles);
    printf("접근 1회당 평균 사이클: %llu\n", total_cycles / iterations);

    free(memory);
    return 0;
}