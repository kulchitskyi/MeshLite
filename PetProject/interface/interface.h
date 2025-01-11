#pragma once

#include <algorithm>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "interfaceController.h"

class GUI
{
private:
	std::shared_ptr<Controller> controller;

	float modelColor[3]; //rewrite
	float lightIntencity;
	float lightDirection[3] = {-0.2f, -1.0f, -0.3f};
	float modelSize;
	float edgeLimit;


public:
    GUI(std::shared_ptr<Controller> _controller);
	void Initialize(GLFWwindow* window);
	void SetStyle();
	void SetNewFrame();
	void SetWindowPos();
    void SetMenu();
	void RenderInterface();
    void Clean();
};