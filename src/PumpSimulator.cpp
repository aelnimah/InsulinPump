#include "PumpSimulator.h"
#include "ProfileManager.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "ControlIQController.h"
#include "AlertManager.h"
#include "Battery.h"
#include "Cartridge.h"
#include <iostream>

PumpSimulator::PumpSimulator()
    : isRunning(false),
      profileManager(nullptr),
      bolusCalculator(nullptr),
      deliveryManager(nullptr),
      cgmSensor(nullptr),
      controlIQ(nullptr),
      alertManager(nullptr),
      battery(nullptr),
      cartridge(nullptr) {}

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
    if (!isRunning) return;

    // Wait until active profile is set
    if (!profileManager || !profileManager->getActiveProfile()) {
        std::cout << "[PumpSimulator] Waiting for active profile...\n";
        return;
    }

    int currentSimTime = getCurrentSimTime();
    std::cout << "\n[Time = " << currentSimTime << " min]\n";

    if (battery) {
        battery->drain(1);
    }

    if (deliveryManager)
        deliveryManager->onTick(1.0);

    if (cgmSensor)
        cgmSensor->simulateNextReading();

    if (controlIQ) {
        controlIQ->predictBGTrend();
        controlIQ->applyAutomaticAdjustments();
    }

    if (alertManager && battery)
        alertManager->checkBattery(battery);

    if (alertManager && cartridge)
        alertManager->checkCartridge(cartridge);

    if (deliveryManager)
        deliveryManager->processScheduledExtendedDoses(currentSimTime);

    std::cout << "[PumpSimulator] Tick complete.\n";

    if (cliMode)
        simulatedMinutes += 1.0;
}

void PumpSimulator::shutdown() {
    std::cout << "[PumpSimulator] Shutting down.\n";
    stopSimulation();
}

// --- Getters and Setters ---
bool PumpSimulator::getIsRunning() const { return isRunning; }
void PumpSimulator::setIsRunning(bool running) { isRunning = running; }

void PumpSimulator::setProfileManager(ProfileManager* mgr) { profileManager = mgr; }
ProfileManager* PumpSimulator::getProfileManager() { return profileManager; }

void PumpSimulator::setBolusCalculator(BolusCalculator* bc) { bolusCalculator = bc; }
BolusCalculator* PumpSimulator::getBolusCalculator() const { return bolusCalculator; }

void PumpSimulator::setInsulinDeliveryManager(InsulinDeliveryManager* idm) { deliveryManager = idm; }
InsulinDeliveryManager* PumpSimulator::getInsulinDeliveryManager() const { return deliveryManager; }

void PumpSimulator::setCGMSensorInterface(CGMSensorInterface* cgm) { cgmSensor = cgm; }
CGMSensorInterface* PumpSimulator::getCGMSensorInterface() const { return cgmSensor; }

ControlIQController* PumpSimulator::getControlIQController() const {
    return controlIQ;
}

AlertManager* PumpSimulator::getAlertManager() const {
    return alertManager;
}

Battery* PumpSimulator::getBattery() const {
    return battery;
}

Cartridge* PumpSimulator::getCartridge() const {
    return cartridge;
}

void PumpSimulator::setControlIQController(ControlIQController* ctrl) { controlIQ = ctrl; }
void PumpSimulator::setAlertManager(AlertManager* a) { alertManager = a; }
void PumpSimulator::setBattery(Battery* b) { battery = b; }
void PumpSimulator::setCartridge(Cartridge* c) { cartridge = c; }

// --- CLI Simulation Utilities ---
void PumpSimulator::incrementSimTime(double minutes) {
    simulatedMinutes += minutes;
}

double PumpSimulator::getSimulatedMinutes() const {
    return simulatedMinutes;
}

void PumpSimulator::setGUISimTime(int minutes) {
    guiSimulatedMinutes = minutes;
}

int PumpSimulator::getCurrentSimTime() const {
    return cliMode ? static_cast<int>(simulatedMinutes) : guiSimulatedMinutes;
}

double PumpSimulator::getCurrentBG() const {
    return cgmSensor ? cgmSensor->getCurrentBG() : 0.0;
}

double PumpSimulator::getIOB() const {
    return deliveryManager ? deliveryManager->getInsulinOnBoard() : 0.0;
}
