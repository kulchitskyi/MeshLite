#pragma once

#include "InputController.h"

class Mouse
{
private:
    std::shared_ptr <InputController> controller;
    int lastCursorXpos = 20;
    int lastCursorYpos = 20;

    static void mouse_callback_static(GLFWwindow* window, double xposIn, double yposIn);

    static void scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset);
public:
    Mouse(std::shared_ptr <InputController> _controller);
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  
    void SetCallbacks();


};


class Keyboard
{
private:
    std::shared_ptr <InputController> controller;

public:
    Keyboard(std::shared_ptr <InputController> _controller);

    bool isKeyPressed(int16_t keyCode);

    void processInput();
};

