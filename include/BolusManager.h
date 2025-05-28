/*
BolusManager
    - Purpose: Coordinates insulin bolus calculations and delivery using profile data, CGM input, and delivery logic.
    - Spec Refs:
        + Deliver Manual Bolus – Computes and delivers insulin based on BG, carbs, and IOB.
        + Control IQ Auto Adjustments – Can pull BG from CGM for predictive correction boluses.
        + View Pump Info & History – Integrates with delivery system for logging purposes.
    - Design Notes:
        + Uses ProfileManager to fetch the active user profile.
        + Uses BolusCalculator for dose computation and delegates to InsulinDeliveryManager for delivery.
        + Optionally uses CGMSensorInterface to obtain real-time glucose values.
    - Class Overview:
        + computeRecommendedDose() – Calculates dose based on inputs and profile data.
        + deliverBolus(...) – Sends insulin via delivery manager (immediate or extended).
        + getBGFromCGM() – Returns CGM blood glucose (if sensor available).
*/

#ifndef BOLUSMANAGER_H
#define BOLUSMANAGER_H

class ProfileManager;
class BolusCalculator;
class InsulinDeliveryManager;
class CGMSensorInterface;

class BolusManager {
public:
    BolusManager(ProfileManager* pm, BolusCalculator* bc, 
                 InsulinDeliveryManager* idm, CGMSensorInterface* cgm = nullptr);
    ~BolusManager();

    double computeRecommendedDose(double currentBG, double carbIntake);
    void deliverBolus(double dose, bool extended = false, double duration = 0.0);
    void deliverBolus(double dose, bool extended, double immediateAmount, double duration, int splits);
    void deliverBolus(double dose, bool extended, double immediateAmount, double duration, int splits, int startTime);

    double getBGFromCGM() const;

private:
    ProfileManager* profileManager;
    BolusCalculator* bolusCalculator;
    InsulinDeliveryManager* deliveryManager;
    CGMSensorInterface* cgmSensor;
};

#endif // BOLUSMANAGER_H
