#include "Game.h"
#include "Utils.h"
#include <cstdlib>
#include <ctime>
#include <emscripten/html5.h>

Game::Game() : player(0.0f, 0.0f, 0.05f, 0.01f), gameOver(false) {}

Game::~Game() {}

void Game::init() {
    std::srand(std::time(0));
    renderer.init();
    input.init();

    // Initialize game objects
    player = Cell(0.0f, 0.0f, 0.05f, 0.01f);
    for (int i = 0; i < 20; i++) {
        spawnFood();
    }

    // Set up keyboard input callbacks
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, Input::keyCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, Input::keyCallback);
}

void Game::update() {
    if (gameOver) return;

    input.update();
    player.directionX = input.getHorizontalAxis();
    player.directionY = input.getVerticalAxis();

    player.update();

    checkCollisions();

    // Check for game over condition
    if (player.size > 0.5f) {
        gameOver = true;
    }
}

void Game::render() {
    renderer.renderGame(player, organisms, foods, gameOver);
}

void Game::spawnFood() {
    float x = randomFloat(-0.9f, 0.9f);
    float y = randomFloat(-0.9f, 0.9f);
    foods.emplace_back(x, y, 0.02f);
}

void Game::checkCollisions() {
    for (auto it = foods.begin(); it != foods.end(); ) {
        if (distance(player.x, player.y, it->x, it->y) < player.size + it->size) {
            player.grow(0.01f);
            it = foods.erase(it);
            spawnFood();
        } else {
            ++it;
        }
    }
}