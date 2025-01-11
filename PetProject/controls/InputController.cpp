#include "InputController.h"
#include "camera/camera.h"
#include "clock/clock.h"

#include <iostream>

InputController::InputController(std::shared_ptr<GLFWwindow> _window,
	std::shared_ptr<Camera> _camera,
	std::shared_ptr<Clock> _clock)
	: window(_window),
	camera(_camera),
	clock(_clock)
{}

void InputController::Undo()
{

}

void InputController::ResizeObject()
{

}

void InputController::ProcessCameraMovement(CameraMovement direction)
{
	camera->ProcessKeyboard(direction, clock->GetDeltaTime());
}

void InputController::ProcessCameraDirection(float xoffset, float yoffset)
{
	camera->ProcessMouseMovement(xoffset, yoffset);
}

void InputController::ProcessCameraZoom(float offset)
{
	camera->ProcessMouseScroll(offset);
}

GLFWwindow* InputController::GetGLFWwindow()
{
	return window.get();
}