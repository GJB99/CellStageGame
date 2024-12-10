#pragma once
#include <GLES2/gl2.h>
#include <vector>

// Forward declaration
enum class UpgradeType;

class Cell {
public:
    bool readyForUpgrade;

    Cell(float startX, float startY, float startSize, float startSpeed);

    void move(float dx, float dy) {
        x += dx;
        y += dy;
    }

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

    float baseSpeed;
    float baseDamage;
    bool isInvincible;
    float powerUpTimeLeft;
    PowerUpType activePowerUp;

    void applyPowerUp(PowerUpType type, float duration);
    void updatePowerUps(float dt);
    void resetStats();

    // Add new combat stats
    float criticalChance = 0.05f;
    float criticalMultiplier = 2.0f;
    float armor = 0.0f;
    float regeneration = 0.0f;
    bool isStunned = false;
    float stunDuration = 0.0f;
    
    // New combat methods
    float calculateDamage(float baseDamage) {
        if (rand() / (float)RAND_MAX < criticalChance) {
            return baseDamage * criticalMultiplier;
        }
        return baseDamage;
    }
    
    void updateStats(float deltaTime) {
        if (stunDuration > 0) {
            stunDuration -= deltaTime;
            isStunned = stunDuration > 0;
        }
        if (regeneration > 0 && hp < maxHp) {
            hp = std::min(maxHp, hp + regeneration * deltaTime);
        }
    }

    // Add these properties
    bool hasPowerUp = false;
    float powerUpTimeLeft = 0.0f;
    bool isInvincible = false;
    
private:
    
};

// Include this at the end of the file to avoid circular dependencies
#include "Upgrade.h"