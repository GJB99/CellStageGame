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

private:
    float horizontalAxis;
    float verticalAxis;
};