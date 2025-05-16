#include "mouse.h"
#include "inputController.h"

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

void Mouse::mouse_button_callback_static(GLFWwindow* window, int button, int action, int mods) {
    Mouse* instance = static_cast<Mouse*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->mouse_button_callback(window, button, action, mods);
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
        controller->processCameraDirection(xoffset, yoffset);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        controller->processModelMovement(xoffset, yoffset);
    }
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS))
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        controller->processPicking(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    controller->processCameraZoom(static_cast<float>(yoffset));
}

void Mouse::setCallbacks() {
    glfwSetWindowUserPointer(controller->getGLFWwindow(), this);
    glfwSetMouseButtonCallback(controller->getGLFWwindow(), mouse_button_callback_static);
    glfwSetCursorPosCallback(controller->getGLFWwindow(), mouse_callback_static);
    glfwSetScrollCallback(controller->getGLFWwindow(), scroll_callback_static);
}