#pragma once
#include "Cell.h"
#include "Food.h"
#include "Renderer.h"
#include "Input.h"
#include "Upgrade.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void init();
    void update();
    void render();

    Input& getInput() { return input; }

private:
    Cell player;
    std::vector<Cell> organisms;
    std::vector<Food> foods;
    Renderer renderer;
    Input input;
    UpgradeSystem upgradeSystem;
    bool gameOver;

    void spawnFood();
    void spawnOrganism();
    void checkCollisions();
};