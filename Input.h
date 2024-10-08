#pragma once
#include <GLES2/gl2.h>
#include <emscripten/html5.h>
#include <unordered_map>

class Input {
public:
    Input();
    ~Input();

    void init();
    void update();
    bool isKeyPressed(int keyCode) const;
    float getHorizontalAxis() const;
    float getVerticalAxis() const;

    bool isMouseClicked() const;
    float getMouseX() const;
    float getMouseY() const;

    void setMouseClicked(bool clicked);
    void setMousePosition(float x, float y);

    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
    static EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);

private:
    std::unordered_map<int, bool> keyStates;
    bool mouseClicked;
    float mouseX, mouseY;
    float horizontalAxis, verticalAxis;
};