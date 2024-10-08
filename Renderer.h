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
    void renderGame(const Cell& player, const std::vector<Cell>& organisms, const std::vector<Food>& foods, bool gameOver, float cameraX, float cameraY);
    void renderUpgradeOptions(const std::vector<UpgradeType>& options, float playerX, float playerY);    void renderUI(const Cell& player);

    void setWorldSize(float width, float height) {
        worldWidth = width;
        worldHeight = height;
    }

private:
    GLuint gameShaderProgram;
    GLuint uiShaderProgram;
    GLuint gameVbo;
    GLuint uiVbo;
    GLint gamePositionAttrib, gameTranslationUniform, gameScaleUniform, gameColorUniform;
    GLint uiPositionAttrib, uiColorUniform;
    float cameraX, cameraY;
    float worldWidth, worldHeight;

    void initShaders();
    void initBuffers();
    void renderCell(const Cell& cell, float r, float g, float b);
    void renderFood(const Food& food);

    void initUIShaders();
    void initUIBuffers();
    void renderBar(float x, float y, float width, float height, float fillPercentage, float r, float g, float b);
    void renderText(const std::string& text, float x, float y, float scale);

    std::string upgradeTypeToString(UpgradeType type);  // Add this line if not already present
    GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
};