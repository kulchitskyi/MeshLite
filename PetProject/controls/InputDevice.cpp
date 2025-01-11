#include "InputDevice.h"
#include "camera/camera.h"

#include <iostream>

void Mouse::mouse_callback_static(GLFWwindow* window, double xposIn, double yposIn) {
    Mouse* instance = static_cast<Mouse*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->mouse_callback(window, xposIn, yposIn);
    }
}

void Mouse::scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset) {
    Mouse* instance = static_cast<Mouse*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->scroll_callback(window, xoffset, yoffset);
    }
}

Mouse::Mouse(std::shared_ptr <InputController> _controller)
    : controller(_controller)
    {}

void Mouse::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - lastCursorXpos;
    float yoffset = lastCursorYpos - ypos;

    lastCursorXpos = xpos;
    lastCursorYpos = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        controller->ProcessCameraDirection(xoffset, yoffset);
    }
}

void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    controller->ProcessCameraZoom(static_cast<float>(yoffset));
}

void Mouse::SetCallbacks() {
    glfwSetWindowUserPointer(controller->GetGLFWwindow(), this);
    glfwSetCursorPosCallback(controller->GetGLFWwindow(), mouse_callback_static);
    glfwSetScrollCallback(controller->GetGLFWwindow(), scroll_callback_static);
}

Keyboard::Keyboard(std::shared_ptr <InputController> _controller)
    : controller(_controller)
    {}

bool Keyboard::isKeyPressed(int16_t keyCode)
{
    return (glfwGetKey(controller->GetGLFWwindow(), keyCode) == GLFW_PRESS);
}

void Keyboard::processInput()
{
    if (isKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(controller->GetGLFWwindow(), true);

    if (isKeyPressed(GLFW_KEY_W))
        controller->ProcessCameraMovement(CameraMovement::FORWARD);
    if (isKeyPressed(GLFW_KEY_S))
        controller->ProcessCameraMovement(CameraMovement::BACKWARD);
    if (isKeyPressed(GLFW_KEY_A))
        controller->ProcessCameraMovement(CameraMovement::LEFT);
    if (isKeyPressed(GLFW_KEY_D))
        controller->ProcessCameraMovement(CameraMovement::RIGHT);
    if (isKeyPressed(GLFW_KEY_Z) && isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        controller->Undo();
}