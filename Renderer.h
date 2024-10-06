#pragma once
#include <GLES2/gl2.h>
#include "Cell.h"
#include "Food.h"
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void renderGame(const Cell& player, const std::vector<Cell>& organisms, const std::vector<Food>& foods, bool gameOver);

private:
    GLuint shaderProgram;
    GLuint vbo;
    GLint positionAttrib, translationUniform, scaleUniform, colorUniform;

    void initShaders();
    void initBuffers();
    void renderCell(const Cell& cell, float r, float g, float b);
    void renderFood(const Food& food);
};