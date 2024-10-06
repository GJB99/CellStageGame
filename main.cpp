#include <emscripten.h>
#include <emscripten/html5.h>
#include "Game.h"
#include <vector>
Game* game = nullptr;

void main_loop() {
    game->update();
    game->render();
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

    game = new Game();
    game->init();

    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}