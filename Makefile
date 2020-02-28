all:
	swig -c++ -python Perceptron.i
	python setupPerceptron.py build_ext --inplace
clean:
	rm Perceptron.h.gch
