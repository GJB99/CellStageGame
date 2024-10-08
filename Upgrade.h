#pragma once
#include <string>
#include "Cell.h"   

class Cell;

enum class UpgradeType {
    UtilitySpeed,
    DefenseHP,
    OffensiveDamage
};

std::string upgradeTypeToString(UpgradeType type); 

class UpgradeSystem {
public:
    UpgradeSystem();

    void applyUpgrade(Cell& cell, UpgradeType type);
    bool canUpgrade(const Cell& cell, UpgradeType type);

private:
    int upgradeCost(UpgradeType type);
};