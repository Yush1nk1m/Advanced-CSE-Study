#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "common_threads.h"

void* mythread(void* arg) {
    int m = (int) arg;
    printf("%d\n", m);
    return (void*) (arg + 1);
}

int main(int argc, char* argv[]) {
    pthread_t p;
    int rc, m;
    Pthread_create(&p, NULL, mythread, (void*) 100);
    Pthread_join(p, (void**) &m);
    printf("returned %d\n", m);
    
    return 0;
}