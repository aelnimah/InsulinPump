#include "ControlIQController.h"
#include "CGMSensorInterface.h"
#include "InsulinDeliveryManager.h"
#include "Profile.h"
#include <iostream>
#include <cmath>

ControlIQController::ControlIQController()
    : cgmSensor(nullptr), predictedBG(0.0), isActive(false),
      deliveryManager(nullptr), activeProfile(nullptr) {}

ControlIQController::~ControlIQController() {}

void ControlIQController::processSensorReading(double currentBG) {
    predictedBG = currentBG;
    std::cout << "[ControlIQController] Received sensor reading: " << currentBG << " mmol/L\n";
}

void ControlIQController::predictBGTrend() {
    if (cgmSensor) {
        double current = cgmSensor->getCurrentBG();
        double predictionOffset = 1.0;  // Simple offset for simulation.
        predictedBG = current + predictionOffset;
        std::cout << "[ControlIQController] Predicted BG in 30 minutes: " << predictedBG << " mmol/L\n";
    } else {
        std::cout << "[ControlIQController] [Error] No CGM sensor available for prediction.\n";
    }
}

void ControlIQController::applyAutomaticAdjustments() {
    if (!deliveryManager) {
        std::cout << "[ControlIQController] [Error] Insulin Delivery Manager not set.\n";
        return;
    }
    if (predictedBG < 3.9) {
        std::cout << "[ControlIQController] Predicted BG (" << predictedBG << " mmol/L) is very low. Stopping basal delivery.\n";
        deliveryManager->stopBasalDelivery();
    }
    else if (predictedBG <= 6.25) {
        std::cout << "[ControlIQController] Predicted BG (" << predictedBG << " mmol/L) is slightly low. Reducing basal delivery.\n";
        if (deliveryManager->isBasalRunning()) {
            double currentRate = deliveryManager->getCurrentBasalRate();
            double newRate = currentRate * 0.8;
            deliveryManager->startBasalDelivery(newRate);
        } else {
            std::cout << "[ControlIQController] Basal not running; cannot reduce rate.\n";
        }
    }
    else if (predictedBG <= 8.9) {
        std::cout << "[ControlIQController] Predicted BG (" << predictedBG << " mmol/L) is normal. Maintaining current basal.\n";
    }
    else if (predictedBG < 10) {
        std::cout << "[ControlIQController] Predicted BG (" << predictedBG << " mmol/L) is high. Increasing basal delivery.\n";
        if (deliveryManager->isBasalRunning()) {
            double currentRate = deliveryManager->getCurrentBasalRate();
            double newRate = currentRate * 1.2;
            deliveryManager->startBasalDelivery(newRate);
        } else {
            std::cout << "[ControlIQController] Basal not running; cannot increase rate.\n";
        }
    }
    else {
        std::cout << "[ControlIQController] Predicted BG (" << predictedBG << " mmol/L) is very high. Delivering correction bolus.\n";
        double correctionDose = predictedBG - 7.0;  // Placeholder formula.
        deliveryManager->deliverBolus(correctionDose, false, 0.0);
    }
}

double ControlIQController::getPredictedBG() const { return predictedBG; }
void ControlIQController::setPredictedBG(double bg) { predictedBG = bg; }
bool ControlIQController::getIsActive() const { return isActive; }
void ControlIQController::setIsActive(bool active) { isActive = active; }
CGMSensorInterface* ControlIQController::getCGMSensor() const { return cgmSensor; }
void ControlIQController::setCGMSensor(CGMSensorInterface* sensor) { cgmSensor = sensor; }
void ControlIQController::setDeliveryManager(InsulinDeliveryManager* manager) { deliveryManager = manager; }
