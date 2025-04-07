#ifndef INSULINDELIVERYMANAGER_H
#define INSULINDELIVERYMANAGER_H

#include <string>
#include <vector>

class BolusCalculator;
class Battery;
class Cartridge;

struct ExtendedDoseEvent {
    double dose;          // dose to deliver in this event
    double scheduledTime; // simulated time (in minutes) when this event should fire
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

    // Immediate bolus delivery.
    // (If extended==false, duration is ignored.)
    void deliverBolus(double amount, bool extended, double duration);

    // Extended bolus delivery with user-specified immediate portion, duration, and splits.
    void deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits);
    

    // Process scheduled extended doses given the current simulated time (in minutes)
    void processScheduledExtendedDoses(double currentSimTime);

    void startBasalDelivery(double rate);
    void stopBasalDelivery();
    void resumeBasalDelivery();
    
    void updateIOB(double elapsedTime);
    bool hasSufficientInsulin(double requiredUnits);
    void onTick(double elapsedTime);

    double getCurrentBasalRate() const;
    void setCurrentBasalRate(double rate);
    double getInsulinOnBoard() const;
    void setInsulinOnBoard(double iob);
    bool isBasalRunning() const;
    void setBasalRunning(bool running);

    void setCartridge(Cartridge* cart) { cartridge = cart; }
    void setBolusCalculator(BolusCalculator* bc) { bolusCalculator = bc; }
    void setBattery(Battery* bat);
};

#endif // INSULINDELIVERYMANAGER_H