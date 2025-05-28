#include "BolusManager.h"
#include "ProfileManager.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "Profile.h"
#include <iostream>

BolusManager::BolusManager(ProfileManager* pm, BolusCalculator* bc,
                           InsulinDeliveryManager* idm, CGMSensorInterface* cgm)
    : profileManager(pm), bolusCalculator(bc), deliveryManager(idm), cgmSensor(cgm) {}

BolusManager::~BolusManager() {
    // Does not delete pointers – assumes shared ownership elsewhere.
}

double BolusManager::computeRecommendedDose(double bg, double carbs) {
    std::cout << "[BolusManager] Starting dose computation...\n";

    if (!profileManager) {
        std::cerr << "[ERROR] profileManager is null!\n";
        return 0.0;
    }
    if (!cgmSensor) {
        std::cerr << "[ERROR] CGM sensor is null!\n";
        return 0.0;
    }
    if (!deliveryManager) {
        std::cerr << "[ERROR] deliveryManager is null!\n";
        return 0.0;
    }

    Profile* active = profileManager->getActiveProfile();
    if (!active) {
        std::cerr << "[ERROR] No active profile set!\n";
        return 0.0;
    }

    double iob = deliveryManager->getInsulinOnBoard();
    std::cout << "[BolusManager] BG: " << bg << ", Carbs: " << carbs << ", IOB: " << iob << "\n";

    double dose = bolusCalculator->calculateBolus(bg, carbs, iob, active);
    std::cout << "[BolusManager] Recommended dose: " << dose << "\n";

    return dose;
}



// Delivers a quick (immediate) bolus or extended if flag set
void BolusManager::deliverBolus(double dose, bool extended, double duration) {
    deliveryManager->deliverBolus(dose, extended, duration);
}

// Delivers a complex extended bolus with immediate and scheduled components
void BolusManager::deliverBolus(double dose, bool extended, double immediateAmount, double duration, int splits) {
    if (!extended)
        deliveryManager->deliverBolus(dose, false, 0.0);
    else
        deliveryManager->deliverBolus(dose, true, immediateAmount, duration, splits);
}

void BolusManager::deliverBolus(double dose, bool extended, double immediateAmount, double duration, int splits, int startTime) {
    if (!extended)
        deliveryManager->deliverBolus(dose, false, 0.0);
    else
        deliveryManager->deliverBolus(dose, true, immediateAmount, duration, splits, startTime);
}

// Returns CGM BG reading, or 0.0 if CGM is not connected
double BolusManager::getBGFromCGM() const {
    return cgmSensor ? cgmSensor->getCurrentBG() : 0.0;
}
