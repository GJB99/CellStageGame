#pragma once
#include "Cell.h"
#include "Upgrade.h"
#include <vector>

class UI {
public:
    UI();
    void init();
    void render(const Cell& player, float cameraX, float cameraY, float worldWidth, float worldHeight);
    void renderUpgradeOptions(const std::vector<UpgradeType>& options, float playerX, float playerY, float cameraX, float cameraY, float worldWidth, float worldHeight);
    std::string upgradeTypeToString(UpgradeType type);

private:
    GLuint shaderProgram;
    GLuint vbo;
    GLint positionAttrib, colorUniform;
    GLint resolutionUniform;

    void initShaders();
    void initBuffers();
    void renderBar(float x, float y, float width, float height, float fillPercentage, float r, float g, float b, float a);    
    void renderXPBar(float xpPercentage);
    void renderHPBar(float hpPercentage);
    void renderLevelText(int level);
    void renderText(const std::string& text, float x, float y, float scale);
};