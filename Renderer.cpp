#include "Renderer.h"
#include <vector>
#include <set>
#include <string>
#include <set>

Renderer::Renderer() : gameShaderProgram(0), uiShaderProgram(0), gameVbo(0), uiVbo(0), cameraX(0.0f), cameraY(0.0f) {}
Renderer::~Renderer() {
    if (gameShaderProgram) {
        glDeleteProgram(gameShaderProgram);
    }
    if (uiShaderProgram) {
        glDeleteProgram(uiShaderProgram);
    }
    if (gameVbo) {
        glDeleteBuffers(1, &gameVbo);
    }
    if (uiVbo) {
        glDeleteBuffers(1, &uiVbo);
    }
}

void Renderer::init() {
    initShaders();
    initBuffers();
    initUIShaders();
    initUIBuffers();

    // Set the initial screen size
    glUseProgram(gameShaderProgram);
    glUniform2f(glGetUniformLocation(gameShaderProgram, "uScreenSize"), worldWidth, worldHeight);
}

void Renderer::initUIShaders() {
    const char* vertexShaderSource = R"(
        attribute vec2 position;
        uniform vec2 uScreenSize;
        void main() {
            vec2 clipSpace = (position / uScreenSize) * 2.0 - 1.0;
            gl_Position = vec4(clipSpace, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        precision mediump float;
        uniform vec4 uColor;
        void main() {
            gl_FragColor = uColor;
        }
    )";

    uiShaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (uiShaderProgram == 0) {
        printf("Failed to create UI shader program\n");
        return;
    }

    uiPositionAttrib = glGetAttribLocation(uiShaderProgram, "position");
    uiColorUniform = glGetUniformLocation(uiShaderProgram, "uColor");
    GLint uScreenSizeUniform = glGetUniformLocation(uiShaderProgram, "uScreenSize");

    glUseProgram(uiShaderProgram);
    glUniform2f(uScreenSizeUniform, 800.0f, 600.0f);  // Set your actual screen resolution
}

void Renderer::initShaders() {
    // Game object shader
    const char* gameVertexShaderSource = R"(
        attribute vec2 aPosition;
        uniform vec2 uTranslation;
        uniform float uScale;
        uniform vec2 uScreenSize;
        void main() {
            vec2 scaledPosition = aPosition * uScale;
            vec2 translatedPosition = scaledPosition + uTranslation;
            vec2 normalizedPosition = (translatedPosition / uScreenSize) * 2.0 - 1.0;
            gl_Position = vec4(normalizedPosition, 0.0, 1.0);
        }
    )";

    const char* gameFragmentShaderSource = R"(
        precision mediump float;
        uniform vec3 uColor;
        void main() {
            gl_FragColor = vec4(uColor, 1.0);
        }
    )";

    // UI shader
    const char* uiVertexShaderSource = R"(
        attribute vec2 aPosition;
        uniform vec2 uScreenSize;
        void main() {
            vec2 normalizedPosition = (aPosition / uScreenSize) * 2.0 - 1.0;
            gl_Position = vec4(normalizedPosition, 0.0, 1.0);
        }
    )";

    const char* uiFragmentShaderSource = R"(
        precision mediump float;
        uniform vec4 uColor;
        void main() {
            gl_FragColor = uColor;
        }
    )";

    // Compile and link game shader program
    gameShaderProgram = createShaderProgram(gameVertexShaderSource, gameFragmentShaderSource);
    gamePositionAttrib = glGetAttribLocation(gameShaderProgram, "aPosition");
    gameTranslationUniform = glGetUniformLocation(gameShaderProgram, "uTranslation");
    gameScaleUniform = glGetUniformLocation(gameShaderProgram, "uScale");
    gameColorUniform = glGetUniformLocation(gameShaderProgram, "uColor");

    // Compile and link UI shader program
    uiShaderProgram = createShaderProgram(uiVertexShaderSource, uiFragmentShaderSource);
    uiPositionAttrib = glGetAttribLocation(uiShaderProgram, "aPosition");
    uiColorUniform = glGetUniformLocation(uiShaderProgram, "uColor");
}

void Renderer::initBuffers() {
    std::vector<float> circleVertices;
    circleVertices.push_back(0.0f);
    circleVertices.push_back(0.0f);
    for (int i = 0; i <= 20; i++) {
        float angle = i * 2.0f * 3.14159f / 20;
        circleVertices.push_back(cos(angle));
        circleVertices.push_back(sin(angle));
    }

    glGenBuffers(1, &gameVbo);
    glBindBuffer(GL_ARRAY_BUFFER, gameVbo);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(gamePositionAttrib);
    glVertexAttribPointer(gamePositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Renderer::renderGame(const Cell& player, const std::vector<Cell>& organisms, const std::vector<Food>& foods, bool gameOver, float cameraX, float cameraY) {
    this->cameraX = cameraX;
    this->cameraY = cameraY;

    glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gameShaderProgram);

    // Update the screen size uniform
    glUniform2f(glGetUniformLocation(gameShaderProgram, "uScreenSize"), worldWidth, worldHeight);

    // Render the player cell
    renderCell(player, 0.0f, 1.0f, 0.0f);

    // Render other organisms
    for (const auto& organism : organisms) {
        renderCell(organism, 0.0f, 0.0f, 1.0f);
    }

    // Render the food particles
    for (const auto& food : foods) {
        renderFood(food);
    }

    if (gameOver) {
        glClearColor(0.8f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::renderCell(const Cell& cell, float r, float g, float b) {
    glUseProgram(gameShaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, gameVbo);
    glVertexAttribPointer(gamePositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(gamePositionAttrib);

    glUniform2f(gameTranslationUniform, cell.x - cameraX, cell.y - cameraY);
    glUniform1f(gameScaleUniform, cell.size);
    glUniform3f(gameColorUniform, r, g, b);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}

void Renderer::renderFood(const Food& food) {
    glUseProgram(gameShaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, gameVbo);
    glVertexAttribPointer(gamePositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(gamePositionAttrib);

    glUniform2f(gameTranslationUniform, food.x - cameraX, food.y - cameraY);
    glUniform1f(gameScaleUniform, food.size);
    glUniform3f(gameColorUniform, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}

void Renderer::initUIBuffers() {
    // Create a simple quad for UI elements
    float uiVertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenBuffers(1, &uiVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uiVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uiVertices), uiVertices, GL_STATIC_DRAW);
}

void Renderer::renderUpgradeOptions(const std::vector<UpgradeType>& options, float playerX, float playerY) {
    glUseProgram(uiShaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, uiVbo);
    glVertexAttribPointer(uiPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uiPositionAttrib);

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    float bgWidth = 320.0f;
    float bgHeight = options.size() * 70.0f + 20.0f;
    
    float bgX = screenWidth / 2 - bgWidth / 2;
    float bgY = screenHeight / 2 - bgHeight / 2;

    // Render semi-transparent background
    glUniform4f(uiColorUniform, 0.0f, 0.0f, 0.0f, 0.7f);
    float vertices[] = {bgX, bgY, bgX + bgWidth, bgY, bgX + bgWidth, bgY + bgHeight, bgX, bgY + bgHeight};
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    float optionWidth = 300.0f;
    float optionHeight = 60.0f;
    float spacing = 10.0f;

    for (size_t i = 0; i < options.size(); ++i) {
        float y = bgY + 10.0f + i * (optionHeight + spacing);
        float x = bgX + 10.0f;
        
        // Render option background
        glUniform4f(uiColorUniform, 0.3f, 0.3f, 0.3f, 1.0f);
        float optionVertices[] = {x, y, x + optionWidth, y, x + optionWidth, y + optionHeight, x, y + optionHeight};
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(optionVertices), optionVertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        std::string optionText = upgradeTypeToString(options[i]);
        renderText(optionText, x + 10, y + optionHeight / 2 - 10, 1.0f);
    }

    glDisableVertexAttribArray(uiPositionAttrib);
}

std::string Renderer::upgradeTypeToString(UpgradeType type) {
    switch (type) {
        case UpgradeType::UtilitySpeed: return "Utility: Speed";
        case UpgradeType::DefenseHP: return "Defense: HP";
        case UpgradeType::OffensiveDamage: return "Offensive: Damage";
        default: return "Unknown";
    }
}

void Renderer::renderBar(float x, float y, float width, float height, float fillPercentage, float r, float g, float b) {
    glUseProgram(uiShaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, uiVbo);
    glVertexAttribPointer(uiPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uiPositionAttrib);

    glUniform2f(glGetUniformLocation(uiShaderProgram, "uScreenSize"), 800.0f, 600.0f);
    glUniform4f(uiColorUniform, r, g, b, 1.0f);

    // Draw background
    glUniform4f(uiColorUniform, 0.2f, 0.2f, 0.2f, 1.0f);
    float vertices[] = {x, y, x + width, y, x + width, y + height, x, y + height};
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Draw fill
    glUniform4f(uiColorUniform, r, g, b, 1.0f);
    vertices[2] = vertices[4] = x + width * fillPercentage;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Renderer::renderText(const std::string& text, float x, float y, float scale) {
    glUseProgram(uiShaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, uiVbo);
    glVertexAttribPointer(uiPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uiPositionAttrib);

    float charWidth = 8.0f * scale;
    float charHeight = 16.0f * scale;

    for (size_t i = 0; i < text.length(); ++i) {
        float charX = x + i * charWidth;
        
        // Render character background
        glUniform4f(uiColorUniform, 1.0f, 1.0f, 1.0f, 1.0f);
        float charVertices[] = {charX, y, charX + charWidth, y, charX + charWidth, y + charHeight, charX, y + charHeight};
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(charVertices), charVertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glDisableVertexAttribArray(uiPositionAttrib);
}

GLuint Renderer::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}