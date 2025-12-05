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