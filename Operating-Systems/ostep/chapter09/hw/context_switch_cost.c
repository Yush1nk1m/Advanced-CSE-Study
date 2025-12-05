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
    const int TARGET_CPU = 0;

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