#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "interface/interface.h"
#include "shaders/shader.h"
#include "camera/camera.h"
#include "model/model.h"
#include "renderer/viewportRenderer.h"
#include "clock/clock.h"

#include "interface/interfaceController.h"
#include "inputHandler/inputController.h"
#include "inputHandler/mouse.h"
#include "inputHandler/keyboard.h"

#include "controls/Controller.h"



int main(){
    auto camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 13.0f));
    auto clock = std::make_shared<Clock>(); 
    auto scene = std::make_shared<Scene>(camera);
    auto renderer = std::make_shared<Renderer>(scene);
    auto controller = std::make_shared<Controller>(scene, renderer->getGLFWwindowRawPtr());

    auto modelShader = Shader{ "shaders/vs.txt", "shaders/fs.txt" };
    renderer->setShaders(modelShader);

    auto interfaceController = std::make_shared<GUIController>(scene, controller);
    GUI graphicalUserInterface(interfaceController);
    
    auto inputController = std::make_shared<InputController>(renderer->getGLFWwindow(), camera, clock, renderer, controller);
    Keyboard keyboard(inputController);
    Mouse mouse(inputController);

    mouse.setCallbacks();

    graphicalUserInterface.initialize(renderer->getGLFWwindowRawPtr());
    graphicalUserInterface.setStyle();
    graphicalUserInterface.setWindowPos();

    while (!glfwWindowShouldClose(renderer->getGLFWwindowRawPtr())) {
        clock->updateDeltaTime();
        keyboard.processInput();

        renderer->clearBuffers();

        graphicalUserInterface.setWindowPos();
        graphicalUserInterface.setNewFrame();
        graphicalUserInterface.setMenu();

        renderer->renderModels();

        graphicalUserInterface.renderInterface();
        renderer->renderNewFrame();
    }
    graphicalUserInterface.clean();
    renderer->terminate();
    return 0;
}