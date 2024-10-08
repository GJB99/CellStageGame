#pragma once
#include <GLES2/gl2.h>
#include <vector>

// Forward declaration
enum class UpgradeType;

class Cell {
public:
    Cell(float startX, float startY, float startSize, float startSpeed);

    void update(float worldWidth, float worldHeight);
    void grow(float amount);
    void applyUpgrade(UpgradeType upgrade);
    void gainExperience(int amount); 
    void levelUp();
    void takeDamage(int amount);

    float x, y;
    float size;
    float speed;
    float directionX, directionY;
    float damage;
    int xp;
    int level;
    int hp;
    int maxHp;
    int xpToNextLevel;
    std::vector<UpgradeType> upgrades;

private:
    
};

// Include this at the end of the file to avoid circular dependencies
#include "Upgrade.h"