#pragma once
#include "model_IO/model_IO_Factory.h"

#include <memory>

class Scene;

struct GLFWwindow;

class Controller
{
private:
	std::unique_ptr<Model_IO::Model_IO_Factory> IO_factory;
public:
	GLFWwindow* _window;
	std::shared_ptr<Scene> _scene;
	void simplifySelectedModels(uint32_t targetVertexCount);
	Controller(std::shared_ptr<Scene> scene, GLFWwindow* window);
	~Controller();
};