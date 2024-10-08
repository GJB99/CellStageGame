#include "UI.h"
#include <string>
#include <sstream>

UI::UI() : shaderProgram(0), vbo(0) {}

void UI::init() {
    initShaders();
    initBuffers();
}

void UI::render(const Cell& player) {
    float xpPercentage = static_cast<float>(player.xp) / player.xpToNextLevel;
    float hpPercentage = static_cast<float>(player.hp) / player.maxHp;

    renderXPBar(xpPercentage);
    renderHPBar(hpPercentage);
    renderLevelText(player.level);
}

void UI::initShaders() {
    // TODO: Implement shader initialization for UI elements
}

void UI::initBuffers() {
    // TODO: Implement buffer initialization for UI elements
}

void UI::renderXPBar(float xpPercentage) {
    // TODO: Implement XP bar rendering
    // For now, just print to console
    printf("XP: %.2f%%\n", xpPercentage * 100);
}

void UI::renderHPBar(float hpPercentage) {
    // TODO: Implement HP bar rendering
    // For now, just print to console
    printf("HP: %.2f%%\n", hpPercentage * 100);
}

void UI::renderLevelText(int level) {
    // TODO: Implement level text rendering
    // For now, just print to console
    printf("Level: %d\n", level);
}