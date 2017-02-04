#include "Reader.h"
#include "Classifier.h"

#ifndef TESTER
#define TESTER

class Tester {
public:
	void Test();
	void Report();
	Tester();

private:
	void TestSingleFile(string fileName);
	void ListFiles(string path, vector<string>* files); 
	int TP, TN, FP, FN;
	bool Label2Bool(int label);
};

#endif