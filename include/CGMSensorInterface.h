#ifndef CGMSENSORINTERFACE_H
#define CGMSENSORINTERFACE_H

#include <vector>

class Profile;
class InsulinDeliveryManager;

/*
 * CGMSensorInterface
 * ------------------
 * This class simulates a Continuous Glucose Monitor (CGM) sensor.
 * It provides the current blood glucose (BG) value and can simulate the next reading.
 *
 * Use Cases Supported:
 * - Feeding CGM data to ControlIQController for automatic insulin adjustments.
 */
class CGMSensorInterface {
private:
    Profile* profile;
    double currentBG;
    bool isActive;
    double scheduledCarbs;

    std::vector<std::pair<int, int>> carbSchedule; // (minute, grams)
    int simulatedTime = 0;
    InsulinDeliveryManager* deliveryManager = nullptr;


public:
    CGMSensorInterface();
    ~CGMSensorInterface();

    // Get the current blood glucose reading.
    double getCurrentBG() const;
    // Simulate the next sensor reading.
    void simulateNextReading();
    // Set a new blood glucose value.
    void setBG(double newValue);

    void addCarbs(int grams);               // Call this to simulate snacking
    void setSimulatedTime(int time);        // Called by PumpSimulator each tick
    void setDeliveryManager(InsulinDeliveryManager* dm);  // Inject dependency

};

#endif // CGMSENSORINTERFACE_H