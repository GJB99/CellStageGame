#pragma once
#include <emscripten/html5.h>

class Game;

class Input {
public:
    Input();

    void init();
    void update();

    float getHorizontalAxis();
    float getVerticalAxis();

    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
    static Game* game;

    bool isMouseClicked();
    float getMouseX();
    float getMouseY();

    // Make these public
    bool mouseClicked;
    float mouseX, mouseY;

private:
    float horizontalAxis;
    float verticalAxis;
};