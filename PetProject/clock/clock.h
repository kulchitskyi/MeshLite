#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Clock
{
private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
public:
	void UpdateDeltaTime();
	float GetDeltaTime();
};