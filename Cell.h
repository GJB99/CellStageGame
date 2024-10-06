#pragma once
#include <GLES2/gl2.h>

class Cell {
public:
    Cell(float startX, float startY, float startSize, float startSpeed);

    void update();
    void render(GLuint shaderProgram);
    void grow(float amount);

    float x, y;
    float size;
    float speed;
    float directionX, directionY;
    int xp;

private:
    // Add any private members or methods
};