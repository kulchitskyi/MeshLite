#include "clock.h"

void Clock::UpdateDeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

float Clock::GetDeltaTime()
{
	return deltaTime;
}
