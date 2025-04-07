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

    // Compute the recommended dose based on current BG and carbohydrate input.
    double computeRecommendedDose(double currentBG, double carbIntake);

    // Immediate bolus delivery.
    void deliverBolus(double dose, bool extended = false, double duration = 0.0);
    
    // Extended bolus delivery with user-specified parameters.
    void deliverBolus(double dose, bool extended, double immediateAmount, double duration, int splits);
    
    // Optionally, get the current BG from the CGM sensor if available.
    double getBGFromCGM() const;

private:
    ProfileManager* profileManager;
    BolusCalculator* bolusCalculator;
    InsulinDeliveryManager* deliveryManager;
    CGMSensorInterface* cgmSensor;  // Optional, can be nullptr.
};

#endif // BOLUSMANAGER_H
