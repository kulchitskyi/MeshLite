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
#include "controls/InputController.h"
#include "controls/InputDevice.h"


int main()
{
    auto camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 13.0f));
    auto clock = std::make_shared<Clock>();
    auto scene = std::make_shared<Scene>(camera);
    auto renderer = std::make_shared<Renderer>(scene);

    auto modelShader = std::make_shared<Shader>("shaders/vs.txt", "shaders/fs.txt");
    renderer->SetShaders(modelShader);
    auto model = std::make_shared<Model>("D:/laystall/vase.obj");
    renderer->SetModel(model);

    auto interfaceController = std::make_shared<Controller>(model, modelShader, scene);
    GUI graphicalUserInterface(interfaceController);
    
    auto inputController = std::make_shared<InputController>(renderer->GetGLFWwindow(), camera, clock);
    Keyboard keyboard(inputController);
    Mouse mouse(inputController);

    mouse.SetCallbacks();

    graphicalUserInterface.Initialize(renderer->GetGLFWwindowRawPtr());
    graphicalUserInterface.SetStyle();
    graphicalUserInterface.SetWindowPos();

    while (!glfwWindowShouldClose(renderer->GetGLFWwindowRawPtr()))
    {
        clock->UpdateDeltaTime();
        keyboard.processInput();
        renderer->ClearBuffers();
        graphicalUserInterface.SetWindowPos();
        graphicalUserInterface.SetNewFrame();
        graphicalUserInterface.SetMenu();

        renderer->UpdateModelShader();
        renderer->UpdateModelProjection();

        graphicalUserInterface.RenderInterface();
        renderer->RenderNewFrame();
    }
    graphicalUserInterface.Clean();
    renderer->Terminate();
    return 0;
}