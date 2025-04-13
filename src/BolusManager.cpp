#include "BolusManager.h"
#include "ProfileManager.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "Profile.h"

BolusManager::BolusManager(ProfileManager* pm, BolusCalculator* bc,
                           InsulinDeliveryManager* idm, CGMSensorInterface* cgm)
    : profileManager(pm), bolusCalculator(bc), deliveryManager(idm), cgmSensor(cgm) {}

BolusManager::~BolusManager() {
    // Does not delete pointers – assumes shared ownership elsewhere.
}

// Computes the recommended bolus dose based on profile, BG, carbs, and IOB
double BolusManager::computeRecommendedDose(double currentBG, double carbIntake) {
    Profile* activeProfile = profileManager->getActiveProfile();
    if (!activeProfile)
        return 0.0; // No active profile means we cannot calculate a dose

    double iob = deliveryManager->getInsulinOnBoard();
    return bolusCalculator->calculateBolus(currentBG, carbIntake, iob, activeProfile);
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

// Returns CGM BG reading, or 0.0 if CGM is not connected
double BolusManager::getBGFromCGM() const {
    return cgmSensor ? cgmSensor->getCurrentBG() : 0.0;
}
