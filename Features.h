#ifndef FEATURES
#define FEATURES

#include "FrameData.h"

class Features {
public:
	Features(FrameData** data, int frameCount, int pointIndex);
	~Features();
	bool IsValidShape(int shapeKind);

	long timeFromFirstToDown;
	bool firstFrameOnEdge;
	bool firstFrameFromEdge;
	int curSize;
	int edgeLength;

	

private:
	void FillArea(FrameData* frame, int x, int y, bool** area);
	void TryExpand(FrameData* frame, int gridX, int gridY, bool** area);
	void FindFirstFrame();
	void FillPreviousFrame(FrameData* frame, FrameData* prevFrame, bool** area, bool** tmpArea);
	void PrepareFirstFrame();

	int gridWidth;
	int gridHeight;
	int frameCount;
	
	int xSpan;
	int ySpan;
	FrameData** data;
	bool** area;
	
	int threshold;

	int downIndex;
	int firstFrameIndex;
	bool** firstFrameArea;
	
	
};

#endif