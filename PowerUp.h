#pragma once
#include "Cell.h"

enum class PowerUpType {
    SpeedBoost,
    Shield,
    DoubleDamage,
    Healing
};

class PowerUp {
public:
    PowerUp(float x, float y, PowerUpType type) 
        : x(x), y(y), type(type), duration(10.0f), active(false) {}
    
    float x, y;
    PowerUpType type;
    float duration;
    bool active;
    
    void apply(Cell& cell) {
        switch(type) {
            case PowerUpType::SpeedBoost:
                cell.speed *= 1.5f;
                break;
            case PowerUpType::Shield:
                cell.isInvincible = true;
                break;
            case PowerUpType::DoubleDamage:
                cell.damage *= 2.0f;
                break;
            case PowerUpType::Healing:
                cell.hp = std::min(cell.hp + 50, cell.maxHp);
                break;
        }
    }
    
    void remove(Cell& cell) {
        switch(type) {
            case PowerUpType::SpeedBoost:
                cell.speed /= 1.5f;
                break;
            case PowerUpType::Shield:
                cell.isInvincible = false;
                break;
            case PowerUpType::DoubleDamage:
                cell.damage /= 2.0f;
                break;
        }
    }
};