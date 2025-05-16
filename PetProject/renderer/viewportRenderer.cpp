#include "viewportRenderer.h"
#include "camera/camera.h"
#include "utils/utils.h"

GLuint Renderer::pickingFBO = 0;
GLuint Renderer::pickingTexture = 0;
GLuint Renderer::depthBuffer = 0;

namespace Callback
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer::initPickingFramebuffer(width, height);

		glViewport(0, 0, width, height);
	}
}


Renderer::Renderer(std::shared_ptr<Scene>_scene)
	: scene(_scene)
	{
		ititializeGLFW();
		createGLFWwindow();
		enableFaceCulling();
		modelShader = Shader{ "shaders/vs.txt", "shaders/fs.txt" };
		pickingShader = Shader{ "shaders/pickingVS.txt", "shaders/pickingFS.txt" };
		initPickingFramebuffer(1920, 1080);
	}

std::shared_ptr<GLFWwindow> Renderer::getGLFWwindow()
{
	return window;
}

GLFWwindow* Renderer::getGLFWwindowRawPtr()
{
	return window.get();
}

void Renderer::ititializeGLFW(uint8_t openGLmajor, uint8_t openGLminor)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLmajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLminor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Renderer::createGLFWwindow()
{

	screenWidth = 1920;
	screenHeight = 1080;
	
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* _window = glfwCreateWindow(1920, 1080, "cursach", nullptr, nullptr);
	glfwSetWindowPos(_window, 0, 0);

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
	glfwSetCursorPos(window.get(), screenWidth / 3, screenHeight / 3);
}


void Renderer::enableFaceCulling()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_STENCIL_TEST);

	glCullFace(GL_BACK);
}

void Renderer::setShaders(Shader &_modelShader)
{
	modelShader = _modelShader;
}

void Renderer::addModel(std::shared_ptr<AssimpLoader::Model> _model)
{
	
	scene->models.push_back(_model);
}

void Renderer::updateModelShader(std::shared_ptr<AssimpLoader::Model> model)
{
	modelShader.use();
	glm::mat4 view = scene->camera->getViewMatrix();
	modelShader.setMat4("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(scene->camera->zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	modelShader.setMat4("projection", projection);
	
	modelShader.setVec3("viewPos", scene->camera->position);
	modelShader.setVec3("dirLight.direction", scene->lightDirection);
	modelShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
	modelShader.setVec3("dirLight.diffuse", scene->lightIntencity);
	modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	modelShader.setVec4("color", model->modelColor);
	
	int id = std::distance(scene->models.begin(),
		std::find(scene->models.begin(), scene->models.end(), model)
	);
	
}

void Renderer::updateModelProjection(std::shared_ptr<AssimpLoader::Model> model)
{
	glm::mat4 modelProjection = glm::mat4(1.0f);
	modelProjection = glm::translate(modelProjection, model->modelPosition);
	modelProjection = glm::scale(modelProjection, glm::vec3(model->modelSize, model->modelSize, model->modelSize));
	modelShader.setMat4("model", modelProjection);
}

void Renderer::renderModels()
{
	for (auto& model : scene->models)
	{
		updateModelProjection(model);
		updateModelShader(model);
		model->draw(modelShader);
	}

}

void Renderer::initPickingFramebuffer(int width, int height) 
{
	glGenFramebuffers(1, &pickingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);

	glGenTextures(1, &pickingTexture);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, pickingTexture, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int Renderer::performPicking(int mouseX, int mouseY) 
{
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	pickingShader.use();
	
	

	for (auto& model : scene->models) 
	{

		glm::mat4 modelProjection = glm::mat4(1.0f);
		modelProjection = glm::translate(modelProjection, model->modelPosition);
		modelProjection = glm::scale(modelProjection, glm::vec3(model->modelSize, model->modelSize, model->modelSize));
		pickingShader.setMat4("model", modelProjection);
		
		glm::mat4 projection = glm::perspective(glm::radians(scene->camera->zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = scene->camera->getViewMatrix();
		pickingShader.setMat4("projection", projection);
		pickingShader.setMat4("view", view);

		int id = std::distance(scene->models.begin(),
			std::find(scene->models.begin(), scene->models.end(), model)
		);

		pickingShader.setVec4("objectColor", scene->idToColor[id]);
		
		model->draw(pickingShader);
	}

	glFlush();
	glFinish();

	uint8_t data[3];
	glReadPixels(mouseX,  screenHeight - mouseY , 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, pickingFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
	glBlitFramebuffer(0, 0, screenWidth, screenHeight,
		0, 0, screenWidth, screenHeight,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	
	return Utils::colorToID(data[0], data[1], data[2]);
}

  
void Renderer::clearBuffers()
{
	glClearColor(0.09f, 0.07f, 0.07f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderNewFrame()
{
	glfwSwapBuffers(window.get());
	glfwPollEvents();
}

void Renderer::terminate()
{
	glfwTerminate();
}


