#include "Classifier.h"
#include "Features.h"
#include <iostream>

using namespace std;

Classifier::Classifier() {
	maxStorage = 50;
	circularVector = new FrameData*[maxStorage];
	start = 0;
	end = -1;
	capacity = 0;
	maxTouchID = 20;
	lastTime = new long[maxTouchID];
	for (int i = 0; i < maxTouchID; i++) {
		lastTime[i] = -10000;
	}
	lastJudge = new bool[maxTouchID];
}

Classifier::~Classifier() {
	delete []circularVector;
	delete []lastTime;
	delete []lastJudge;
}

vector<bool> Classifier::Classify(FrameData* data) {
	vector<bool> result;
	StoreFrame(data);
	FrameData** sorted = new FrameData*[maxStorage]; // in descending order of time
	sorted[0] = data;
	int validNum = 1;
	while (hasPreviousFrame()) {
		sorted[validNum] = GetPreviousFrame();
		validNum++;
	}
	for (int i = 0; i < data->touchID.size(); i++) {
		result.push_back(ClassifySinglePoint(sorted, validNum, i));
	}
	delete []sorted;
	for (int i = 0; i < data->touchID.size(); i++) {
		result.push_back(false);
	}
	return result;
}

bool Classifier::ClassifySinglePoint(FrameData** data, int frameCount, int pointIndex) {
	long tmpTime = lastTime[data[0]->touchID[pointIndex]];
	lastTime[data[0]->touchID[pointIndex]] = data[0]->time;
	if (data[0]->time - tmpTime < 20) {
		return lastJudge[data[0]->touchID[pointIndex]];
	}

	bool result = true;
	Features* feature = new Features(data, frameCount, pointIndex);
	if (feature->firstFrameOnEdge) {
		if (feature->firstFrameFromEdge) {
			if (feature->timeFromFirstToDown < 400 && feature->curSize <= 25 && feature->IsValidShape(1)) {
                result = false;
			} else {
				result = true; 
			}
		} else {
			if (feature->IsValidShape(2)) {
                result = false;
			} else {
				result = true; 
			}
		}
	} else {
		if (!feature->IsValidShape(3)) {
            result = true; 
		} else {
			result = false;
		}
	}

	delete feature;
	return result;
}

int Classifier::NextIndex(int cur) {
	if (cur < maxStorage - 1) {
		return cur + 1;
	} else {
		return 0;
	}
}

int Classifier::PreviousIndex(int cur) {
	if (cur == 0) {
		return maxStorage - 1;
	} else {
		return cur - 1;
	}
}

void Classifier::StoreFrame(FrameData* data) {
	end = NextIndex(end);
	circularVector[end] = data;
	if (capacity == maxStorage) {
		start = NextIndex(start);
	}
	if (capacity < maxStorage) {
		capacity++;
	}
	cur = end;
}

bool Classifier::hasPreviousFrame() {
	if (end < 0) {
		return false;
	}
	return cur != start;
}

FrameData* Classifier::GetPreviousFrame() {
	cur = PreviousIndex(cur);
	return circularVector[cur];
}