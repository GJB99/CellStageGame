#pragma once
#include "Cell.h"

enum class UpgradeType {
    Speed,
    Size,
    Defense,
    Offense
};

class UpgradeSystem {
public:
    UpgradeSystem();

    void applyUpgrade(Cell& cell, UpgradeType type);
    bool canUpgrade(const Cell& cell, UpgradeType type);

private:
    int upgradeCost(UpgradeType type);
};