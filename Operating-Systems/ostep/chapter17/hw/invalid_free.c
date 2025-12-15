#include <stdlib.h>

int main(int argc, char* argv[]) {
    int* x = (int*) malloc(sizeof(int) * 10);
    x += 5;
    free(x);

    return 0;
}