#pragma once

#include <memory>

class GUIController;
struct GLFWwindow;

class GUI 
{
private:
	std::shared_ptr<GUIController> controller;

	float _modelColor[3];
	float _lightIntencity;
	float _lightDirection[3] = {-0.2f, -1.0f, -0.3f};
	float _modelSize = 1.0f;
	int   _vertexCountLimit = 3;

public:
    GUI(std::shared_ptr<GUIController> _controller);
	void initialize(GLFWwindow* window);
	void setStyle();
	void setNewFrame();
	void setWindowPos();
    void setMenu();
	void renderInterface();
    void clean();
};