from Perceptron import *
from random import *
import os

class FastPerceptron:

    def __init__(self, layers, learnrate):
        self.nlayers = len(layers)
        self.ninput = layers[0]

        layerArr = MallocIntArr(self.nlayers)
        for i in range(0,self.nlayers):
            editIntArray(layerArr, i, layers[i])
        initPerceptron(self.nlayers, layerArr, learnrate)
        Free(layerArr)

        self.inputArr = MallocDoubleArr(self.ninput)

    def forwardPass(self, input):
        for i in range(0, self.ninput):
            editDoubleArray(self.inputArr, i, input[i])
        return forwardPass(self.inputArr)

    def backProp(self, err):
        backProp(err)

if __name__ == "__main__":
##    print "compiling perceptron code...\n"
##    os.system("make")

    test = FastPerceptron([2, 1000], .0001)
    err = 0
    printrate = 1000
    count = 0
    correct = [-1, -1, 1, 1]
    while True:
        i = int(random()*4)
        out = test.forwardPass([(2*(i%2)-1), (2*int(i/2)-1)])
        error = correct[i] - out
        test.backProp(error)
        err += error * error
        count += 1
        if count % printrate == 0:
            print err/printrate
            err = 0
            count = 0

