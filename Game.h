#pragma once
#include "Cell.h"
#include "UI.h"
#include "Food.h"
#include "Renderer.h"
#include "Input.h"
#include "Upgrade.h"
#include "PowerUp.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void init();
    void update();
    void render();
    void selectUpgrade(int index);
    Input& getInput() { return input; }
    std::string upgradeTypeToString(UpgradeType type);

    void spawnPowerUp();
    void updateDifficulty();
    void handlePowerUps();

private:
    Cell player;
    UI ui;
    std::vector<Cell> organisms;
    std::vector<Food> foods;
    Renderer renderer;
    Input input;
    UpgradeSystem upgradeSystem;
    bool gameOver;

    float worldWidth = 5.0f;
    float worldHeight = 5.0f;
    float cameraX = 0.0f;
    float cameraY = 0.0f;

    void spawnFood();
    void spawnOrganism();
    void checkCollisions();
    void updateCamera();  
    void handleUpgrades();

    bool showingUpgradeOptions;
    std::vector<UpgradeType> currentUpgradeOptions;

    std::vector<PowerUp> powerUps;
    float powerUpSpawnTimer;
    float difficultyLevel;
    int score;

    void updateOrganism(Cell& org);
};