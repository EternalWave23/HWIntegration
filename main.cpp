#include <iostream>
#include "Reader.h"
#include "Classifier.h"
#include "Tester.h"
using namespace std;

int main() {
	cout << "hello world" << endl;
	Reader reader;
	reader.ReadFile("E:\\test\\");
	reader.Parse();
	
	Classifier cls;




	Tester tester;
	tester.Test(&reader, &cls);
	tester.Report();
	system("pause");
	return 0;
}