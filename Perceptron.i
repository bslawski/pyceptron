%module Perceptron

%{
#define SWIG_FILE_WITH_INIT
#include "Perceptron.h"
%}

void initPerceptron(int nlyrs, int *nnrns, double learnrate);

double forwardPass(void *input);

void backProp(double error);

double *MallocDoubleArr(int arrLen);
int *MallocIntArr(int arrLen);

void Free(void *);

void editDoubleArray(double *arr, int ind, double val);
void editIntArray(int *arr, int ind, int val);
