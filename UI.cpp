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
    // For now, we'll just print the text to the console
    printf("Render text: %s at (%f, %f)\n", text.c_str(), x, y);
    // TODO: Implement actual text rendering using WebGL
}

void UI::render(const Cell& player, float cameraX, float cameraY, float worldWidth, float worldHeight) {
    glUseProgram(shaderProgram);
    glUniform2f(resolutionUniform, 800.0f, 600.0f);  // Set your actual screen resolution

    float xpPercentage = static_cast<float>(player.xp) / player.xpToNextLevel;
    float hpPercentage = static_cast<float>(player.hp) / player.maxHp;

    renderBar(10, 10, 200, 20, xpPercentage, 0.0f, 1.0f, 0.0f, 1.0f);  // XP bar
    renderBar(10, 40, 200, 20, hpPercentage, 1.0f, 0.0f, 0.0f, 1.0f);  // HP bar
    // Implement level text rendering
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
    glUniform2f(resolutionUniform, 800.0f, 600.0f);  // Set your actual screen resolution

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    float bgWidth = 320.0f;
    float bgHeight = options.size() * 70.0f + 20.0f;
    float bgX = ((playerX - cameraX) / worldWidth) * screenWidth - bgWidth / 2;
    float bgY = ((playerY - cameraY) / worldHeight) * screenHeight - bgHeight / 2;

    // Render semi-transparent background
    renderBar(bgX, bgY, bgWidth, bgHeight, 1.0f, 0.0f, 0.0f, 0.0f, 0.7f);

    float optionWidth = 300.0f;
    float optionHeight = 60.0f;
    float spacing = 10.0f;

    for (size_t i = 0; i < options.size(); ++i) {
        float y = bgY + 10.0f + i * (optionHeight + spacing);
        float x = bgX + 10.0f;
        
        // Render option background
        renderBar(x, y, optionWidth, optionHeight, 1.0f, 0.3f, 0.3f, 0.3f, 1.0f);

        // Render option text
        std::string optionText = upgradeTypeToString(options[i]);
        renderText(optionText, x + 10, y + optionHeight / 2 - 10, 1.0f);
    }
}