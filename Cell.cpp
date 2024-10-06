#include "Cell.h"
#include <algorithm>

Cell::Cell(float startX, float startY, float startSize, float startSpeed)
    : x(startX), y(startY), size(startSize), speed(startSpeed), directionX(0.0f), directionY(0.0f) {}

void Cell::update() {
    x += directionX * speed;
    y += directionY * speed;
    
    // Keep the cell within the canvas
    x = std::max(-1.0f + size, std::min(1.0f - size, x));
    y = std::max(-1.0f + size, std::min(1.0f - size, y));
}

void Cell::grow(float amount) {
    size += amount;
    speed = std::max(0.005f, 0.01f - size * 0.05f);  // Slow down as we grow
}