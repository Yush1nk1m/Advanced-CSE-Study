#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "common_threads.h"

volatile int count;

void* mythread(void* arg) {
    pthread_mutex_t* mutex = (pthread_mutex_t*) arg;
    for (int i = 0; i < 1000000; ++i) {
        Pthread_mutex_lock(mutex);
        ++count;
        Pthread_mutex_unlock(mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_t p[20];
    pthread_mutex_t mutex;

    Mutex_init(&mutex);
    for (int i = 0; i < 20; ++i) {
        Pthread_create(&p[i], NULL, mythread, &mutex);
    }
    for (int i = 0; i < 20; ++i) {
        Pthread_join(p[i], NULL);
    }

    printf("count: %d\n", count);

    // Mutex_destroy(&mutex);
}