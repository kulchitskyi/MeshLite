#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders/shader.h"
#include "model/model.h"
#include "scene/sceneCondition.h"

class Renderer
{
private:
	std::shared_ptr<GLFWwindow> window;
	uint16_t screenWidth = 860;
	uint16_t screenHeight = 640;

	std::shared_ptr<Scene> scene;
	std::shared_ptr<Model> model;
	std::shared_ptr<Shader> modelShader;

public:
	Renderer(std::shared_ptr<Scene> _scene);
	std::shared_ptr<GLFWwindow> GetGLFWwindow();
	GLFWwindow* GetGLFWwindowRawPtr();
	void GLFWititialize(uint8_t openGLmajor = 4, uint8_t openGLminor = 4);
	void CreateGLFWwindow(uint16_t _screenWidth = 860, uint16_t _screenHeight = 640);
	void EnableFaceCulling();
	void SetShaders(std::shared_ptr<Shader> _modelShader);
	void SetModel(std::shared_ptr<Model> _model);
	void UpdateModelShader();
	void UpdateModelProjection();
	void ClearBuffers();
	void RenderNewFrame();
	void Terminate();
};