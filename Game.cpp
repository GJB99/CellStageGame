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
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &input, true, Input::mouseCallback);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &input, true, Input::mouseCallback);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, &input, true, Input::mouseMoveCallback);
}


void Game::selectUpgrade(int index) {
    if (!showingUpgradeOptions || index < 0 || index >= currentUpgradeOptions.size()) {
        return;
    }
    
    UpgradeType selectedUpgrade = currentUpgradeOptions[index];
    player.applyUpgrade(selectedUpgrade);
    showingUpgradeOptions = false;
    currentUpgradeOptions.clear();
    player.readyForUpgrade = false;
    printf("Upgrade applied: %s\n", upgradeTypeToString(selectedUpgrade).c_str());
}

void Game::updateCamera() {
    cameraX = player.x - worldWidth / 2;
    cameraY = player.y - worldHeight / 2;
}

void Game::handleUpgrades() {
    if (player.readyForUpgrade && !showingUpgradeOptions) {
        showingUpgradeOptions = true;
        // Generate upgrade options
        std::vector<UpgradeType> allUpgrades = {
            UpgradeType::UtilitySpeed,
            UpgradeType::DefenseHP,
            UpgradeType::OffensiveDamage,
            UpgradeType::DefenseArmor,
            UpgradeType::UtilityRegeneration,
            UpgradeType::OffensiveCritical
        };
        
        std::random_shuffle(allUpgrades.begin(), allUpgrades.end());
        currentUpgradeOptions = {allUpgrades[0], allUpgrades[1], allUpgrades[2]};
    }

    if (showingUpgradeOptions) {
        // Pause game movement/updates while showing upgrade menu
        if (input.isKeyJustPressed(49)) { // 1 key
            selectUpgrade(0);
        } else if (input.isKeyJustPressed(50)) { // 2 key
            selectUpgrade(1);
        } else if (input.isKeyJustPressed(51)) { // 3 key
            selectUpgrade(2);
        }

        // Handle mouse input
        if (input.isMouseClicked()) {
            float mouseX = input.getMouseX();
            float mouseY = input.getMouseY();
            
            float screenWidth = 800.0f;
            float screenHeight = 600.0f;
            float bgWidth = 320.0f;
            float optionWidth = 300.0f;
            float optionHeight = 60.0f;
            float spacing = 10.0f;
            float bgHeight = currentUpgradeOptions.size() * 70.0f + 20.0f;
            float bgX = screenWidth / 2 - bgWidth / 2;
            float bgY = screenHeight / 2 - bgHeight / 2;

            for (size_t i = 0; i < currentUpgradeOptions.size(); ++i) {
                float y = bgY + 10.0f + i * (optionHeight + spacing);
                float x = bgX + 10.0f;

                if (mouseX >= x && mouseX <= x + optionWidth && 
                    mouseY >= y && mouseY <= y + optionHeight) {
                    selectUpgrade(i);
                    break;
                }
            }
        }
    }
}

void Game::update() {
    if (gameOver) return;

    input.update();
    handleUpgrades();
    handlePowerUps();
    updateDifficulty();

    // Update player movement
    float dx = 0.0f, dy = 0.0f;
    if (input.isKeyPressed(87)) dy += 1.0f; // W key
    if (input.isKeyPressed(83)) dy -= 1.0f; // S key
    if (input.isKeyPressed(65)) dx -= 1.0f; // A key
    if (input.isKeyPressed(68)) dx += 1.0f; // D key
    
    // Normalize diagonal movement
    if (dx != 0.0f && dy != 0.0f) {
        float length = sqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
    }

    // Apply speed after normalization
    player.x += dx * player.speed;
    player.y += dy * player.speed;
    player.updatePowerUps(1.0f/60.0f);

    // Update AI behavior for organisms
    for (auto& org : organisms) {
        // Move towards player if close, away if too big
        float dx = player.x - org.x;
        float dy = player.y - org.y;
        float dist = sqrt(dx*dx + dy*dy);
        
        if (org.size > player.size * 1.2f) {
            // Chase player if significantly larger
            org.directionX = dx/dist;
            org.directionY = dy/dist;
        } else if (player.size > org.size * 1.2f) {
            // Run away if player is significantly larger
            org.directionX = -dx/dist;
            org.directionY = -dy/dist;
        }
        
        org.update(worldWidth, worldHeight);
    }

    updateCamera();
    checkCollisions();
}

void Game::render() {
    renderer.renderGame(player, organisms, foods, gameOver, cameraX, cameraY);
    
    if (showingUpgradeOptions) {
        ui.setMousePosition(input.getMouseX(), input.getMouseY());
        ui.renderUpgradeOptions(currentUpgradeOptions, player.x, player.y, cameraX, cameraY, worldWidth, worldHeight);
    } else {
        ui.render(player, cameraX, cameraY, worldWidth, worldHeight);
    }
}

void Game::spawnFood() {
    float x = randomFloat(-worldWidth/2, worldWidth/2);
    float y = randomFloat(-worldHeight/2, worldHeight/2);
    
    float random = randomFloat(0, 1);
    FoodType type = FoodType::Normal;
    
    if (random < 0.05f) type = FoodType::Golden;
    else if (random < 0.08f) type = FoodType::Poison;
    else if (random < 0.10f) type = FoodType::Rainbow;
    
    foods.emplace_back(x, y, 0.05f, type);
}

void Game::checkCollisions() {
    // Check collisions with food
    for (auto it = foods.begin(); it != foods.end();) {
        float dist = distance(player.x, player.y, it->x, it->y);
        if (dist < player.size + it->size) {
            player.grow(it->size * 0.1f);
            player.gainExperience(it->getXPValue());
            score += it->getXPValue();
            it = foods.erase(it);
            spawnFood();  // Spawn new food to maintain count
        } else {
            ++it;
        }
    }

    // Check collisions with power-ups
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        float dist = distance(player.x, player.y, it->x, it->y);
        if (dist < player.size + 0.05f) {
            it->apply(player);
            it = powerUps.erase(it);
        } else {
            ++it;
        }
    }

    // Check collisions with organisms
    for (auto it = organisms.begin(); it != organisms.end();) {
        float dist = distance(player.x, player.y, it->x, it->y);
        if (dist < player.size + it->size) {
            if (player.size > it->size * 1.1f) {
                float xpGain = it->level * 15;
                player.gainExperience(xpGain);
                score += xpGain;
                if (player.lifeSteal > 0) {
                    player.hp = std::min(player.maxHp, 
                        player.hp + it->size * player.lifeSteal * 100);
                }
                it = organisms.erase(it);
                spawnOrganism();  // Keep organism count constant
            } else if (it->size > player.size * 1.1f && !player.isInvincible) {
                float damage = it->calculateDamage(it->damage);
                damage *= (1.0f - player.armor);
                player.takeDamage(damage);
                if (player.hp <= 0) {
                    gameOver = true;
                }
            }
            continue;
        }
        ++it;
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

void Game::spawnPowerUp() {
    if (powerUps.size() < 3) {  // Maximum 3 power-ups at once
        float x = randomFloat(-worldWidth/2, worldWidth/2);
        float y = randomFloat(-worldHeight/2, worldHeight/2);
        PowerUpType type = static_cast<PowerUpType>(rand() % 4);
        powerUps.emplace_back(x, y, type);
    }
}

void Game::updateDifficulty() {
    difficultyLevel = 1.0f + (player.level * 0.1f);  // Increase difficulty with player level
    
    // Spawn more organisms based on difficulty
    while (organisms.size() < 5 + (player.level * 2)) {
        spawnOrganism();
    }
    
    // Make organisms stronger
    for (auto& org : organisms) {
        org.damage = 10 * difficultyLevel;
        org.speed = 0.01f * difficultyLevel;
    }
}

void Game::handlePowerUps() {
    powerUpSpawnTimer += 1.0f/60.0f;  // Assuming 60 FPS
    if (powerUpSpawnTimer >= 10.0f) {  // Spawn power-up every 10 seconds
        spawnPowerUp();
        powerUpSpawnTimer = 0;
    }
    
    // Check collisions with power-ups
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        if (distance(player.x, player.y, it->x, it->y) < player.size + 0.05f) {
            player.applyPowerUp(it->type, 10.0f);  // 10 second duration
            it = powerUps.erase(it);
        } else {
            ++it;
        }
    }
}