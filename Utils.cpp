#include "Utils.h"
#include <cstdlib>
#include <cmath>
#include <cstdlib>
#include <cmath>

float randomFloat(float min, float max) {
    return min + (max - min) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));}

float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}