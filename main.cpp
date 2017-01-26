#include <iostream>
#include "Reader.h"
#include "Classifier.h"
#include "Tester.h"
using namespace std;

int main() {
	// cout << "hello world" << endl;
	Reader* reader = new Reader();
	reader->ReadFile("F:\\Huawei\\Data\\test");
	
	Classifier* cls = new Classifier();

	Tester* tester = new Tester();
	tester->Test(reader, cls);
	tester->Report();

	delete reader;
	delete cls;
	delete(tester);

	cout << "Complete!" << endl;
	system("pause");
	return 0;
}