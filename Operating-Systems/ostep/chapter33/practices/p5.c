#include "p4.c"

void* producer(void* arg) {
    for (int i = 0; i < (int) arg; ++i) {
        put(i);
    }
}

void* consumer(void* arg) {
    while (1) {
        printf("%d\n", get());
    }
}