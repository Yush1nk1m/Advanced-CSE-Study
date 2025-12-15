#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size, i;
    int* array;
} Vector;

Vector* createVector(int size) {
    Vector* vector = (Vector*) malloc(sizeof(Vector));
    vector->size = size;
    vector->i = 0;
    vector->array = (int*) malloc(sizeof(int) * size);
}

void addElement(Vector* vector, int element) {
    if (vector->i >= vector->size) {
        vector->size <<= 1;
        vector->array = realloc(vector->array, vector->size * sizeof(int));
    }
    vector->array[vector->i++] = element;
}

void printElements(Vector* vector) {
    printf("vector: [ ");
    for (int j = 0; j < vector->i; ++j)
        printf("%d ", vector->array[j]);
    printf("]\n");
}

void freeVector(Vector* vector) {
    free(vector->array);
    free(vector);
}

int main(int argc, char* argv[]) {
    Vector* vector = createVector(1);
    for (int i = 0; i < 10; ++i)
        addElement(vector, i);
    printElements(vector);
    freeVector(vector);

    return 0;
}