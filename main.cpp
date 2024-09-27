#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Shader sources
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

// Shader program and attribute/uniform locations
GLuint shaderProgram;
GLint positionAttrib, translationUniform, scaleUniform, colorUniform;

float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
}

// Update the Cell class
class Cell {
public:
    float x, y;
    float size;
    float speed;
    float directionX, directionY;

    Cell(float startX, float startY, float startSize, float startSpeed)
        : x(startX), y(startY), size(startSize), speed(startSpeed) {
        directionX = 0.0f;
        directionY = 0.0f;
    }

    void update() {
        x += directionX * speed;
        y += directionY * speed;
        
        // Keep the cell within the canvas
        x = std::max(-1.0f + size, std::min(1.0f - size, x));
        y = std::max(-1.0f + size, std::min(1.0f - size, y));
    }

    void grow(float amount) {
        size += amount;
        speed = std::max(0.005f, 0.01f - size * 0.05f);  // Slow down as we grow
    }

    void render() {
        glUniform2f(translationUniform, x, y);
        glUniform1f(scaleUniform, size);
        glUniform3f(colorUniform, 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
    }
};

// Food class
class Food {
public:
    float x, y;
    float size;

    Food(float startX, float startY, float startSize)
        : x(startX), y(startY), size(startSize) {}

    void render() {
        glUniform2f(translationUniform, x, y);
        glUniform1f(scaleUniform, size);
        glUniform3f(colorUniform, 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 22);
    }
};

// Global variables
Cell player(0.0f, 0.0f, 0.05f, 0.01f);
std::vector<Food> foods;
bool gameOver = false;

// Game initialization
void init() {
    printf("Initializing...\n");
    // Compile and link shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Get attribute and uniform locations
    positionAttrib = glGetAttribLocation(shaderProgram, "aPosition");
    translationUniform = glGetUniformLocation(shaderProgram, "uTranslation");
    scaleUniform = glGetUniformLocation(shaderProgram, "uScale");
    colorUniform = glGetUniformLocation(shaderProgram, "uColor");

    // Create circle vertex data
    std::vector<float> circleVertices;
    circleVertices.push_back(0.0f);
    circleVertices.push_back(0.0f);
    for (int i = 0; i <= 20; i++) {
        float angle = i * 2.0f * 3.14159f / 20;
        circleVertices.push_back(cos(angle));
        circleVertices.push_back(sin(angle));
    }

    // Create and bind vertex buffer
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(positionAttrib);
    glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Initialize game objects
    player = Cell(0.0f, 0.0f, 0.05f, 0.01f);
    for (int i = 0; i < 20; i++) {  // Increase initial food count
        float x = randomFloat(-0.9f, 0.9f);
        float y = randomFloat(-0.9f, 0.9f);
        foods.emplace_back(x, y, 0.02f);
    }

    printf("Shaders compiled and linked.\n");

    // ... (rest of the init function)
    printf("Initialization complete.\n");
}

// Keyboard input callback
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData) {
    if (gameOver) return EM_TRUE;

    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        switch (e->keyCode) {
            case 37: // Left arrow
                player.directionX = -1.0f;
                break;
            case 38: // Up arrow
                player.directionY = 1.0f;
                break;
            case 39: // Right arrow
                player.directionX = 1.0f;
                break;
            case 40: // Down arrow
                player.directionY = -1.0f;
                break;
        }
    } else if (eventType == EMSCRIPTEN_EVENT_KEYUP) {
        switch (e->keyCode) {
            case 37: // Left arrow
            case 39: // Right arrow
                player.directionX = 0.0f;
                break;
            case 38: // Up arrow
            case 40: // Down arrow
                player.directionY = 0.0f;
                break;
        }
    }
    return EM_TRUE;
}

// Game update function
void update() {
    if (gameOver) return;

    player.update();

    // Check for collisions with food
    for (auto it = foods.begin(); it != foods.end(); ) {
        if (distance(player.x, player.y, it->x, it->y) < player.size + it->size) {
            player.grow(0.01f);  // Grow the player
            it = foods.erase(it);  // Remove the food

            // Add a new food particle
            float x = randomFloat(-0.9f, 0.9f);
            float y = randomFloat(-0.9f, 0.9f);
            foods.emplace_back(x, y, 0.02f);
        } else {
            ++it;
        }
    }

    // Check for game over condition
    if (player.size > 0.5f) {
        gameOver = true;
    }
}

// Game render function
void render() {
    glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Render the player cell
    player.render();

    // Render the food particles
    for (auto& food : foods) {
        food.render();
    }

    // Render game over text if necessary
    if (gameOver) {
        // You would need to implement text rendering here
        // For now, we'll just change the background color to indicate game over
        glClearColor(0.8f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

// Main loop
void main_loop() {
    update();
    render();
}

int main() {
    // Initialize WebGL context
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.alpha = EM_TRUE;
    attr.depth = EM_TRUE;
    attr.stencil = EM_TRUE;
    attr.antialias = EM_TRUE;
    attr.premultipliedAlpha = EM_FALSE;
    attr.preserveDrawingBuffer = EM_FALSE;
    attr.powerPreference = EM_WEBGL_POWER_PREFERENCE_DEFAULT;
    attr.failIfMajorPerformanceCaveat = EM_FALSE;
    attr.majorVersion = 1;
    attr.minorVersion = 0;
    attr.enableExtensionsByDefault = EM_TRUE;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
    if (ctx == 0) {
        printf("Failed to create WebGL context!\n");
        return 1;
    }
    printf("WebGL context created successfully.\n");
    
    emscripten_webgl_make_context_current(ctx);
    
    std::srand(std::time(0));
    init();

    // Set up keyboard input callbacks
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, key_callback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, key_callback);

    // Set the main loop
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}