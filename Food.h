#pragma once
#include <GLES2/gl2.h>

enum class FoodType {
    Normal,
    Golden,  // More XP
    Poison,  // Damages but gives more XP
    Rainbow  // Random bonus effect
};

class Food {
public:
    float x, y;
    float size;
    FoodType type;
    
    Food(float x, float y, float size, FoodType type = FoodType::Normal)
        : x(x), y(y), size(size), type(type) {}
        
    int getXPValue() const {
        switch (type) {
            case FoodType::Normal: return 10;
            case FoodType::Golden: return 25;
            case FoodType::Poison: return 35;
            case FoodType::Rainbow: return 20;
            default: return 10;
        }
    }
};