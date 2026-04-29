#include <stdio.h>
#include "common_threads.h"

void* child(void* arg) {
    printf("child\n");
    return NULL;
}

int main(int argc, char* argv[]) {
    printf("parent: begin\n");
    pthread_t c;
    Pthread_create(&c, NULL, child, NULL);
    printf("parent: end\n");
    return 0;
}