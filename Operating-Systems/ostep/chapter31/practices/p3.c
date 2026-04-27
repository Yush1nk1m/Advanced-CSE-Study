#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common_threads.h"

typedef struct __lock_t { int flag; } lock_t;

// int CompareAndSwap(int* ptr, int expected, int new) {
//     int actual = *ptr;
//     if (actual == expected)
//         *ptr = new;
//     return actual;
// }

int CompareAndSwap(int* ptr, int old, int new) {
    unsigned char ret;

    __asm__ __volatile__ (
        " lock\n"
        " cmpxchgl %2,%1\n"
        " sete %0\n"
        : "=q" (ret), "+m" (*ptr)
        : "r" (new), "a" (old)
        : "memory");
    
    return ret;
}

void init(lock_t* mutex) {
	//	0 -> 락이 사용 가능함, 1 -> 락 사용 중
	mutex->flag = 0;
}

void lock(lock_t* mutex) {
	while (CompareAndSwap(&mutex->flag, 0, 1) == 0)
		;	// busy wait
}

void unlock(lock_t* mutex) {
	mutex->flag = 0;
}

volatile int count;

void* routine(void* arg) {
	lock_t* mutex = (lock_t*) arg;
	for (int i = 0; i < 1000000; ++i) {
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