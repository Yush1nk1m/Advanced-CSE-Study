# OSTEP Chapter 09 Homework

본 장의 숙제는 시스템 콜과 컨텍스트 스위칭의 비용을 측정하는 것을 목표로 한다. 현대 CPU의 성능은 아주 빠르게 향상되었기 때문에 두 비용은 밀리 초 단위에서 측정하기가 어렵고 나노 초 단위에서 측정해야만 한다. 이를 위해 `rdtsc` 명령어를 활용할 것을 권장하고 있다. 이는 CPU가 관리하고 있는 클럭 수를 통해 시간을 측정하는 방식으로 나노 초 단위의 측정을 가능케한다. 그리고 컨텍스트 스위칭 비용 측정 시 두 프로세스의 CPU 친숙도를 제어하여 특정 코어에서만 실행되도록 강제하고 프로세스들 간 `write()`, `read()` 시스템 콜을 환형으로 호출하여 컨텍스트 스위칭이 즉시 발생하도록 유도한다.

## 1. 시스템 콜 비용 측정

**syscall_cost.c**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <x86intrin.h>  // Header for __rdtsc()

// # of measurement iterations
#define ITERATIONS      1000000
// Value used to convert clock cycles to time
#define CPU_FREQ_MHZ    3900.0

/**
 * Executes the null system call (read 0 bytes).
 * Declared static inline to eliminate function call overhead.
 */
static inline void null_syscall(int fd) {
    // TODO: Perform the null system call
    read(fd, NULL, 0);
}

int main(int argc, char* argv[]) {
    unsigned long long start_cycles, end_cycles, total_cycles = 0;
    int i;

    // Open /dev/null for the null system call
    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) {
        perror("Error opening /dev/null");
        exit(EXIT_FAILURE);
    }

    printf("--- System Call Cost Measurement (Using RDTSC) ---\n");
    printf("Assumed CPU Frequency: %.1f MHz (%.2f ns/cycle)\n", CPU_FREQ_MHZ, 1000.0 / CPU_FREQ_MHZ);
    printf("Iterations: %d\n", ITERATIONS);

    // 1. Measurement Loop Execution
    for (i = 0; i < ITERATIONS; ++i) {
        start_cycles = __rdtsc();

        // TODO: Call the null system call
        null_syscall(fd);

        end_cycles = __rdtsc();
        total_cycles += (end_cycles - start_cycles);
    }

    close(fd);

    // 2. Result Calculation
    double average_cycles = (double) total_cycles / ITERATIONS;
    // Time per cycle (ns) = 1000 MHz / CPU_FREQ_MHZ
    double time_per_cycle_ns = 1000.0 / CPU_FREQ_MHZ;
    double average_cost_ns = average_cycles * time_per_cycle_ns;

    printf("-------------------------------------------\n");
    printf("Average Cost per System Call:\n");
    printf("    Average Clock Cycles: %.2f cycles\n", average_cycles);
    printf("    Average Time Cost: %.3f ns (nanoseconds)\n", average_cost_ns);

    return 0;

}
```

코드의 핵심은 `null_syscall()` 프로시저로 빈 시스템 콜을 호출하여 순수하게 시스템 콜 호출만으로 몇 사이클이 소모되는지를 측정한다는 것이다.

**syscall_cost.c output**

```bash
yushinkim@yushinkim-15Z90N-VA70K:~/Study/Advanced-CSE-Study/Operating-Systems/ostep/chapter09/hw$ ./syscall_cost
--- System Call Cost Measurement (Using RDTSC) ---
Assumed CPU Frequency: 3900.0 MHz (0.26 ns/cycle)
Iterations: 1000000
-------------------------------------------
Average Cost per System Call:
    Average Clock Cycles: 357.75 cycles
    Average Time Cost: 91.730 ns (nanoseconds)
```

CPU는 Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz이다. 시스템 콜 호출당 평균적으로 약 90ns 정도가 소요된다.

## 2. 컨텍스트 스위치 비용 측정

**context_switch_cost.c**

```c
#define _GNU_SOURCE     // Required for sched_setaffinity
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>      // sched_setaffinity
#include <x86intrin.h>  // Header for __rdtsc()

// # of communication cycles (read/write pairs)
#define CYCLES      10000
// Data size to be exchanged
#define DATA_SIZE   1
// My CPU frequency in MHz
#define CPU_FREQ_MHZ 3900.0

// Function to pin the process to a specific CPU core
void set_cpu_affinity(int cpu_id) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);

    // pid 0 means current process
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        perror("sched_setaffinity failed");
        exit(EXIT_FAILURE);
    }
    printf("[%d] Process pinned to CPU %d.\n", (int) getpid(), cpu_id);
}

int main(int argc, char* argv[]) {
    int pipe1[2];   // Parent -> Child communication
    int pipe2[2];   // Child -> Parent communication
    pid_t pid;

    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // TODO: Set the target CPU core ID for affinity
    const int TARGET_CPU = 3;

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // ------------------ Child Process ------------------
        set_cpu_affinity(TARGET_CPU);   // Pin child to the same core

        // TODO: Close the unused pipe ends for the child process.
        close(pipe1[1]);
        close(pipe2[0]);

        // Allocate a minimal buffer for communication
        char data[DATA_SIZE];

        // TODO: Implement the communication cycle with the parent.
        // 1. Wait for data from pipe1[0] (Induces blocking/context switch)
        // 2. Write data to pipe2[1] (Unblocks parent/induces context switch)

        for (int i = 0; i < CYCLES; ++i) {
            read(pipe1[0], data, DATA_SIZE);
            write(pipe2[1], data, DATA_SIZE);
        }
    } else {
        // ------------------ Parent Process ------------------
        set_cpu_affinity(TARGET_CPU);   // Pin parent to the same core

        // TODO: Close the unused pipe ends for the parent process
        close(pipe1[0]);
        close(pipe2[1]);

        unsigned long long start_cycles, end_cycles;
        char data[DATA_SIZE];

        start_cycles = __rdtsc();   // Start measurement

        // TODO: Implement the communication cycle with the parent.
        // 1. Write data to pipe1[1] (Unblocks child/induces context switch)
        // 2. Wait for data from pipe2[0] (Induces blocking/context switch)

        for (int i = 0; i < CYCLES; ++i) {
            write(pipe1[1], data, DATA_SIZE);
            read(pipe2[0], data, DATA_SIZE);
        }

        end_cycles = __rdtsc();     // End measurement

        unsigned long long total_cycles = end_cycles - start_cycles;

        // Single cycle (Parent_Write -> Child_Read -> Child_Write -> Parent_Read) includes 2 context switches.
        double total_switches = CYCLES * 2.0;
        double average_cycles = (double) total_cycles / total_switches;
        double time_per_cycle_ns = 1000.0 / CPU_FREQ_MHZ;
        double average_cost_ns = average_cycles * time_per_cycle_ns;

        printf("\n--- Context Switch Cost Measurement (RDTSC, Cycles: %d, Total Switches: %.0f) ---\n", CYCLES, total_switches);
        printf("Total Clock Cycles: %llu\n", total_cycles);
        printf("Average Cost per Context Switch:\n");
        printf("    Average Clock Cycles: %.2f cycles\n", average_cycles);
        printf("    Average Time Cost: %.3f ns (nanoseconds)\n", average_cost_ns);

        // Wait for child process to finish
        wait(NULL);
    }
    return 0;
}
```

컨텍스트 스위치 비용 분석을 위해 두 프로세스가 파이프를 통해 `read()`, `write()` 시스템 콜을 사용하여 매우 작은 데이터를 환형(circular)으로 10,000회 교환하도록 하였다. 그렇게 하면 두 프로세스 간 컨텍스트 스위치가 20,000회 발생하게 되고 이를 통해 컨텍스트 스위치당 소요된 시간을 계산할 수 있다.

**context_switch_cost.c output**

```bash
$ ./context_switch_cost
[825452] Process pinned to CPU 0.
[825451] Process pinned to CPU 0.

--- Context Switch Cost Measurement (RDTSC, Cycles: 10000, Total Switches: 20000) ---
Total Clock Cycles: 132533915
Average Cost per Context Switch:
    Average Clock Cycles: 6626.70 cycles
    Average Time Cost: 1699.153 ns (nanoseconds)
```

컨텍스트 스위치의 평균 소요 시간에 대한 측정 결과는 1,200ns~2,200ns 정도로 변동이 크지만 평균적으로 1,700ns 정도가 소요된다.

## 3. 결론

시스템에 위 코드들로부터 작동되는 두 개의 프로세스만 가동될 수 있는 상황이라고 가정하자.

빈 시스템 콜은 잠시 커널 모드에 접어들어 아무 작업도 하지 않은 채로 커널 스택에서 즉시 레지스터 값들을 복원하고 기존에 프로세스가 사용하고 있던 가상 주소 공간을 그대로 사용하면 된다. 이러한 작업은 사실상 커널 모드로 전환되었다가 다시 사용자 모드로 전환되는 것이나 다름이 없다.

반면 컨텍스트 스위치에 대한 프로그램은 그 안에 다음과 같은 복잡한 과정들이 포함되어 있다.

1. 시스템 콜 호출로 인한 커널 모드 전환
2. 시스템 콜 수행 후 컨텍스트 스위치 발생
3. 기존의 프로세스의 상태가 Waiting으로 전환
4. 새로운 프로세스의 가상 주소 공간 덮어쓰기(On-demand copy 방식)

위와 같은 과정들 때문에 컨텍스트 스위치는 빈 시스템 콜 호출에 비해 비용을 정확히 측정하기가 어렵다. 앞선 실험에서는 컨텍스트 스위치의 경우 비용의 변동폭이 매우 크기도 했고, 소요된 시간 동안 컨텍스트 스위치만 발생하는 것도 아니다.

일반적으로 컨텍스트 스위치 자체는 빈 시스템 콜 호출보다 5~6배 정도의 시간이 소요된다고 알려져 있다.
