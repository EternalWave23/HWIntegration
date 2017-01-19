#include "Reader.h"
#include "Classifier.h"

#ifndef TESTER
#define TESTER

class Tester {
public:
	void Test(Reader* reader, Classifier* cls);
	void Report();
};

#endif