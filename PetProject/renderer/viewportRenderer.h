#pragma once
#include "shaders/shader.h"
#include "model/model.h"
#include "scene/sceneCondition.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Renderer
{
private:
	std::shared_ptr<GLFWwindow> window;
	uint16_t screenWidth = 860;
	uint16_t screenHeight = 640;

	std::shared_ptr<Scene> scene;
	Shader modelShader;
	Shader pickingShader;
	static GLuint pickingFBO, pickingTexture, depthBuffer;

public:
	Renderer(std::shared_ptr<Scene> _scene);
	std::shared_ptr<GLFWwindow> getGLFWwindow();
	GLFWwindow* getGLFWwindowRawPtr();
	void ititializeGLFW(uint8_t openGLmajor = 4, uint8_t openGLminor = 4);
	void createGLFWwindow();
	void enableFaceCulling();
	void setShaders(Shader &_modelShader);
	void addModel(std::shared_ptr<AssimpLoader::Model> _model);
	void updateModelShader(std::shared_ptr<AssimpLoader::Model> model);
	void updateModelProjection(std::shared_ptr<AssimpLoader::Model> model);
	void renderModels();
	static void initPickingFramebuffer(int width, int height);
	auto performPicking(int mouseX, int mouseY) -> int;
	void clearBuffers();
	void renderNewFrame();
	void terminate();
};