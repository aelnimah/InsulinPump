#ifndef PUMPSIMULATOR_H
#define PUMPSIMULATOR_H

class ProfileManager; // Forward declaration
class BolusCalculator;
class InsulinDeliveryManager;
class CGMSensorInterface;

// Controls simulation (starting, stopping, updating state)
class PumpSimulator {
private:
    bool isRunning;
    ProfileManager* profileManager;

    // Bolus dependencies
    BolusCalculator* bolusCalculator;
    InsulinDeliveryManager* deliveryManager;
    CGMSensorInterface* cgmSensor;

public:
    PumpSimulator();
    ~PumpSimulator();

    void startSimulation();
    void stopSimulation();
    void updateSimulationState();
    void shutdown();

    bool getIsRunning() const;
    void setIsRunning(bool running);

    void setProfileManager(ProfileManager* mgr);
    ProfileManager* getProfileManager();

    // New setter methods for bolus dependencies
    void setBolusCalculator(BolusCalculator* bc);
    void setInsulinDeliveryManager(InsulinDeliveryManager* idm);
    void setCGMSensorInterface(CGMSensorInterface* cgm);

    // New getters (if needed)
    BolusCalculator* getBolusCalculator() const;
    InsulinDeliveryManager* getInsulinDeliveryManager() const;
    CGMSensorInterface* getCGMSensorInterface() const;
};

#endif // PUMPSIMULATOR_H
