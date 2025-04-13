/*
ControlIQController
    - Purpose: Adjusts insulin delivery automatically based on CGM sensor data using a simple prediction model.
    - Spec Refs:
        + Control IQ Auto Adjustments – Delivers predictive corrections and adjusts basal.
        + Handle Pump Malfunction – Should be prepared for missing sensor or delivery manager.
    - Design Notes:
        + Designed to run periodically (e.g., each tick) to read CGM and adjust dosing.
        + Uses a basic BG offset for prediction, modifiable later with trend data.
    - Class Overview:
        + processSensorReading() – Updates predicted BG based on current CGM input.
        + predictBGTrend() – Offsets current BG to simulate a short-term prediction.
        + applyAutomaticAdjustments() – Uses thresholds to stop, reduce, increase basal or trigger bolus.
*/

#ifndef CONTROLIQCONTROLLER_H
#define CONTROLIQCONTROLLER_H

class CGMSensorInterface;
class InsulinDeliveryManager;
class Profile;

class ControlIQController {
private:
    CGMSensorInterface* cgmSensor;
    InsulinDeliveryManager* deliveryManager;
    Profile* activeProfile;
    double predictedBG;
    bool isActive;

public:
    ControlIQController();
    ~ControlIQController();

    void processSensorReading(double currentBG);
    void predictBGTrend();
    void applyAutomaticAdjustments();

    double getPredictedBG() const;
    void setPredictedBG(double bg);
    bool getIsActive() const;
    void setIsActive(bool active);

    void setInsulinDeliveryManager(InsulinDeliveryManager* mgr);
    InsulinDeliveryManager* getInsulinDeliveryManager() const;

    CGMSensorInterface* getCGMSensor() const;
    void setCGMSensor(CGMSensorInterface* sensor);
};

#endif // CONTROLIQCONTROLLER_H
