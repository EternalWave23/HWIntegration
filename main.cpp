#include <iostream>
#include "Reader.h"
#include "Classifier.h"
#include "Tester.h" 

using namespace std;

int main() {
	// cout << "hello world" << endl;
	Tester* tester = new Tester();
	tester->Test();
	tester->Report();
	delete tester;

	cout << "Complete!" << endl;
	system("pause");
	return 0;
}