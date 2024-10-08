#include "Game.h"
#include "Utils.h"
#include "Upgrade.h"
#include <cstdlib>
#include <ctime>
#include <emscripten/html5.h>

Game::Game() : player(0.0f, 0.0f, 0.05f, 0.01f), gameOver(false), ui() {}

void Game::init() {
    std::srand(std::time(0));
    renderer.init();
    input.init();
    ui.init();
    renderer.setWorldSize(worldWidth, worldHeight);

    // Initialize game objects
    player = Cell(0.0f, 0.0f, 0.05f, 0.01f);
    for (int i = 0; i < 20; i++) {
        spawnFood();
        printf("Food spawned at (%f, %f)\n", foods.back().x, foods.back().y);
    }

    // Set up keyboard input callbacks
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, Input::keyCallback);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, EM_TRUE, Input::keyCallback);
}

void Game::selectUpgrade(int index) {
    if (index >= 0 && index < currentUpgradeOptions.size()) {
        UpgradeType selectedUpgrade = currentUpgradeOptions[index];
        upgradeSystem.applyUpgrade(player, selectedUpgrade);
        player.applyUpgrade(selectedUpgrade);
        showingUpgradeOptions = false;
        currentUpgradeOptions.clear();
        player.xp = 0;  // Reset XP to 0
        player.levelUp();  // Increase level and XP requirement
        printf("Upgrade applied: %s\n", upgradeTypeToString(selectedUpgrade).c_str());
    }
}

void Game::updateCamera() {
    cameraX = player.x - worldWidth / 2;
    cameraY = player.y - worldHeight / 2;
}

// Add this method to Game class
void Game::handleUpgrades() {
    if (player.xp >= player.xpToNextLevel && !showingUpgradeOptions) {
        showingUpgradeOptions = true;
        currentUpgradeOptions = {
            UpgradeType::Wings,
            UpgradeType::Spike,
            UpgradeType::HarderSkin
        };
        printf("Showing upgrade options\n");  // Debug print
    }
}

void Game::update() {
    if (gameOver) return;

    input.update();
    player.directionX = input.getHorizontalAxis();
    player.directionY = input.getVerticalAxis();

    if (!showingUpgradeOptions) {
        player.update(worldWidth, worldHeight);
        updateCamera();
        checkCollisions();
    } else {
        // Handle upgrade selection
        if (input.isMouseClicked()) {
            float mouseX = input.getMouseX();
            float mouseY = input.getMouseY();
            
            // Calculate upgrade option positions (similar to Renderer::renderUpgradeOptions)
            float centerX = worldWidth / 2;
            float centerY = worldHeight / 2;
            float optionWidth = 300.0f;
            float optionHeight = 60.0f;
            float spacing = 20.0f;

            for (size_t i = 0; i < currentUpgradeOptions.size(); ++i) {
                float y = centerY + (i - (currentUpgradeOptions.size() - 1) / 2.0f) * (optionHeight + spacing);
                float x = centerX - optionWidth / 2;

                if (mouseX >= x && mouseX <= x + optionWidth && mouseY >= y && mouseY <= y + optionHeight) {
                    selectUpgrade(i);
                    break;
                }
            }
        }
    }

    handleUpgrades();

    // HP reduction logic
    static int frameCount = 0;
    frameCount++;
    if (frameCount >= 100) {
        player.takeDamage(1);
        frameCount = 0;
    }

    if (player.hp <= 0) {
        gameOver = true;
        printf("Game Over! Player HP reached 0.\n");
    }
}

void Game::render() {
    renderer.renderGame(player, organisms, foods, gameOver, cameraX, cameraY);
    
    if (showingUpgradeOptions) {
        renderer.renderUpgradeOptions(currentUpgradeOptions, player.x, player.y);
        printf("Rendering upgrade options\n");  // Debug print
    }
    
    renderer.renderUI(player);
}

void Game::spawnFood() {
    float x = randomFloat(-worldWidth/2, worldWidth/2);
    float y = randomFloat(-worldHeight/2, worldHeight/2);
    foods.emplace_back(x, y, 0.05f);  // Increased size from 0.02f to 0.05f
}

void Game::checkCollisions() {
    for (auto it = foods.begin(); it != foods.end(); ) {
        if (distance(player.x, player.y, it->x, it->y) < player.size + it->size) {
            player.grow(0.01f);
            player.gainExperience(10);  // Add experience when eating food
            it = foods.erase(it);
            spawnFood();
        } else {
            ++it;
        }
    }
}