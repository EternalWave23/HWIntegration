#include "Reader.h"

using namespace std;

Reader::Reader(string fileName) {
	fileIndex = 0;
	nextFrameIndex = 0;
	hasNextFrame = false;
	allData = new vector<vector<FrameData*>*>();
	ReadSingleFile(fileName);
}

Reader::~Reader() {
	for (int i = 0; i < allData->size(); i++) {
		for (int j = 0; j < (*allData)[i]->size(); j++) {
			delete (*(*allData)[i])[j];
		}
		delete (*allData)[i];
	}
	delete allData;
}

void Reader::ReadSingleFile(string fileName) {
	cout << fileName.c_str() << endl;
	vector<string>* rawData = new vector<string>();
	char buffer[3000];  
	ifstream in(fileName);  
    if (!in.is_open()) { 
		cout << "Error opening file"; 
		exit(1); 
	}  
    while (!in.eof()) {
        in.getline(buffer, 3000); 
		if (strlen(buffer) > 1) {
			rawData->push_back(buffer);
		}
    }
	in.close();
	//cout << rawData->size() << endl;
	Parse(rawData);
	delete rawData;
}

void Reader::Parse(vector<string>* rawData) {
	vector<FrameData*>* frames = new vector<FrameData*>();
	for (int i = 0; i < rawData->size(); i++) {
	//for (int i = 0; i < 1; i++) {
		vector<string>* tokens = new vector<string>();
		Split((*rawData)[i], ',', tokens);
		int time = atoi((*tokens)[0].c_str());
		bool found = false;
		if (frames->size() > 0) {
			if ((*frames)[frames->size() - 1]->time == time) {
				found = true;
			}
		}
		FrameData* data = 0;
		if (found) {
			data = (*frames)[frames->size() - 1];
		} else {
			data = new FrameData();
			data->time = time;
			vector<string>* capData = new vector<string>();
			Split((*tokens)[8], ';', capData);
			for (int j = 0; j < 28; j++) {
				for (int k = 0; k < 16; k++) {
					data->capacity[j][k] = atoi((*capData)[j + k * 28].c_str());
					if (data->capacity[j][k] < 0) {
						data->capacity[j][k] = -data->capacity[j][k];
					}
				}
			}
			delete capData;
			frames->push_back(data);
		}
		if (strcmp((*tokens)[2].c_str(), "True") == 0) {
			data->touchID.push_back(atoi((*tokens)[3].c_str()));
			data->touchX.push_back(atoi((*tokens)[4].c_str()));
			data->touchY.push_back(atoi((*tokens)[5].c_str()));
			data->intented.push_back(strcmp((*tokens)[6].c_str(), "True") == 0);
			if (strcmp((*tokens)[7].c_str(), "SWIPEMID") == 0) {
				data->label.push_back(1);
			} else if (strcmp((*tokens)[7].c_str(), "SWIPEEDGE") == 0) {
				data->label.push_back(2);
			} else if (strcmp((*tokens)[7].c_str(), "TOUCHMID") == 0) {
				data->label.push_back(3);
			} else if (strcmp((*tokens)[7].c_str(), "TOUCHNEAR") == 0) {
				data->label.push_back(4);
			} else if (strcmp((*tokens)[7].c_str(), "TOUCHEDGE") == 0) {
				data->label.push_back(5);
			} else if (strcmp((*tokens)[7].c_str(), "MOVEBIG") == 0) {
				data->label.push_back(6);
			} else if (strcmp((*tokens)[7].c_str(), "GRIPBIG") == 0) {
				data->label.push_back(7);
			} else if (strcmp((*tokens)[7].c_str(), "GRIPFINGER") == 0) {
				data->label.push_back(8);
			} else { // QUARTERGRIP
				data->label.push_back(9);
			}
			data->isDown.push_back(false);
		}
		delete tokens;
	}
	cout << frames->size() << " frames" << endl;

	SplitTouchEvent(frames);
	allData->push_back(frames);
	if (allData->size() > 0) {
		hasNextFrame = true;
	}
}

bool Reader::HasNextFrame() {
	return hasNextFrame;
}

void Reader::Split(string data, char delimiter, vector<string>* result) {
	int start = 0;
	for (int end = 0; end < data.length(); end++) {
		if (data[end] == delimiter) {
			if (end - 1 >= start) {
				result->push_back(data.substr(start, end - start));
			}
			start = end + 1;
		} else if (end == data.length() -1) {
			result->push_back(data.substr(start, end - start + 1));
		}
	}
}

void Reader::SplitTouchEvent(vector<FrameData*>* frames) {
	long lastTime[20];
	for (int i = 0; i < 20; i++) {
		lastTime[i] = -10000;
	}

	for (int i = 0; i < frames->size(); i++) {
		for (int j = 0; j < (*frames)[i]->touchID.size(); j++) {
			int ID = (*frames)[i]->touchID[j];
			if (ID >= 0 && ID < 20) {
				if ((*frames)[i]->time - lastTime[ID] > 20) {
					(*frames)[i]->isDown[j] = true;
				}
				lastTime[ID] = (*frames)[i]->time;
			}
		}
	}
}

FrameData* Reader::NextFrame() {
	int tmpFileIndex = fileIndex;
	int tmpNextFrameIndex = nextFrameIndex;

	if (nextFrameIndex == (*allData)[fileIndex]->size() - 1) {
		if (fileIndex == allData->size() - 1) {
			hasNextFrame = false;
		} else {
			fileIndex++;
			nextFrameIndex = 0;
		}
	} else {
		nextFrameIndex++;
	}
	//cout << tmpFileIndex << ", " << tmpNextFrameIndex << ", " << allData->size() << ", " << (*(*allData)[tmpFileIndex]).size() << endl;
	return (*(*allData)[tmpFileIndex])[tmpNextFrameIndex];
}