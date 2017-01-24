#ifndef FRAMEDATA
#define FRAMEDATA

#include <vector>

using namespace std;

class FrameData {
public:
	long time;
	vector<int> touchID;
	vector<int> touchX;
	vector<int> touchY;
	vector<bool> isDown;
	vector<int> intented;
	vector<int> label;
	int capacity[28][16];

	FrameData();
};
#endif