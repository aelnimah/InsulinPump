#include "PumpSimulator.h"
#include "ProfileManager.h"
#include <iostream>

PumpSimulator::PumpSimulator() : isRunning(false), profileManager(nullptr) {}
PumpSimulator::~PumpSimulator() {}

void PumpSimulator::startSimulation() {
    isRunning = true;
    std::cout << "[PumpSimulator] Simulation started.\n";
}

void PumpSimulator::stopSimulation() {
    isRunning = false;
    std::cout << "[PumpSimulator] Simulation stopped.\n";
}

void PumpSimulator::updateSimulationState() {
    // Placeholder for future logic.
}

void PumpSimulator::shutdown() {
    std::cout << "[PumpSimulator] Shutting down.\n";
    stopSimulation();
}

bool PumpSimulator::getIsRunning() const { return isRunning; }
void PumpSimulator::setIsRunning(bool running) { isRunning = running; }

void PumpSimulator::setProfileManager(ProfileManager* mgr) { profileManager = mgr; }
ProfileManager* PumpSimulator::getProfileManager() { return profileManager; }

BolusCalculator* PumpSimulator::getBolusCalculator() const { return bolusCalculator; }
InsulinDeliveryManager* PumpSimulator::getInsulinDeliveryManager() const { return deliveryManager; }
CGMSensorInterface* PumpSimulator::getCGMSensorInterface() const { return cgmSensor; }
