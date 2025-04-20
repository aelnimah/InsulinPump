#include "CGMSensorInterface.h"
#include "InsulinDeliveryManager.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

CGMSensorInterface::CGMSensorInterface() : profile(nullptr), currentBG(6.0), scheduledCarbs(0.0) {
    std::srand(std::time(nullptr));
}

CGMSensorInterface::~CGMSensorInterface() {}

double CGMSensorInterface::getCurrentBG() const {
    return currentBG;
}

void CGMSensorInterface::simulateNextReading() {
    double iobDrop = 0.0;
    if (deliveryManager) {
        double iob = deliveryManager->getInsulinOnBoard();
        iobDrop = iob * 0.15; // Drop of ~0.15 mmol/L per unit of insulin
    }

    // Apply carb effect
    for (auto it = carbSchedule.begin(); it != carbSchedule.end();) {
        if (it->first <= simulatedTime) {
            double bgRise = it->second * 0.0033; // ~0.1 mmol/L per g over 30 min
            currentBG += bgRise;
            it = carbSchedule.erase(it);
        } else {
            ++it;
        }
    }

    // Apply insulin effect
    currentBG = std::max(0.5, currentBG - iobDrop);

    // Add tiny fluctuation
    double delta = (std::rand() % 9 - 6) / 2000.0;
    currentBG += delta;
}

void CGMSensorInterface::setBG(double newValue) {
    currentBG = newValue;
}

void CGMSensorInterface::addCarbs(int grams) {
    for (int i = 1; i <= 30; ++i) {
        carbSchedule.push_back({simulatedTime + i, grams});
    }
}

void CGMSensorInterface::setSimulatedTime(int time) {
    simulatedTime = time;
}

void CGMSensorInterface::setDeliveryManager(InsulinDeliveryManager* dm) {
    deliveryManager = dm;
}