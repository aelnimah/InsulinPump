#include "Cartridge.h"
#include <iostream>

// Default: 200-unit cartridge (fully filled)
Cartridge::Cartridge() : capacity(200.0), currentVolume(200.0) {}

Cartridge::~Cartridge() {}

// Attempts to use insulin from cartridge; returns false if insufficient
bool Cartridge::useInsulin(double amount) {
    if (currentVolume >= amount) {
        currentVolume -= amount;
        std::cout << "[Cartridge] Using " << amount 
                  << " units. Remaining: " << currentVolume << " units.\n";
        return true;
    }
    std::cout << "[Cartridge] Insufficient insulin.\n";
    return false;
}

// Refills cartridge to full capacity
void Cartridge::refill() {
    currentVolume = capacity;
    std::cout << "[Cartridge] Cartridge refilled to " << capacity << " units.\n";
}

// Returns true if insulin volume is less than 10% of capacity
bool Cartridge::isLow() const {
    return currentVolume < (capacity * 0.1);
}

// Getters and setters
double Cartridge::getCapacity() const { return capacity; }
void Cartridge::setCapacity(double cap) { capacity = cap; }

double Cartridge::getCurrentVolume() const { return currentVolume; }
void Cartridge::setCurrentVolume(double vol) { currentVolume = vol; }
