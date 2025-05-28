#include "ControlIQController.h"
#include "CGMSensorInterface.h"
#include "InsulinDeliveryManager.h"
#include "Profile.h"
#include <iostream>
#include <cmath>

// Constructor: initialize pointers and state
ControlIQController::ControlIQController()
    : cgmSensor(nullptr),
      deliveryManager(nullptr),
      activeProfile(nullptr),
      predictedBG(0.0),
      isActive(false) {}

ControlIQController::~ControlIQController() {}

// Set predicted BG to latest CGM value (direct feed)
void ControlIQController::processSensorReading(double currentBG) {
    predictedBG = currentBG;
    std::cout << "[ControlIQController] Received sensor reading: " << currentBG << " mmol/L\n";
}

// Simulate a simple BG trend prediction based on a constant offset
void ControlIQController::predictBGTrend() {
    if (!cgmSensor || !deliveryManager) return;

    double currentBG = cgmSensor->getCurrentBG();
    double iob = deliveryManager->getInsulinOnBoard();

    // Simulate BG drop based on IOB: e.g. 1 U drops BG by 1.5 mmol/L over 30 min
    double predictedDrop = iob * 1.5;
    double predictedBG = currentBG - predictedDrop;

    std::cout << "[ControlIQController] Current BG: " << currentBG << " mmol/L\n";
    std::cout << "[ControlIQController] IOB: " << iob << " U → Predicted drop: " << predictedDrop << "\n";
    std::cout << "[ControlIQController] Predicted BG in 30 minutes: " << predictedBG << " mmol/L\n";

    this->predictedBG = predictedBG;
}

// Based on predicted BG, take automatic action
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
        double correctionDose = predictedBG - 7.0;  // Basic placeholder logic
        deliveryManager->deliverBolus(correctionDose, false, 0.0);
    }
}

// Accessors and mutators
double ControlIQController::getPredictedBG() const { return predictedBG; }
void ControlIQController::setPredictedBG(double bg) { predictedBG = bg; }

bool ControlIQController::getIsActive() const { return isActive; }
void ControlIQController::setIsActive(bool active) { isActive = active; }

CGMSensorInterface* ControlIQController::getCGMSensor() const { return cgmSensor; }
void ControlIQController::setCGMSensor(CGMSensorInterface* sensor) { cgmSensor = sensor; }

void ControlIQController::setInsulinDeliveryManager(InsulinDeliveryManager* manager) { deliveryManager = manager; }
InsulinDeliveryManager* ControlIQController::getInsulinDeliveryManager() const { return deliveryManager; }
