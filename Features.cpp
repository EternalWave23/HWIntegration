#include "Features.h"
#include <iostream>
using namespace std;

Features::Features(FrameData** data, int frameCount, int pointIndex) {
	this->data = data;
	gridWidth = 1080 / 16;
	gridHeight = 1920 / 28;
	downIndex = 0;
	this->frameCount = frameCount;

	FrameData* cur = data[downIndex];
	int downX = cur->touchX[pointIndex];
	int downY = cur->touchY[pointIndex];
	area = new bool*[28];
	for (int i = 0; i < 28; i++) {
		area[i] = new bool[16];
		for (int j = 0; j < 16; j++) {
			area[i][j] = false;
		}
	}
	//cout << "framecount = " << frameCount << endl;
	curSize = 0;
	FillArea(cur, downX, downY, area);
	FindFirstFrame();
	PrepareFirstFrame();
	/*cout << "capacity:" << endl;
	for (int i = 0; i < 28; i ++) {
		for (int j = 0; j < 16; j++) {
			cout << cur->capacity[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "area: " << downX << "," << downY << endl;
	for (int i = 0; i < 28; i ++) {
		for (int j = 0; j < 16; j++) {
			cout << area[i][j] << "\t";
		}
		cout << endl;
	}*/
}

Features::~Features() {
	for (int i = 0; i < 28; i++) {
		delete []area[i];
	}
	delete []area;

	if (firstFrameArea != area) {
		for (int i = 0; i < 28; i++) {
			delete []firstFrameArea[i];
		}
		delete []firstFrameArea;
	}
}

void Features::PrepareFirstFrame() {
	timeFromFirstToDown = 0;
	timeFromFirstToDown = data[downIndex]->time - data[firstFrameIndex]->time;
	//cout << "time = " << timeFromFirstToDown << endl;
	
	edgeLength = 0;
	int min = 100;
	int max = 0;
	firstFrameOnEdge = false;
	for (int i = 0; i < 28; i++) {
		if (firstFrameArea[i][0]) {
			firstFrameOnEdge = true;
			if (i < min) {
				min = i;
			}
			if (i > max) {
				max = i;
			}
		}
	}
	if (max - min < 100 && max - min + 1 > edgeLength) {
		edgeLength = max - min + 1;
	}
	min = 100;
	max = 0;
	for (int i = 0; i < 28; i++) {
		if (firstFrameArea[i][15]) {
			firstFrameOnEdge = true;
			if (i < min) {
				min = i;
			}
			if (i > max) {
				max = i;
			}
		}
	}
	if (max - min < 100 && max - min + 1 > edgeLength) {
		edgeLength = max - min + 1;
	}
	min = 100;
	max = 0;
	for (int i = 0; i < 16; i++) {
		if (firstFrameArea[0][i]) {
			firstFrameOnEdge = true;
			if (i < min) {
				min = i;
			}
			if (i > max) {
				max = i;
			}
		}
	}
	if (max - min < 100 && max - min + 1 > edgeLength) {
		edgeLength = max - min + 1;
	}
	min = 100;
	max = 0;
	for (int i = 0; i < 16; i++) {
		if (firstFrameArea[27][i]) {
			firstFrameOnEdge = true;
			if (i < min) {
				min = i;
			}
			if (i > max) {
				max = i;
			}
		}
	}
	if (max - min < 100 && max - min + 1 > edgeLength) {
		edgeLength = max - min + 1;
	}
	//cout << "first frame on edge = " << firstFrameOnEdge << endl;
	//cout << "edge length = " << edgeLength << endl;

	if (!firstFrameOnEdge) {
		firstFrameFromEdge = false;
	} else {
		double cumuX = 0;
		double cumuY = 0;
		int count = 0;
		for (int i = 0; i < 28; i++) {
			for (int j = 0; j < 16; j++) {
				if (firstFrameArea[i][j]) {
					cumuX += data[firstFrameIndex]->capacity[i][j] * j;
					cumuY += data[firstFrameIndex]->capacity[i][j] * i;
					count += data[firstFrameIndex]->capacity[i][j];
				}
			}
		}
		cumuX = cumuX / count * gridWidth + gridWidth / 2;
		cumuY = cumuY / count * gridHeight + gridHeight / 2;
		double revX = 1080 - cumuX;
		double revY = 1920 - cumuY;
		double minDis = cumuX;
		if (cumuY < minDis) {
			minDis = cumuY;
		}
		if (revX < minDis) {
			minDis = revX;
		}
		if (revY < minDis) {
			minDis = revY;
		}
		if (minDis < 0.9 * gridWidth) {
			firstFrameFromEdge = true;
		} else {
			firstFrameFromEdge = false;
		}
	}
	//cout << "first frame from edge = " << firstFrameFromEdge << endl;

	curSize = 0;
	int minX = 16;
	int maxX = 0;
	int minY = 28;
	int maxY = 0;
	for (int i = 0; i < 28; i++) {
		for (int j = 0; j < 16; j++) {
			if (area[i][j]) {
				curSize++;
				if (i < minY) {
					minY = i;
				}
				if (i > maxY) {
					maxY = i;
				}
				if (j < minX) {
					minX = j;
				}
				if (j > maxX) {
					maxX = j;
				}
			}
		}
	}
	xSpan = maxX - minX + 1;
	ySpan = maxY - minY + 1;
	//cout << "cur size = " << curSize << endl;
	//cout << "xSpan = " << xSpan << ", ySpan = " << ySpan << endl;
}

void Features::TryExpand(FrameData* frame, int gridX, int gridY, bool** area) {
	int left = gridX - 1;
	if (left >= 0) {
		if (!area[gridY][left] && frame->capacity[gridY][left] > threshold) {
			area[gridY][left] = true;
			TryExpand(frame, left, gridY, area);
		}
	}
	int right = gridX + 1;
	if (right < 16) {
		if (!area[gridY][right] && frame->capacity[gridY][right] > threshold) {
			area[gridY][right] = true;
			TryExpand(frame, right, gridY, area);
		}
	}
	int up = gridY - 1;
	if (up >= 0) {
		if (!area[up][gridX] && frame->capacity[up][gridX] > threshold) {
			area[up][gridX] = true;
			TryExpand(frame, gridX, up, area);
		}
	}
	int down = gridY + 1;
	if (down < 28) {
		if (!area[down][gridX] && frame->capacity[down][gridX] > threshold) {
			area[down][gridX] = true;
			TryExpand(frame, gridX, down, area);
		}
	}
}

void Features::FillArea(FrameData* frame, int x, int y, bool** area) {
	int max = 0;
	for (int i = 0; i < 28; i++) {
		for (int j = 0; j < 16; j++) {
			if (frame->capacity[i][j] > max) {
				max = frame->capacity[i][j];
			}
		}
	}
	if (max < 250) {
		threshold = 50;
	} else if (max < 2000) {
		threshold = max / 5;
	} else {
		threshold = 400;
	}
	int gridX = x / gridWidth;
	int gridY = y / gridHeight;
	if (gridX == 16) {
		gridX = 15;
	}
	if (gridY == 28) {
		gridY = 27;
	}
	//printf("x = %d, y = %d\n", gridX, gridY);
	if (frame->capacity[gridY][gridX] > threshold) {
		area[gridY][gridX] = true;
		TryExpand(frame, gridX, gridY, area);
	}
}

void Features::FindFirstFrame() {
	firstFrameIndex = 0;
	firstFrameArea = area;
	if (frameCount > 1) {
		bool found = false;
		for (int i = 0; i < 28; i++) {
			for (int j = 0; j < 16; j++) {
				if (area[i][j]) {
					found = true;
					break;
				}
			}
		}
		if (!found) {
			return;
		}
		while (firstFrameIndex < frameCount - 1) {
			//cout << "frame index = " << firstFrameIndex << endl;
			bool** tmpArea = new bool*[28];
			for (int i = 0; i < 28; i++) {
				tmpArea[i] = new bool[16];
				for (int j = 0; j < 16; j++) {
					tmpArea[i][j] = false;
				}
			}
			FillPreviousFrame(data[firstFrameIndex], data[firstFrameIndex + 1], firstFrameArea, tmpArea);
			bool flag = false;
			for (int i = 0; i < 28; i++) {
				for (int j = 0; j < 16; j++) {
					if (tmpArea[i][j]) {
						flag = true;
						break;
					}
				}
			}
			if (!flag) {
				for (int i = 0; i < 28; i++) {
					delete []tmpArea[i];
				}
				delete []tmpArea;
				break;
			} else {
				if (firstFrameArea != area) {
					for (int i = 0; i < 28; i++) {
						delete []firstFrameArea[i];
					}
					delete []firstFrameArea;
				}
				firstFrameArea = tmpArea;
				firstFrameIndex++;
			}
		}
	}
	//cout << "index = " << firstFrameIndex << endl;
}

void Features::FillPreviousFrame(FrameData* frame, FrameData* prevFrame, bool** area, bool** tmpArea) {
	double cumuX = 0;
	double cumuY = 0;
	int count = 0;
	for (int i = 0; i < 28; i++) {
		for (int j = 0; j < 16; j++) {
			if (area[i][j]) {
				cumuX += frame->capacity[i][j] * j;
				cumuY += frame->capacity[i][j] * i;
				count += frame->capacity[i][j];
			}
		}
	}
	//cout << "cumuX = " << cumuX << " cumuY = " << cumuY << " count = " << count <<endl;
	cumuX = cumuX / count * gridWidth + gridWidth / 2;
	cumuY = cumuY / count * gridHeight + gridHeight / 2;
	//cout << "cumuX = " << cumuX << " cumuY = " << cumuY << endl;
	FillArea(prevFrame, cumuX, cumuY, tmpArea); 
}

bool Features::IsValidShape(int shapeKind) {
	if (shapeKind == 1) { // fromedge
		return curSize <= 25 && xSpan / ySpan >= 0.5;
	} else if (shapeKind == 2) { // onedge
		return (ySpan/xSpan <= 3 && edgeLength < 10);
	} else { // middle
		return ySpan/xSpan <= 3 && edgeLength < 10;
	}
}
