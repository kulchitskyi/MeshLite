#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera;
class Clock;
enum class CameraMovement;


class InputController
{
private:
	std::shared_ptr<GLFWwindow> window;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Clock> clock;
public:
	InputController(std::shared_ptr<GLFWwindow> _window, 
		std::shared_ptr<Camera> _camera,
		std::shared_ptr<Clock> _clock);
	void Undo();
	void ResizeObject();
	void ProcessCameraMovement(CameraMovement direction);
	void ProcessCameraDirection(float xoffset, float yoffset);
	void ProcessCameraZoom(float offset);
	GLFWwindow* GetGLFWwindow();
};