#include "UI.h"
#include <string>
#include <sstream>
#include <GLES2/gl2.h>

UI::UI() : shaderProgram(0), vbo(0) {}

void UI::init() {
    initShaders();
    initBuffers();
}

void UI::renderText(const std::string& text, float x, float y, float scale) {
    // Draw black outline
    for(int offsetX = -2; offsetX <= 2; offsetX++) {
        for(int offsetY = -2; offsetY <= 2; offsetY++) {
            if(offsetX == 0 && offsetY == 0) continue;
            renderBar(x + offsetX, y + offsetY, text.length() * 12.0f * scale, 20.0f * scale, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    // Draw white text
    renderBar(x, y, text.length() * 12.0f * scale, 20.0f * scale, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void UI::render(const Cell& player, float cameraX, float cameraY, float worldWidth, float worldHeight) {
    glUseProgram(shaderProgram);
    glUniform2f(resolutionUniform, 800.0f, 600.0f);  // Set your actual screen resolution

    // Health bar
    renderBar(10, 10, 200, 20, player.hp / (float)player.maxHp, 1.0f, 0.0f, 0.0f, 1.0f);
    
    // XP bar
    renderBar(10, 40, 200, 20, player.xp / (float)player.xpToNextLevel, 0.0f, 1.0f, 0.0f, 1.0f);
    
    // Level
    std::string levelText = "Level: " + std::to_string(player.level);
    renderText(levelText, 10, 70, 1.0f);
    
    // Score
    std::string scoreText = "Score: " + std::to_string(score);
    renderText(scoreText, 10, 100, 1.0f);
    
    // Active power-ups
    if (player.powerUpTimeLeft > 0) {
        std::string powerUpText = "Power-up: " + std::to_string((int)player.powerUpTimeLeft) + "s";
        renderText(powerUpText, 10, 130, 1.0f);
    }
}

void UI::renderBar(float x, float y, float width, float height, float fillPercentage, float r, float g, float b, float a) {
    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttrib);

    // Render bar background
    glUniform4f(colorUniform, 0.2f, 0.2f, 0.2f, a);
    float bgVertices[] = {x, y, x + width, y, x + width, y + height, x, y + height};
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bgVertices), bgVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Render bar fill
    glUniform4f(colorUniform, r, g, b, a);
    float fillWidth = width * fillPercentage;
    float fillVertices[] = {x, y, x + fillWidth, y, x + fillWidth, y + height, x, y + height};
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(fillVertices), fillVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void UI::initShaders() {
    const char* vertexShaderSource = R"(
        attribute vec2 aPosition;
        uniform vec2 uResolution;
        void main() {
            vec2 zeroToOne = aPosition / uResolution;
            vec2 zeroToTwo = zeroToOne * 2.0;
            vec2 clipSpace = zeroToTwo - 1.0;
            gl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);
        }
    )";

    const char* fragmentShaderSource = R"(
        precision mediump float;
        uniform vec4 uColor;
        void main() {
            gl_FragColor = uColor;
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    positionAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    colorUniform = glGetUniformLocation(shaderProgram, "uColor");
    resolutionUniform = glGetUniformLocation(shaderProgram, "uResolution");
}

void UI::initBuffers() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float vertices[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void UI::renderXPBar(float xpPercentage) {
    // Implement XP bar rendering using OpenGL
}

void UI::renderHPBar(float hpPercentage) {
    // Implement HP bar rendering using OpenGL
}

void UI::renderLevelText(int level) {
    // Implement level text rendering using OpenGL
}

std::string UI::upgradeTypeToString(UpgradeType type) {
    switch (type) {
        case UpgradeType::UtilitySpeed: return "Utility: Speed";
        case UpgradeType::DefenseHP: return "Defense: HP";
        case UpgradeType::OffensiveDamage: return "Offensive: Damage";
        default: return "Unknown";
    }
}

void UI::renderUpgradeOptions(const std::vector<UpgradeType>& options, float playerX, float playerY, float cameraX, float cameraY, float worldWidth, float worldHeight) {
    glUseProgram(shaderProgram);
    glUniform2f(resolutionUniform, 800.0f, 600.0f);

    // Draw semi-transparent black background for entire screen
    renderBar(0, 0, 800, 600, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f);

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    float bgWidth = 320.0f;
    float bgHeight = options.size() * 70.0f + 20.0f;
    float bgX = screenWidth / 2 - bgWidth / 2;
    float bgY = screenHeight / 2 - bgHeight / 2;

    // Draw menu background
    renderBar(bgX, bgY, bgWidth, bgHeight, 1.0f, 0.2f, 0.2f, 0.2f, 0.9f);

    // Draw title with outline
    std::string title = "Choose Upgrade (Press 1, 2, or 3)";
    // Black outline
    for(int offsetX = -1; offsetX <= 1; offsetX++) {
        for(int offsetY = -1; offsetY <= 1; offsetY++) {
            if(offsetX == 0 && offsetY == 0) continue;
            renderText(title, bgX + 10 + offsetX, bgY - 30 + offsetY, 1.0f);
        }
    }
    // White text
    renderText(title, bgX + 10, bgY - 30, 1.0f);

    // Draw each option with better visibility
    float optionWidth = 300.0f;
    float optionHeight = 60.0f;
    float spacing = 10.0f;

    for (size_t i = 0; i < options.size(); ++i) {
        float y = bgY + 10.0f + i * (optionHeight + spacing);
        float x = bgX + 10.0f;
        
        // Check if mouse is over this option
        bool isHovered = (mouseX >= x && mouseX <= x + optionWidth && 
                         mouseY >= y && mouseY <= y + optionHeight);
        
        // Draw option background with highlight if hovered
        float r = isHovered ? 0.4f : 0.2f;
        float g = isHovered ? 0.4f : 0.2f;
        float b = isHovered ? 0.4f : 0.2f;
        renderBar(x, y, optionWidth, optionHeight, 1.0f, r, g, b, 1.0f);
        
        // Draw option text
        std::string text = std::to_string(i + 1) + ". " + upgradeTypeToString(options[i]);
        renderText(text, x + 10, y + optionHeight/2 - 10, 1.2f);  // Increased scale to 1.2
    }
}