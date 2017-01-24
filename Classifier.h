#ifndef CLASSIFIER
#define CLASSIFIER

#include "FrameData.h"

class Classifier {
public:
	Classifier();
	vector<bool> Classify(FrameData* data);
	~Classifier();

private:
	bool ClassifySinglePoint(FrameData** data, int frameCount, int index);
	bool hasPreviousFrame();
	FrameData* GetPreviousFrame();
	void StoreFrame(FrameData* data);
	int NextIndex(int cur);
	int PreviousIndex(int cur);

	FrameData** circularVector;
	int start;
	int end;
	int capacity;
	int cur;
	int maxStorage;
	long* lastTime;
	bool* lastJudge;
	int maxTouchID;
};

#endif