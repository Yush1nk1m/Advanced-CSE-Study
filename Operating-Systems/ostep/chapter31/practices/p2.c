#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common_threads.h"

typedef struct __lock_t { int flag; } lock_t;

int TestAndSet(int* old_ptr, int new) {
	int old = *old_ptr;
	*old_ptr = new;
	return old;
}

void init(lock_t* mutex) {
	//	0 -> 락이 사용 가능함, 1 -> 락 사용 중
	mutex->flag = 0;
}

void lock(lock_t* mutex) {
	while (TestAndSet(&mutex->flag, 1) == 1)
		;	// busy wait
}

void unlock(lock_t* mutex) {
	mutex->flag = 0;
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