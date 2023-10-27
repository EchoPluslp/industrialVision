#pragma once
// SharedData.h

#include <mutex>

struct Dimension {
	int width;
	int height;
};

extern Dimension dimensions;
extern std::mutex mtx;
extern bool readyToModify;
extern std::condition_variable Setcv;
