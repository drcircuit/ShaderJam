#include "Timer.h"

float divisor = 1000000000.0;
void Timer::Initialize() {
	startTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	lastTime = startTime;
}

float Timer::Ellapsed() {
	auto current =  std::chrono::high_resolution_clock::now().time_since_epoch().count() - startTime;
	lastTime = current;
	return current / divisor ;
}

float Timer::GetStartTime() {
	return startTime / divisor;
}

float Timer::GetFrameTime() {
	auto current = std::chrono::high_resolution_clock::now().time_since_epoch().count() - startTime;
	auto frameTime = current - lastTime;
	return frameTime/ divisor;
}

void Timer::Restart() {
	startTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	lastTime = startTime;
}