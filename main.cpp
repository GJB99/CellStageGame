#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

// Game initialization
void init() {
    // Set up your game state, load assets, etc.
}

// Game update function
void update() {
    // Update game logic: movement, collision, evolution
}

// Game render function
void render() {
    // Clear the screen
    glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render your game objects here
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
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx =
        emscripten_webgl_create_context("#canvas", &attr);
    emscripten_webgl_make_context_current(ctx);

    init();

    // Set the main loop
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}