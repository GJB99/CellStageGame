#include "Renderer.h"
#include <vector>

Renderer::Renderer() : shaderProgram(0), vbo(0) {}

Renderer::~Renderer() {
    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

void Renderer::init() {
    initShaders();
    initBuffers();
}

void Renderer::initShaders() {
    const char* vertexShaderSource = R"(
        attribute vec2 aPosition;
        uniform vec2 uTranslation;
        uniform float uScale;
        void main() {
            gl_Position = vec4(aPosition * uScale + uTranslation, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        precision mediump float;
        uniform vec3 uColor;
        void main() {
            gl_FragColor = vec4(uColor, 1.0);
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
    glUseProgram(shaderProgram);

    positionAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    translationUniform = glGetUniformLocation(shaderProgram, "uTranslation");
    scaleUniform = glGetUniformLocation(shaderProgram, "uScale");
    colorUniform = glGetUniformLocation(shaderProgram, "uColor");
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

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Renderer::renderGame(const Cell& player, const std::vector<Cell>& organisms, const std::vector<Food>& foods, bool gameOver) {
    glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

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
    glUniform2f(translationUniform, cell.x, cell.y);
    glUniform1f(scaleUniform, cell.size);
    glUniform3f(colorUniform, r, g, b);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}

void Renderer::renderFood(const Food& food) {
    glUniform2f(translationUniform, food.x, food.y);
    glUniform1f(scaleUniform, food.size);
    glUniform3f(colorUniform, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
}