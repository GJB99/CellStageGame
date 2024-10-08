#include "Upgrade.h"

std::string upgradeTypeToString(UpgradeType type) {
    switch (type) {
        case UpgradeType::Wings: return "Wings";
        case UpgradeType::Spike: return "Spike";
        case UpgradeType::HarderSkin: return "Harder Skin";
        case UpgradeType::Speed: return "Speed";
        case UpgradeType::Size: return "Size";
        case UpgradeType::Defense: return "Defense";
        case UpgradeType::Offense: return "Offense";
        default: return "Unknown";
    }
}

UpgradeSystem::UpgradeSystem() {
    // Initialize upgrade system if needed
}

void UpgradeSystem::applyUpgrade(Cell& cell, UpgradeType type) {
    // Implement upgrade logic here
    switch (type) {
        case UpgradeType::Wings:
        case UpgradeType::Speed:
            cell.speed *= 1.1f;
            break;
        case UpgradeType::Size:
            cell.grow(0.01f);
            break;
        case UpgradeType::Spike:
        case UpgradeType::Offense:
            // Implement offense upgrade
            break;
        case UpgradeType::HarderSkin:
        case UpgradeType::Defense:
            cell.maxHp += 20;
            cell.hp += 20;
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
        case UpgradeType::Wings:
        case UpgradeType::Speed:
            return 10;
        case UpgradeType::Size:
            return 15;
        case UpgradeType::Spike:
        case UpgradeType::HarderSkin:
        case UpgradeType::Defense:
            return 20;
        case UpgradeType::Offense:
            return 25;
    }
    return 0;
}