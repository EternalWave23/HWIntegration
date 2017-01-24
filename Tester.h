#include "Reader.h"
#include "Classifier.h"

#ifndef TESTER
#define TESTER

class Tester {
public:
	void Test(Reader* reader, Classifier* cls);
	void Report();
	Tester();

private:
	int TP, TN, FP, FN;
	bool Label2Bool(int label);
};

#endif