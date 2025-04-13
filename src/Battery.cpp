#include "Battery.h"

#include <algorithm>

// Battery starts fully charged
Battery::Battery() : level(100) {}

Battery::~Battery() {}

// Returns current battery level
int Battery::getLevel() const {
    return level;
}

// Sets battery level (e.g., via simulation or user action)
void Battery::setLevel(int lvl) {
    level = std::max(0, std::min(100, lvl));
}
