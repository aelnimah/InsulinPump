#ifndef PUMPTESTER_H
#define PUMPTESTER_H

#include <string>

class Profile;
class ProfileManager;
class BolusCalculator;
class InsulinDeliveryManager;
class PumpSimulator;
class Cartridge;
class Battery;
class CGMSensorInterface;
class ControlIQController;
class AlertManager;

class PumpTester {
public:
    PumpTester();
    ~PumpTester();

    void runAllTests();
    void testManualBolus();
    void testExtendedBolus();
    void testBasalControl();

    void testIOBDecayWithExtendedBolus();

private:
    void simulateTime(double minutes);
    void printHeader(const std::string& title);

    // Core components
    PumpSimulator* simulator;
    ProfileManager* profileManager;
    BolusCalculator* bolusCalculator;
    InsulinDeliveryManager* deliveryManager;
    Cartridge* cartridge;
    Battery* battery;
    CGMSensorInterface* cgmSensor;
    ControlIQController* controlIQ;
    AlertManager* alertManager;

    Profile* activeProfile;
};

#endif // PUMPTESTER_H
