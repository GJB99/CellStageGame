#include "Cell.h"
#include <algorithm>

Cell::Cell(float startX, float startY, float startSize, float startSpeed)
    : x(startX), y(startY), size(startSize), speed(startSpeed), 
      directionX(0.0f), directionY(0.0f), xp(0), level(1), 
      hp(100), maxHp(100), xpToNextLevel(100), readyForUpgrade(false) {}

void Cell::update(float worldWidth, float worldHeight) {
    x += directionX * speed;
    y += directionY * speed;
    
    // Wrap around the world edges
    if (x < -worldWidth/2) x += worldWidth;
    if (x > worldWidth/2) x -= worldWidth;
    if (y < -worldHeight/2) y += worldHeight;
    if (y > worldHeight/2) y -= worldHeight;
}

void Cell::applyUpgrade(UpgradeType upgrade) {
    switch (upgrade) {
        case UpgradeType::UtilitySpeed:
            speed *= 1.2f;
            break;
        case UpgradeType::DefenseHP:
            maxHp += 20;
            hp = maxHp;
            break;
        case UpgradeType::OffensiveDamage:
            damage *= 1.2f;
            break;
        case UpgradeType::DefenseArmor:
            armor += 0.1f;
            break;
        case UpgradeType::UtilityRegeneration:
            regeneration += 2.0f;
            break;
        case UpgradeType::OffensiveCritical:
            criticalChance += 0.05f;
            criticalMultiplier += 0.2f;
            break;
        case UpgradeType::DefenseStunResist:
            stunResistance += 0.2f;
            break;
        case UpgradeType::UtilitySize:
            size *= 1.15f;
            break;
        case UpgradeType::OffensiveLifeSteal:
            lifeSteal += 0.1f;
            break;
    }
    upgrades.push_back(upgrade);
}

void Cell::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
    printf("Player took %d damage. Current HP: %d\n", amount, hp);
}

void Cell::gainExperience(int amount) {
    xp += amount;
    while (xp >= xpToNextLevel) {
        levelUp();
    }
}

void Cell::levelUp() {
    level++;
    xpToNextLevel = static_cast<int>(xpToNextLevel * 1.1f);  // Increase by 10%
    maxHp += 10;
    hp = maxHp;
    readyForUpgrade = true;
    printf("Leveled up! New level: %d, XP required for next level: %d\n", level, xpToNextLevel);
}

void Cell::grow(float amount) {
    size += amount;
    speed = std::max(0.005f, 0.01f - size * 0.05f);  // Slow down as we grow
}

void Cell::updateStats(float deltaTime) {
    // Update power-up duration
    if (powerUpTimeLeft > 0) {
        powerUpTimeLeft -= deltaTime;
        if (powerUpTimeLeft <= 0) {
            // Remove power-up effects
            isInvincible = false;
            speed = baseSpeed;
            damage = baseDamage;
        }
    }

    // Apply regeneration
    if (regeneration > 0 && hp < maxHp) {
        hp = std::min(maxHp, hp + regeneration * deltaTime);
    }

    // Update stun status
    if (stunDuration > 0) {
        stunDuration -= deltaTime;
        isStunned = stunDuration > 0;
    }
}

float Cell::calculateDamage(float baseDamage) {
    if (rand() / (float)RAND_MAX < criticalChance) {
        return baseDamage * criticalMultiplier;
    }
    return baseDamage;
}