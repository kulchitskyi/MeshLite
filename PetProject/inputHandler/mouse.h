#pragma once

#include <memory>

struct GLFWwindow;
class InputController;

class Mouse
{
private:
    std::shared_ptr <InputController> controller;
    int lastCursorXpos = 20;
    int lastCursorYpos = 20;

    static void mouse_callback_static(GLFWwindow* window, double xposIn, double yposIn);

    static void scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset);

    static void mouse_button_callback_static(GLFWwindow* window, int button, int action, int mods);
public:
    Mouse(std::shared_ptr <InputController> _controller);
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void setCallbacks();
};