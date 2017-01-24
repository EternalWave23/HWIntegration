#include "FrameData.h"

FrameData::FrameData() {
	for (int i = 0; i < 28; i++) {
		for (int j = 0; j < 16; j++) {
			capacity[i][j] = 0;
		}
	}
}