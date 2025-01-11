#include "viewportRenderer.h"
#include "camera/camera.h"

namespace Callback
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}


Renderer::Renderer(std::shared_ptr<Scene>_scene)
	: scene(_scene)
	{
		GLFWititialize();
		CreateGLFWwindow(1920, 1080);
		EnableFaceCulling();
		modelShader = std::make_shared<Shader>("vs.txt", "fs.txt");
		model = std::make_shared<Model>("D:/laystall/vase.obj");
	}

std::shared_ptr<GLFWwindow> Renderer::GetGLFWwindow()
{
	return window;
}

GLFWwindow* Renderer::GetGLFWwindowRawPtr()
{
	return window.get();
}

void Renderer::GLFWititialize(uint8_t openGLmajor, uint8_t openGLminor)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLmajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLminor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Renderer::CreateGLFWwindow(uint16_t _screenWidth, uint16_t _screenHeight)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	GLFWwindow* _window = glfwCreateWindow(screenWidth, screenHeight, "Lowpoly generator", nullptr, nullptr);
	window = std::shared_ptr<GLFWwindow>(_window, [](GLFWwindow* window) {
		glfwDestroyWindow(window);
		});
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window.get());
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{

		std::cout << "Failed to initialize GLAD\n";
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(window.get(), Callback::framebuffer_size_callback);
	glfwSetCursorPos(window.get(), screenWidth / 2, screenHeight / 2); //fffffffffffffffu
}


void Renderer::EnableFaceCulling()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Renderer::SetShaders(std::shared_ptr<Shader> _modelShader)
{
	modelShader = _modelShader;
}

void Renderer::SetModel(std::shared_ptr<Model> _model)
{
	model = _model;
}

void Renderer::UpdateModelShader()
{
	modelShader->use();
	glm::mat4 projection = glm::perspective(glm::radians(scene->camera->Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glm::mat4 view = scene->camera->GetViewMatrix();
	modelShader->setMat4("projection", projection);
	modelShader->setMat4("view", view);
	modelShader->setVec3("viewPos", scene->camera->Position);
	modelShader->setVec3("dirLight.direction", scene->lightDirection);
	modelShader->setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
	modelShader->setVec3("dirLight.diffuse", scene->lightIntencity);
	modelShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	modelShader->setVec4("color", model->modelColor);
}

void Renderer::UpdateModelProjection()
{
	glm::mat4 modelProjection = glm::mat4(1.0f);
	modelProjection = glm::translate(modelProjection, glm::vec3(0.0f, 0.0f, 0.0f));
	modelProjection = glm::scale(modelProjection, glm::vec3(model->modelSize, model->modelSize, model->modelSize));
	modelShader->setMat4("model", modelProjection);
	model->Draw(*modelShader);
}
  
void Renderer::ClearBuffers()
{
	glClearColor(0.09f, 0.07f, 0.07f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderNewFrame()
{
	glfwSwapBuffers(window.get());
	glfwPollEvents();
}

void Renderer::Terminate()
{
	glfwTerminate();
}


