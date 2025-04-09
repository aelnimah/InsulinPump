#ifndef CONTROLIQCONTROLLER_H
#define CONTROLIQCONTROLLER_H


class CGMSensorInterface;
class InsulinDeliveryManager;
class Profile;
/*
 * ControlIQController
 * -------------------
 * This class automatically adjusts insulin delivery based on CGM readings.
 * It supports the "Control IQ Automatic Adjustments" use case, including predicting
 * blood glucose trends and applying automatic corrections.
 */
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

    // Process a new sensor reading.
    void processSensorReading(double currentBG);
    // Predict future blood glucose trends.
    void predictBGTrend();
    // Apply automatic adjustments to insulin delivery.
    void applyAutomaticAdjustments();

    // Getters and setters.
    double getPredictedBG() const;
    void setPredictedBG(double bg);
    bool getIsActive() const;
    void setIsActive(bool active);
    void setInsulinDeliveryManager(InsulinDeliveryManager* mgr) { deliveryManager = mgr; }
    InsulinDeliveryManager* getInsulinDeliveryManager() const { return deliveryManager; }
    CGMSensorInterface* getCGMSensor() const;
    void setCGMSensor(CGMSensorInterface* sensor);
    void setDeliveryManager(InsulinDeliveryManager* manager);
};

#endif // CONTROLIQCONTROLLER_H
