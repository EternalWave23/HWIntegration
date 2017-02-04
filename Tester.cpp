#include "Tester.h"

Tester::Tester() {
	TP = 0;
	TN = 0;
	FP = 0;
	FN = 0;
}

void Tester::ListFiles(string path, vector<string>* files ) {  
    long hFile = 0;  
    struct _finddata_t fileinfo;  
    string p;  
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {  
		do {  
            if ((fileinfo.attrib &  _A_SUBDIR)) {  
                if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0) {
                    ListFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
            } else {  
                files->push_back(p.assign(path).append("\\").append(fileinfo.name));  
            }  
        } while (_findnext(hFile, &fileinfo) == 0);  
        _findclose(hFile);  
    }  
}  

void Tester::Test() {
	vector<string>* files = new vector<string>();  
	ListFiles("F:\\Huawei\\Data\\test", files);  

	for (int i = 0; i < files->size(); i++) { 
		cout << "File No. " << i << endl;
	//for (int i = 0; i < 1; i++) { 
		TestSingleFile((*files)[i]);
	}  
	delete files;
}

void Tester::TestSingleFile(string fileName) {
	//int i = 1;
	Reader* reader = new Reader(fileName);
	Classifier* cls = new Classifier();

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
	delete reader;
	delete cls;
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