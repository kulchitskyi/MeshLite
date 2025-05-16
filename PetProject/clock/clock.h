#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Clock final
{
private:
	float _deltaTime = 0.0f;
	float _lastFrame = 0.0f;
public:
	void updateDeltaTime();
	auto getDeltaTime() -> float;
};