#include "Battery.h"

Battery::Battery() : level(100) {}  // Assume full battery.

Battery::~Battery() {}

int Battery::getLevel() const {
    return level;
}

void Battery::setLevel(int lvl) {
    level = lvl;
}
