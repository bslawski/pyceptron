#include "Perceptron.h"

Perceptron brain;

void initPerceptron(int nlyrs, int *nnrns, double learnrate) {
    brain.initPerceptron(nlyrs, nnrns, learnrate);
}

double forwardPass(void *input) {
    return brain.forwardPass((double *) input);
}

void backProp(double error) {
    brain.backProp(error);
}

double *MallocDoubleArr(int arrLen) {
    return (double *) malloc(arrLen * sizeof(double));
}

int *MallocIntArr(int arrLen) {
    return (int *) malloc(arrLen * sizeof(int));
}

void Free(void *toFree) {
    free(toFree);
}


void editDoubleArray(double *arr, int ind, double val) {
    arr[ind] = val;
}

void editIntArray(int *arr, int ind, int val) {
    arr[ind] = val;
}
