#pragma once
#include <chrono>

class Timer {
public:
	void Initialize();
	float Ellapsed();
	float GetStartTime();
	float GetFrameTime();
	void Restart();
private:
	float startTime;
	float lastTime;
	
};