#include "PumpSimulator.h"
#include "ProfileManager.h"
#include <iostream>

// Constructor: Simulation starts off and no profile manager is assigned
PumpSimulator::PumpSimulator()
    : isRunning(false), profileManager(nullptr) {}

// Destructor: No dynamic memory to free
PumpSimulator::~PumpSimulator() {}

// Starts simulation
void PumpSimulator::startSimulation() {
    isRunning = true;
    std::cout << "[PumpSimulator] Simulation started.\n";
}

// Stops simulations
void PumpSimulator::stopSimulation() {
    isRunning = false;
    std::cout << "[PumpSimulator] Simulation stopped.\n";
}

// ...
void PumpSimulator::updateSimulationState() {
    // Placeholder for time-based logic (e.g. IOB or basal checks).
}

// Shutdown: Stop simulation and perform final cleanup
void PumpSimulator::shutdown() {
    std::cout << "[PumpSimulator] Shutting down.\n";
    stopSimulation();
}

// True if simulation is running
bool PumpSimulator::getIsRunning() const { return isRunning; }

// Set simultion running state
void PumpSimulator::setIsRunning(bool running) { isRunning = running; }

// Assigns a ProfileManager for profile interactions
void PumpSimulator::setProfileManager(ProfileManager* mgr) {
    profileManager = mgr;
}

// Returns currently assigned ProfileManager
ProfileManager* PumpSimulator::getProfileManager() {
    return profileManager;
}

BolusCalculator* PumpSimulator::getBolusCalculator() const {
    return bolusCalculator;
}

InsulinDeliveryManager* PumpSimulator::getInsulinDeliveryManager() const {
    return deliveryManager;
}

CGMSensorInterface* PumpSimulator::getCGMSensorInterface() const {
    return cgmSensor;
}