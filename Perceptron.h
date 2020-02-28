#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Neuron {

    private:

    // fields
        long double charge;
    
    public:

        // constructor
        Neuron() {
            charge = 0.;
        }

        // methods

        double actFun(long double arg) {
            if isnan((tanh(arg))) { printf("%f\n", (double) charge);}
            return (double) tanh(arg);
        }

        double deactFun() {
            double tmp = (double) (1./(1.-charge*charge));
            if (isinf(tmp) || tmp > 999.) {
                return 999.;
            }

            return tmp;
        }

        double axonOut() {
            return actFun(charge);
        }

        void dendriteIn(double dendrite) {
            charge = dendrite;
        }

        void printCharge() {
            printf("%f", (double)charge);
        }
};


class WeightMatrix {
    
    private:

        // fields
        int naxons;
        int ndendrites;
	double *weights;
        double *biases;

    public:

        // constructor
        WeightMatrix() {
            srand(time(NULL));
        }

        void seedMatrix(int nin, int nout) {
            naxons = nin;
            ndendrites = nout;
            weights = 0;
            weights = new double[ndendrites*naxons];
            biases = new double[ndendrites];
            int i, j;
            for (i=0; i<naxons; i++) {
                for (j=0; j<ndendrites; j++) {
                    // random in range [-1, 1]
                    weights[getSubscript(i,j)] = .05 * (2. * ((double)rand()/(double)RAND_MAX) -1.);
                }
            }
            for (j=0; j<ndendrites; j++) {
                biases[j] = .05 * (2. * ((double)rand()/(double)RAND_MAX) -1.);
            }
        }

    // methods
        int getSubscript(int axon, int dendrite) {
            return axon*ndendrites + dendrite;
        }

        void passSignals(Neuron *axons, double *dendrites) {
            int i, j;
            for (j=0; j<ndendrites; j++) {
                dendrites[j] = 0;
            }
            for (i=0; i<naxons; i++) {
                for (j=0; j<ndendrites; j++) {
                    dendrites[j] += weights[getSubscript(i,j)] * axons[i].axonOut();
                }
            }
            for (j=0; j<ndendrites; j++) {
                dendrites[j] += biases[j];
            }
            for (j=0; j<ndendrites; j++) {
                if (isnan(dendrites[j]) || isinf(dendrites[j])) {
                    dendrites[j] = 99999.;
                }
            }
        }

        void backProp(double *layerErr, Neuron *neurons, double learnrate) {
            double *propErr = new double[naxons];
            int i,j;
            double d;
            double tmp;
            for (i=0; i<naxons; i++) {
                propErr[i] = 0.;
            }
            for (i=0; i<naxons; i++) {
                for (j=0; j<ndendrites; j++) {
                    d = layerErr[j] * weights[getSubscript(i,j)];
                    tmp = neurons[i].axonOut();
                    weights[getSubscript(i,j)] += learnrate * d * neurons[i].axonOut();
                    propErr[i] += d * neurons[i].deactFun();
                }
            }
            for (j=0; j<ndendrites; j++) {
                biases[j] += learnrate * biases[j] * layerErr[j];
            }
//            delete layerErr;
            layerErr = propErr;
        }

        void printWeights() {
            int i, j;
            for (i=0; i<naxons; i++) {
                for (j=0; j<ndendrites; j++) {
                    printf("[%d,%d] %f\t", i, j, weights[getSubscript(i,j)]);
                }
                printf("\n");
            }
            for (j=0; j<ndendrites; j++) {
                printf("b%d %f\t", j, biases[j]);
            }
            printf("\n");
        }
    
};


class NeuronLayer {

    private:

        // fields
        int nneurons;
        int naxonbranches;
        double learnrate;
        Neuron *neurons;
        WeightMatrix w;
    
    public:

        // constructor
        NeuronLayer() {
            nneurons = 0; 
            naxonbranches = 0;
            learnrate = 0.;
        }

        void growLayer(int nin, int nout, double lrnrate) {
            nneurons = nin;
            naxonbranches = nout;
            neurons = new Neuron[nneurons];
            learnrate = lrnrate;
            w.seedMatrix(nneurons, naxonbranches);
        }

        // methods

        void setCharges(double *newcharges) {
            int i;
            for (i=0; i<nneurons; i++) {
                neurons[i].dendriteIn(newcharges[i]);
            }
            delete newcharges;
        }

        double *axonSignals() {
            double *signals = new double[naxonbranches];
            w.passSignals(neurons, signals);
            return signals;
        }

        void backProp(double *layerErr) {
            w.backProp(layerErr, neurons, learnrate);
        }

        void printLayer() {
            int i;
            printf("%d\n", nneurons);
            for (i=0; i<nneurons; i++) {
                neurons[i].printCharge();
                printf(" ");
            }
            printf("\n");
            for (i=0; i<nneurons; i++) {
                printf("%f ", neurons[i].axonOut());
            }
            printf("\n");
            w.printWeights();
        }
};



class Perceptron {

    private:

        // fields
        int nlayers;
        int ninput;
        NeuronLayer *layers;
        int maxnrns;
        Neuron probe;

    public:

        // constructor
        Perceptron() {
            nlayers = 0;
            ninput = 0;
            maxnrns = 0;
        }

        void initPerceptron(int nlyrs, int *nnrns, double learnrate) {
            nlayers = nlyrs;
            ninput = nnrns[0];
            layers = new NeuronLayer[nlayers];
            int i;
            maxnrns = 0;
            for (i=0; i<nlayers-1; i++) {
                printf("growing %d\t%d to %d\n", i, nnrns[i], nnrns[i+1]);
                layers[i].growLayer(nnrns[i], nnrns[i+1], learnrate);
                if (nnrns[i] > maxnrns) { maxnrns = nnrns[i]; }
            }
            int tmp = nlayers - 1;
            printf("growing %d\t%d to %d\n", i, nnrns[tmp], 1);
            layers[tmp].growLayer(nnrns[tmp], 1, learnrate);
            if (nnrns[tmp] > maxnrns) { maxnrns = nnrns[tmp]; }
        }

        // methods

        double forwardPass(double *inputSignal) {
            layers[0].setCharges(inputSignal);
//            printf("0\n");
//            layers[0].printLayer();
            int i;
//            printf("%d\n", nlayers);
            for (i=1; i<nlayers; i++) {
//                printf("%d\n", i);
//                layers[i].printLayer();
                layers[i].setCharges(layers[i-1].axonSignals());
//                layers[i].printLayer();
            }
            probe.dendriteIn((layers[nlayers-1].axonSignals())[0]);
//            printf("%f\n===\n", probe.axonOut());
            return probe.axonOut();
        }

        void backProp(double err) {
            double *layerErr = new double[1];
            layerErr[0] = err * probe.deactFun();
            int i;
            for (i=nlayers-1; i>=0; i--) {
                layers[i].backProp(layerErr);
            }
            delete layerErr;
        }

};



// Python methods
void initPerceptron(int nlyrs, int *nnrns, double learnrate);

double forwardPass(void *input);

void backProp(double error);

double *MallocDoubleArr(int arrLen);
int *MallocIntArr(int arrLen);

void Free(void *);

void editDoubleArray(double *arr, int ind, double val);
void editIntArray(int *arr, int ind, int val);

