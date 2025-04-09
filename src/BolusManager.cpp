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
    // Dependencies are not deleted here.
}

double BolusManager::computeRecommendedDose(double currentBG, double carbIntake) {
    Profile* activeProfile = profileManager->getActiveProfile();
    if (!activeProfile)
        return 0.0;
    double iob = deliveryManager->getInsulinOnBoard();
    return bolusCalculator->calculateBolus(currentBG, carbIntake, iob, activeProfile);
}

void BolusManager::deliverBolus(double dose, bool extended, double duration) {
    deliveryManager->deliverBolus(dose, extended, duration);
}

void BolusManager::deliverBolus(double dose, bool extended, double immediateAmount, double duration, int splits) {
    if (!extended)
        deliveryManager->deliverBolus(dose, false, 0.0);
    else
        deliveryManager->deliverBolus(dose, true, immediateAmount, duration, splits);
}

double BolusManager::getBGFromCGM() const {
    return cgmSensor ? cgmSensor->getCurrentBG() : 0.0;
}
