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

    bool isMouseClicked() const { return mouseClicked; }
    float getMouseX() const { return mouseX; }
    float getMouseY() const { return mouseY; }

    void setMouseClicked(bool clicked);
    void setMousePosition(float x, float y);

    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);
    static EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData);

    bool isKeyJustPressed(int keyCode) {
        return keys[keyCode] && !prevKeys[keyCode];
    }

private:
    bool keys[256] = {false};
    bool prevKeys[256] = {false};
    bool mouseClicked = false;
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float horizontalAxis, verticalAxis;
};