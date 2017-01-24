#ifndef READER
#define READER

#include <vector>
#include <iostream>
#include <io.h>
#include <fstream>
#include "FrameData.h"

using namespace std;

class Reader {
public:
	Reader();
	void ReadFile(string fileName);
	bool HasNextFrame();

	FrameData* NextFrame();

private:
	void ListFiles(string path, vector<string>* files); 
	void ReadSingleFile(string fileName);
	void Parse(vector<string>* rawData);
	void Split(string data, char delimiter, vector<string>* result);
	void SplitTouchEvent(vector<FrameData*>* frames);

	vector<vector<FrameData*>*>* allData;
	int fileIndex;
	int nextFrameIndex;
	bool hasNextFrame;
};

#endif