#include "Input.h"
#include "Game.h"

Input::Input() : mouseClicked(false), mouseX(0), mouseY(0) {}

Input::~Input() {}

void Input::init() {
    mouseClicked = false;
    mouseX = mouseY = 0;
    memset(keys, 0, sizeof(keys));
    memset(prevKeys, 0, sizeof(prevKeys));
}

void Input::update() {
    memcpy(prevKeys, keys, sizeof(keys));
    mouseClicked = false;  // Reset mouse click each frame
}

EM_BOOL Input::mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
        input->mouseClicked = true;
        input->mouseX = e->targetX;
        input->mouseY = e->targetY;
    }
    return true;
}

EM_BOOL Input::mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    input->mouseX = e->targetX;
    input->mouseY = e->targetY;
    return true;
}

EM_BOOL Input::keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        input->keys[e->keyCode] = true;
    } else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
        input->keys[e->keyCode] = false;
    }
    return true;
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