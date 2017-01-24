#include "Features.h"
#include <iostream>
using namespace std;

Features::Features(FrameData** data, int frameCount, int pointIndex) {
	int gridWidth = 1080 / 16;
	int gridHeight = 1920 / 28;
	FrameData* cur = data[0];
	int downX = cur->touchX[pointIndex];
	int downY = cur->touchY[pointIndex];
	for (int i = 0; i < 28; i++) {
		for (int j = 0; j < 16; j++) {
			area[i][j] = false;
		}
	}
	FillArea(cur, downX, downY);
	/*cout << "capacity:" << endl;
	for (int i = 0; i < 28; i ++) {
		for (int j = 0; j < 16; j++) {
			cout << cur->capacity[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "area:" << endl;
	for (int i = 0; i < 28; i ++) {
		for (int j = 0; j < 16; j++) {
			cout << area[i][j] << "\t";
		}
		cout << endl;
	}*/
}

void Features::TryExpand(FrameData* frame, int gridX, int gridY) {
	int left = gridX - 1;
	if (left >= 0) {
		if (!area[left][gridY] && frame->capacity[left][gridY] > threshold) {
			area[left][gridY] = true;
			TryExpand(frame, left, gridY);
		}
	}
	int right = gridX + 1;
	if (right < 16) {
		if (!area[right][gridY] && frame->capacity[right][gridY] > threshold) {
			area[right][gridY] = true;
			TryExpand(frame, right, gridY);
		}
	}
	int up = gridY - 1;
	if (up >= 0) {
		if (!area[gridX][up] && frame->capacity[gridX][up] > threshold) {
			area[gridX][up] = true;
			TryExpand(frame, gridX, up);
		}
	}
	int down = gridY + 1;
	if (down < 28) {
		if (!area[gridX][down] && frame->capacity[gridX][down] > threshold) {
			area[gridX][down] = true;
			TryExpand(frame, gridX, down);
		}
	}
}

void Features::FillArea(FrameData* frame, int x, int y) {
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
	if (frame->capacity[gridX][gridY] > threshold) {
		area[gridX][gridY] = true;
		TryExpand(frame, gridX, gridY);
	}
}