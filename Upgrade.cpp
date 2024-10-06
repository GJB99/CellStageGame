#include "Upgrade.h"

UpgradeSystem::UpgradeSystem() {
    // Initialize upgrade system if needed
}

void UpgradeSystem::applyUpgrade(Cell& cell, UpgradeType type) {
    // Implement upgrade logic here
    switch (type) {
        case UpgradeType::Speed:
            cell.speed *= 1.1f;
            break;
        case UpgradeType::Size:
            cell.grow(0.01f);
            break;
        case UpgradeType::Defense:
            // Implement defense upgrade
            break;
        case UpgradeType::Offense:
            // Implement offense upgrade
            break;
    }
}

bool UpgradeSystem::canUpgrade(const Cell& cell, UpgradeType type) {
    // Implement logic to check if upgrade is possible
    return cell.xp >= upgradeCost(type);
}

int UpgradeSystem::upgradeCost(UpgradeType type) {
    // Implement cost calculation for each upgrade type
    switch (type) {
        case UpgradeType::Speed:
            return 10;
        case UpgradeType::Size:
            return 15;
        case UpgradeType::Defense:
            return 20;
        case UpgradeType::Offense:
            return 25;
    }
    return 0;
}