#include "Tester.h"

Tester::Tester() {
	TP = 0;
	TN = 0;
	FP = 0;
	FN = 0;
}

void Tester::Test(Reader* reader, Classifier* cls) {
	//int i = 1;
	while (reader->HasNextFrame()) {
		FrameData* tmp = reader->NextFrame();
		//cout << i << ", " << tmp->isDown << endl;
		//i++;
		vector<bool> result = cls->Classify(tmp);
		for (int i = 0; i < tmp->touchID.size(); i++) {
			if (tmp->isDown[i]) {
				//cout << "aaaa" << endl;
				bool groundTruth = Label2Bool(tmp->label[i]);
				if (result[i]) {
					if (groundTruth) {
						TP++;
					} else {
						FP++;
					}
				} else {
					if (groundTruth) {
						TN++;
					} else {
						FN++;
					}
				}
			}
		}
	}
}

void Tester::Report() {
	printf("TP = %d\nTN = %d\nFP = %d\nFN = %d\n", TP, TN, FP, FN);
	if (TP + FP != 0) {
		double accuracy = (double)TP / (TP + FP);
		cout << "accuracy = " << accuracy << endl;
	} else {
		cout << "accuracy = NaN" << endl;
	}
	if (TP + TN != 0) {
		double callback = (double)TP / (TP + TN);
		cout << "callback = " << callback << endl;
	} else {
		cout << "callback = NaN" << endl;
	}
	if (TP + FP != 0 && TP + TN != 0) {
		double accuracy = (double)TP / (TP + FP);
		double callback = (double)TP / (TP + TN);
		double F1score = 2 / (1 / accuracy + 1 / callback);
		cout << "F1 score = " << F1score << endl;
	} else {
		cout << "F1 score = NaN" << endl;
	}
}

bool Tester::Label2Bool(int label) {
	return label < 6;
}