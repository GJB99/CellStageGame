#include "Input.h"
#include "Game.h"

// Forward declaration
EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);

Game* Input::game = nullptr;

Input::Input() : horizontalAxis(0.0f), verticalAxis(0.0f) {}

void Input::init() {
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, mouseCallback);
}

void Input::update() {
    // Reset mouse click state each frame
    mouseClicked = false;
}

float Input::getHorizontalAxis() {
    return horizontalAxis;
}

float Input::getVerticalAxis() {
    return verticalAxis;
}

bool Input::isMouseClicked() {
    bool clicked = mouseClicked;
    mouseClicked = false;  // Reset after checking
    return clicked;
}

float Input::getMouseX() { return mouseX; }
float Input::getMouseY() { return mouseY; }

EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
        input->mouseClicked = true;
        input->mouseX = e->targetX;
        input->mouseY = e->targetY;
    }
    return EM_TRUE;
}

EM_BOOL Input::keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    Game* game = static_cast<Game*>(userData);
    Input* input = &game->getInput();

    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        switch (e->keyCode) {
            case 37: // Left arrow
                input->horizontalAxis = -1.0f;
                break;
            case 38: // Up arrow
                input->verticalAxis = 1.0f;
                break;
            case 39: // Right arrow
                input->horizontalAxis = 1.0f;
                break;
            case 40: // Down arrow
                input->verticalAxis = -1.0f;
                break;
        }
    } else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
        switch (e->keyCode) {
            case 37: // Left arrow
            case 39: // Right arrow
                input->horizontalAxis = 0.0f;
                break;
            case 38: // Up arrow
            case 40: // Down arrow
                input->verticalAxis = 0.0f;
                break;
        }
    }
    return EM_TRUE;
}