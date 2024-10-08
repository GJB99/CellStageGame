#include "Upgrade.h"

std::string upgradeTypeToString(UpgradeType type) {
    switch (type) {
        case UpgradeType::UtilitySpeed: return "Utility: Speed";
        case UpgradeType::DefenseHP: return "Defense: HP";
        case UpgradeType::OffensiveDamage: return "Offensive: Damage";
        default: return "Unknown";
    }
}

UpgradeSystem::UpgradeSystem() {
    // Initialize upgrade system if needed
}

void UpgradeSystem::applyUpgrade(Cell& cell, UpgradeType type) {
    switch (type) {
        case UpgradeType::UtilitySpeed:
            cell.speed *= 1.2f;
            break;
        case UpgradeType::DefenseHP:
            cell.maxHp += 20;
            cell.hp += 20;
            break;
        case UpgradeType::OffensiveDamage:
            cell.damage *= 1.2f;
            break;
    }
}

bool UpgradeSystem::canUpgrade(const Cell& cell, UpgradeType type) {
    return cell.xp >= upgradeCost(type);
}

int UpgradeSystem::upgradeCost(UpgradeType type) {
    switch (type) {
        case UpgradeType::UtilitySpeed:
            return 10;
        case UpgradeType::DefenseHP:
            return 20;
        case UpgradeType::OffensiveDamage:
            return 25;
    }
    return 0;
}