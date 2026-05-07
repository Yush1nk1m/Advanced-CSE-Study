#include <stdio.h>
#include "common_threads.h"

#define MAX 16
#define LOOPS 1024

int buffer[MAX];
int fill = 0;
int use = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void* producer(void* arg) {
    for (int i = 0; i < LOOPS; ++i) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void* consumer(void* arg) {
    int tmp = 0;
    while (tmp != -1) {
        sem_wait(&full);
        sem_wait(&mutex);
        tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("%d\n", tmp);
    }
}

int main(int argc, char* argv[]) {
    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t p, c;
    Pthread_create(&p, NULL, producer, NULL);
    Pthread_create(&c, NULL, consumer, NULL);
    Pthread_join(p, NULL);
    Pthread_join(c, NULL);

    return 0;
}