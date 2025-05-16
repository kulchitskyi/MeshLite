#include "keyboard.h"
#include "camera/camera.h"

#include <iostream>


Keyboard::Keyboard(std::shared_ptr <InputController> _controller)
    : controller(_controller)
    {}

bool Keyboard::isKeyPressed(int16_t keyCode)
{
    return (glfwGetKey(controller->getGLFWwindow(), keyCode) == GLFW_PRESS);
}

bool Keyboard::wasKeyPressed(int16_t keyCode) {
    bool nowPressed = isKeyPressed(keyCode);
    bool& prevPressed = previousKeyState[keyCode];

    bool wasJustPressed = nowPressed && !prevPressed;
    prevPressed = nowPressed;

    return wasJustPressed;
}

void Keyboard::processInput()
{
    if (isKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(controller->getGLFWwindow(), true);

    if (isKeyPressed(GLFW_KEY_W))
        controller->processCameraMovement(CameraMovement::FORWARD);
    if (isKeyPressed(GLFW_KEY_S))
        controller->processCameraMovement(CameraMovement::BACKWARD);
    if (isKeyPressed(GLFW_KEY_A))
        controller->processCameraMovement(CameraMovement::LEFT);
    if (isKeyPressed(GLFW_KEY_D))
        controller->processCameraMovement(CameraMovement::RIGHT);

    if (wasKeyPressed(GLFW_KEY_C) && isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        controller->processObjectCopy();
}