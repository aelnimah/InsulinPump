/*
InsulinDeliveryManager
    - Purpose: Manages delivery of insulin via bolus (immediate and extended) and basal dosing, while tracking IOB.
    - Spec Refs:
        + Deliver Manual Bolus
        + Start/Stop/Resume Basal
        + Control IQ Auto Adjustments
        + Handle Pump Malfunction
        + View Pump Info & History
    - Design Notes:
        + Interfaces with Cartridge and Battery to simulate hardware.
        + Supports future Control IQ logic for predictive delivery.
        + Tracks and executes extended bolus events based on simulation time.
*/

#ifndef INSULINDELIVERYMANAGER_H
#define INSULINDELIVERYMANAGER_H

#include <vector>

class BolusCalculator;
class Battery;
class Cartridge;

struct ExtendedDoseEvent {
    double dose;
    double scheduledTime; // In simulated minutes
};

class InsulinDeliveryManager {
private:
    double currentBasalRate;
    double insulinOnBoard;
    bool basalRunning;
    double previousBasalRate;

    BolusCalculator* bolusCalculator;
    Battery* battery;
    Cartridge* cartridge;

    std::vector<ExtendedDoseEvent> extendedSchedule;

public:
    InsulinDeliveryManager();
    ~InsulinDeliveryManager();

    void deliverBolus(double amount, bool extended, double duration = 0.0);
    void deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits);
    
    // For CLI testing: supports simulation-time aware scheduling
    void deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits, double currentSimTime);


    void processScheduledExtendedDoses(double currentSimTime);

    void startBasalDelivery(double rate);
    void stopBasalDelivery();
    void resumeBasalDelivery();

    void updateIOB(double elapsedTime);
    bool hasSufficientInsulin(double requiredUnits);
    void onTick(double elapsedTime); // Simulates real-time updates

    double getCurrentBasalRate() const;
    void setCurrentBasalRate(double rate);
    double getInsulinOnBoard() const;
    void setInsulinOnBoard(double iob);
    bool isBasalRunning() const;
    void setBasalRunning(bool running);

    void setCartridge(Cartridge* cart);
    void setBolusCalculator(BolusCalculator* bc);
    void setBattery(Battery* bat);
};

#endif // INSULINDELIVERYMANAGER_H
