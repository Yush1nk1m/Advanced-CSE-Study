#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common_threads.h"

typedef struct __lock_t {
    int ticket;
    int turn;
} lock_t;

int FetchAndAdd(int* ptr) {
    int old = *ptr;
    *ptr = old + 1;
    return old;
}

void init(lock_t* mutex) {
	//	0 -> 락이 사용 가능함, 1 -> 락 사용 중
	mutex->ticket = mutex->turn = 0;
}

void lock(lock_t* mutex) {
    int myturn = FetchAndAdd(&mutex->ticket);
    while (mutex->turn != myturn)
        ;
}

void unlock(lock_t* mutex) {
    FetchAndAdd(&mutex->turn);
}

volatile int count;

void* routine(void* arg) {
	lock_t* mutex = (lock_t*) arg;
	for (int i = 0; i < 1000; ++i) {
		lock(mutex);
		++count;
		unlock(mutex);
	}
}

int main(int argc, char* argv[]) {
	pthread_t p[20];
	lock_t mutex;

	init(&mutex);
	for (int i = 0; i < 20; ++i) {
		Pthread_create(&p[i], NULL, routine, &mutex);
	}
	for (int i = 0; i < 20; ++i) {
		Pthread_join(p[i], NULL);
	}

	printf("count: %d\n", count);

	return 0;
}