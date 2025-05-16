#include "inputController.h"
#include "camera/camera.h"
#include "clock/clock.h"
#include "renderer/viewportRenderer.h"
#include "controls/controller.h"
#include "commands/command.h"

#include <iostream>

InputController::InputController(std::shared_ptr<GLFWwindow> window, 
	std::shared_ptr<Camera> camera, 
	std::shared_ptr<Clock> clock, 
	std::shared_ptr<Renderer> renderer, 
	std::shared_ptr<Controller> controller) :
	_window(window),
	_camera(camera),
	_clock(clock),
	_renderer(renderer),
	_controller(controller) 
{}

void InputController::processObjectCopy()
{
	CopyCommand copyCommand;
	copyCommand.setScene(_controller->_scene);
	copyCommand.execute();
}


void InputController::processCameraMovement(CameraMovement direction)
{
	_camera->processKeyboard(direction, _clock->getDeltaTime());
}

void InputController::processCameraDirection(float xoffset, float yoffset)
{
	_camera->processMouseMovement(xoffset, yoffset);
}

void InputController::processCameraZoom(float offset)
{
	_camera->processMouseScroll(offset);
}

void InputController::processModelMovement(float xoffset, float yoffset)
{
	auto& camera = _controller->_scene->camera;
	for (auto& model : _controller->_scene->selectedModels)
	{
		model->modelPosition += camera->right * xoffset * mouseSensitivity;
		model->modelPosition += camera->up * yoffset * mouseSensitivity;
	}
}

void InputController::processPicking(float xmouse, float ymouse)
{
	 auto modelId = _renderer->performPicking(static_cast<int>(xmouse), static_cast<int>(ymouse));

	 _controller->_scene->selectedModels.clear();
	 if (modelId == -1)
	 {
		 return;
	 }
	_controller->_scene->selectModel(modelId);
}

auto InputController::getGLFWwindow() -> GLFWwindow*
{
	return _window.get();
}

InputController::~InputController() = default;
