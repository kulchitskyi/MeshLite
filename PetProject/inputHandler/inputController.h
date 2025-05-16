#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

class Camera;
class Clock;
class Renderer;
class Controller;

enum class CameraMovement;

class InputController final
{
private:
	std::shared_ptr<GLFWwindow> _window;
	std::shared_ptr<Camera>		_camera;
	std::shared_ptr<Clock>		_clock;
	std::shared_ptr<Renderer>	_renderer;
	std::shared_ptr<Controller> _controller;
	float mouseSensitivity = 0.01f;
public:
	InputController(std::shared_ptr<GLFWwindow> window,
		std::shared_ptr<Camera> camera,
		std::shared_ptr<Clock> clock,
		std::shared_ptr<Renderer> renderer,
		std::shared_ptr<Controller> controller);

	void processCameraMovement(CameraMovement direction);
	void processCameraDirection(float xoffset, float yoffset);
	void processCameraZoom(float offset);
	void processModelMovement(float xoffset, float yoffset);
	void processPicking(float xoffset, float yoffset);
	void processObjectCopy();
	
	auto getGLFWwindow() -> GLFWwindow*;
	
	~InputController();
};