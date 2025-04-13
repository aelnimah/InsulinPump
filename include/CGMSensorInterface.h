#ifndef CGMSENSORINTERFACE_H
#define CGMSENSORINTERFACE_H

class Profile;

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

public:
    CGMSensorInterface();
    ~CGMSensorInterface();

    // Get the current blood glucose reading.
    double getCurrentBG() const;
    // Simulate the next sensor reading.
    void simulateNextReading();
    // Set a new blood glucose value.
    void setBG(double newValue);
};

#endif // CGMSENSORINTERFACE_H