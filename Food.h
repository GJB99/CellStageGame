#pragma once
#include <GLES2/gl2.h>

class Food {
public:
    Food(float startX, float startY, float startSize);

    void render(GLuint shaderProgram);

    float x, y;
    float size;
};