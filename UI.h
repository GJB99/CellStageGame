#pragma once
#include <GLES2/gl2.h>
#include "Cell.h"

class UI {
public:
    UI();
    void init();
    void render(const Cell& player);

private:
    GLuint shaderProgram;
    GLuint vbo;
    GLint positionAttrib, colorUniform;

    void initShaders();
    void initBuffers();
    void renderXPBar(float xpPercentage);
    void renderHPBar(float hpPercentage);
    void renderLevelText(int level);
};