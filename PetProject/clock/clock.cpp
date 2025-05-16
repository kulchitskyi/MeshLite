#include "clock.h"

void Clock::updateDeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

auto Clock::getDeltaTime() -> float
{
	return _deltaTime;
}
