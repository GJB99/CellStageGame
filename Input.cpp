#include "Input.h"
#include "Game.h"

Game* Input::game = nullptr;

Input::Input() : horizontalAxis(0.0f), verticalAxis(0.0f) {}

void Input::init() {
    // Nothing to initialize for now
}

void Input::update() {
    // Nothing to update for now, as we're using callbacks
}

float Input::getHorizontalAxis() {
    return horizontalAxis;
}

float Input::getVerticalAxis() {
    return verticalAxis;
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