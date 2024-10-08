#include "Game.h"
#include "Utils.h"
#include "Upgrade.h"
#include <cstdlib>
#include <ctime>
#include <emscripten/html5.h>

Game::Game() : player(0.0f, 0.0f, 0.05f, 0.01f), gameOver(false), ui(), showingUpgradeOptions(false) {}
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

void Game::handleUpgrades() {
    static bool printedHandleUpgrades = false;

    if (player.readyForUpgrade && !showingUpgradeOptions) {
        showingUpgradeOptions = true;
        currentUpgradeOptions = {
            UpgradeType::UtilitySpeed,
            UpgradeType::DefenseHP,
            UpgradeType::OffensiveDamage
        };
        if (!printedHandleUpgrades) {
            printf("Showing upgrade options\n");
            printedHandleUpgrades = true;
        }
    }
}

void Game::update() {
    if (gameOver) return;

    input.update();
    handleUpgrades();

    // Add player movement
    float dx = 0.0f, dy = 0.0f;
    if (input.isKeyPressed(87)) dy += player.speed; // W key
    if (input.isKeyPressed(83)) dy -= player.speed; // S key
    if (input.isKeyPressed(65)) dx -= player.speed; // A key
    if (input.isKeyPressed(68)) dx += player.speed; // D key
    player.x += dx;
    player.y += dy;

    updateCamera();
    checkCollisions();

    if (showingUpgradeOptions) {
        float screenWidth = 800.0f;
        float screenHeight = 600.0f;
        float bgWidth = 320.0f;
        float bgHeight = currentUpgradeOptions.size() * 70.0f + 20.0f;
        float bgX = screenWidth / 2 - bgWidth / 2;
        float bgY = screenHeight / 2 - bgHeight / 2;

        float optionWidth = 300.0f;
        float optionHeight = 60.0f;
        float spacing = 10.0f;

        if (input.isMouseClicked()) {
            float mouseX = input.getMouseX();
            float mouseY = input.getMouseY();
            
            for (size_t i = 0; i < currentUpgradeOptions.size(); ++i) {
                float y = bgY + 10.0f + i * (optionHeight + spacing);
                float x = bgX + 10.0f;

                if (mouseX >= x && mouseX <= x + optionWidth && mouseY >= y && mouseY <= y + optionHeight) {
                    selectUpgrade(i);
                    break;
                }
            }
        }
    }
}

void Game::render() {
    static bool printedRendering = false;

    renderer.renderGame(player, organisms, foods, gameOver, cameraX, cameraY);
    ui.render(player, cameraX, cameraY, worldWidth, worldHeight);

    if (showingUpgradeOptions) {
        if (!printedRendering) {
            printf("Rendering upgrade options\n");
            printedRendering = true;
        }
        renderer.setCameraPosition(cameraX, cameraY);
        renderer.renderUpgradeOptions(currentUpgradeOptions, player.x, player.y);
    }
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

std::string Game::upgradeTypeToString(UpgradeType type) {
    switch (type) {
        case UpgradeType::UtilitySpeed: return "Utility: Speed";
        case UpgradeType::DefenseHP: return "Defense: HP";
        case UpgradeType::OffensiveDamage: return "Offensive: Damage";
        default: return "Unknown";
    }
}