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
	Reader(string fileName);
	~Reader();
	bool HasNextFrame();

	FrameData* NextFrame();

private:
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