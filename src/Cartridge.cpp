#include "Cartridge.h"
#include <iostream>

Cartridge::Cartridge() : capacity(200.0), currentVolume(200.0) {}

Cartridge::~Cartridge() {}

bool Cartridge::useInsulin(double amount) {
    // For now, just deduct the amount and always succeed.
    if(currentVolume >= amount) {
        currentVolume -= amount;
        std::cout << "[Cartridge] Using " << amount << " units. Remaining: " << currentVolume << " units.\n";
        return true;
    }
    std::cout << "[Cartridge] Insufficient insulin.\n";
    return false;
}

void Cartridge::refill() {
    currentVolume = capacity;
}

bool Cartridge::isLow() const {
    return currentVolume < (capacity * 0.1);
}

double Cartridge::getCapacity() const {
    return capacity;
}

void Cartridge::setCapacity(double cap) {
    capacity = cap;
}

double Cartridge::getCurrentVolume() const {
    return currentVolume;
}

void Cartridge::setCurrentVolume(double vol) {
    currentVolume = vol;
}
