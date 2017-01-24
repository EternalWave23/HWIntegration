#ifndef FEATURES
#define FEATURES

#include "FrameData.h"

class Features {
public:
	Features(FrameData** data, int frameCount, int pointIndex);
	~Features();

private:
	void FillArea(FrameData* frame, int x, int y);
	void TryExpand(FrameData* frame, int gridX, int gridY);

	bool area[28][16];
	int gridWidth;
	int gridHeight;
	int threshold;
};

#endif