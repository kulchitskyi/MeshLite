#pragma once
#include "inputController.h"

#include <unordered_map>


class Keyboard
{
private:
    std::shared_ptr <InputController> controller;
    std::unordered_map<int16_t, bool> previousKeyState;
public:
    Keyboard(std::shared_ptr <InputController> _controller);

    bool isKeyPressed(int16_t keyCode);

    bool wasKeyPressed(int16_t keyCode);

    void processInput();
};

