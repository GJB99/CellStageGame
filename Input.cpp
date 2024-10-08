#include "Input.h"
#include "Game.h"

Input::Input() : mouseClicked(false), mouseX(0.0f), mouseY(0.0f), horizontalAxis(0.0f), verticalAxis(0.0f) {}

Input::~Input() {}

void Input::init() {
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, mouseCallback);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, mouseMoveCallback);
}

void Input::update() {
    // Reset mouse click state each frame
    mouseClicked = false;
}

EM_BOOL Input::mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
        input->setMouseClicked(true);
        input->setMousePosition(e->targetX, e->targetY);
    }
    return EM_TRUE;
}

EM_BOOL Input::mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    input->setMousePosition(e->targetX, e->targetY);
    return EM_TRUE;
}

float Input::getHorizontalAxis() const {
    return horizontalAxis;
}

float Input::getVerticalAxis() const {
    return verticalAxis;
}

bool Input::isMouseClicked() const {
    return mouseClicked;
}

bool Input::isKeyPressed(int keyCode) const {
    auto it = keyStates.find(keyCode);
    return (it != keyStates.end() && it->second);
}

float Input::getMouseX() const { return mouseX; }
float Input::getMouseY() const { return mouseY; }

void Input::setMouseClicked(bool clicked) {
    mouseClicked = clicked;
}

void Input::setMousePosition(float x, float y) {
    mouseX = x;
    mouseY = y;
}

EM_BOOL Input::keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    Game* game = static_cast<Game*>(userData);
    Input* input = &game->getInput();

    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        input->keyStates[e->keyCode] = true;
    } else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
        input->keyStates[e->keyCode] = false;
    }

    // Update horizontal and vertical axes
    input->horizontalAxis = 0.0f;
    input->verticalAxis = 0.0f;
    if (input->isKeyPressed(37)) input->horizontalAxis -= 1.0f; // Left arrow
    if (input->isKeyPressed(39)) input->horizontalAxis += 1.0f; // Right arrow
    if (input->isKeyPressed(38)) input->verticalAxis += 1.0f;   // Up arrow
    if (input->isKeyPressed(40)) input->verticalAxis -= 1.0f;   // Down arrow

    return EM_TRUE;
}