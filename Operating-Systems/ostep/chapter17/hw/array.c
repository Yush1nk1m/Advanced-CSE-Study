#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int data[100];
    data[100] = 0;

    int* data2 = (int*) malloc(sizeof(int) * 100);
    data2[100] = 0;
    free(data2);

    return 0;
}