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
            speed *= 1.2f;  // Increase speed by 20%
            break;
        case UpgradeType::DefenseHP:
            maxHp += 20;  // Increase max HP by 20
            hp = maxHp;  // Heal to full
            break;
        case UpgradeType::OffensiveDamage:
            damage *= 1.2f;  // Increase damage by 20%
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