#pragma once
#include <string>
#include "Cell.h"   

class Cell;

enum class UpgradeType {
    Wings,
    Spike,
    HarderSkin,
    Speed,
    Size,
    Defense,
    Offense
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