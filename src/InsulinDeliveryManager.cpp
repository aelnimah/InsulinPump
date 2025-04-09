#include "InsulinDeliveryManager.h"
#include "BolusCalculator.h"
#include "Battery.h"
#include "Cartridge.h"
#include <iostream>

InsulinDeliveryManager::InsulinDeliveryManager()
    : currentBasalRate(0.0),
      insulinOnBoard(0.0),
      basalRunning(false),
      previousBasalRate(0.0),
      bolusCalculator(nullptr),
      battery(nullptr),
      cartridge(nullptr) {}

InsulinDeliveryManager::~InsulinDeliveryManager() {
    // Additional cleanup if required.
}

void InsulinDeliveryManager::deliverBolus(double amount, bool extended, double duration) {
    if (!cartridge) {
        std::cout << "[InsulinDeliveryManager] [Error] No cartridge present.\n";
        return;
    }
    if (cartridge->getCurrentVolume() < amount) {
        std::cout << "[InsulinDeliveryManager] [Error] Insufficient insulin. Bolus canceled.\n";
        return;
    }
    if (!extended) {
        if (!cartridge->useInsulin(amount)) {
            std::cout << "[InsulinDeliveryManager] [Error] Cartridge usage failed. Bolus not delivered.\n";
            return;
        }
        insulinOnBoard -= amount;
        std::cout << "[InsulinDeliveryManager] Delivered immediate bolus of " << amount << " units.\n";
    } else {
        std::cout << "[InsulinDeliveryManager] [Error] Extended bolus parameters not provided.\n";
    }
}

void InsulinDeliveryManager::deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits) {
    if (!extended) {
        deliverBolus(totalDose, false, 0.0);
        return;
    }
    if (!cartridge) {
        std::cout << "[InsulinDeliveryManager] [Error] No cartridge present.\n";
        return;
    }
    if (immediateAmount > totalDose) {
        std::cout << "[InsulinDeliveryManager] [Error] Immediate amount cannot exceed total dose.\n";
        return;
    }
    if (splits <= 0) {
        std::cout << "[InsulinDeliveryManager] [Error] Number of splits must be positive.\n";
        return;
    }
    if (!cartridge->useInsulin(immediateAmount)) {
        std::cout << "[InsulinDeliveryManager] [Error] Cartridge usage failed for immediate portion.\n";
        return;
    }
    insulinOnBoard -= immediateAmount;
    std::cout << "[InsulinDeliveryManager] Delivered immediate portion of " << immediateAmount << " units.\n";

    double remainingDose = totalDose - immediateAmount;
    double perSplitDose = remainingDose / splits;
    double interval = duration / splits; // in minutes
    std::cout << "[InsulinDeliveryManager] Scheduling extended bolus: " << remainingDose
              << " units in " << splits << " splits (" << perSplitDose
              << " units every " << interval << " minutes).\n";
    for (int i = 1; i <= splits; i++) {
        ExtendedDoseEvent event;
        event.dose = perSplitDose;
        event.scheduledTime = i * interval;
        extendedSchedule.push_back(event);
    }
}

void InsulinDeliveryManager::processScheduledExtendedDoses(double currentSimTime) {
    for (auto it = extendedSchedule.begin(); it != extendedSchedule.end(); ) {
        if (currentSimTime >= it->scheduledTime) {
            if (!cartridge) {
                std::cout << "[InsulinDeliveryManager] [Error] No cartridge during scheduled delivery.\n";
                it = extendedSchedule.erase(it);
                continue;
            }
            if (cartridge->useInsulin(it->dose)) {
                insulinOnBoard += it->dose;
                std::cout << "[InsulinDeliveryManager] Delivered scheduled extended dose of "
                          << it->dose << " units at time " << currentSimTime << " minutes.\n";
            } else {
                std::cout << "[InsulinDeliveryManager] [Error] Cartridge usage failed for scheduled dose.\n";
            }
            it = extendedSchedule.erase(it);
        } else {
            ++it;
        }
    }
}

void InsulinDeliveryManager::startBasalDelivery(double rate) {
    if (!battery) {
        std::cout << "[InsulinDeliveryManager] [Error] No battery present.\n";
        return;
    }
    if (battery->getLevel() < 20) {
        std::cout << "[InsulinDeliveryManager] [Error] Battery level is low (" << battery->getLevel() << "%).\n";
        return;
    }
    if (!cartridge) {
        std::cout << "[InsulinDeliveryManager] [Error] No cartridge present.\n";
        return;
    }
    std::cout << "[InsulinDeliveryManager] Cartridge volume: " << cartridge->getCurrentVolume() << " units.\n";
    if (cartridge->getCurrentVolume() < 1.0) {
        std::cout << "[InsulinDeliveryManager] [Error] Cartridge nearly empty. Basal not started.\n";
        return;
    }
    if (rate <= 0.0) {
        std::cout << "[InsulinDeliveryManager] [Error] Invalid basal rate: " << rate << " U/hr\n";
        return;
    }
    if (basalRunning) {
        std::cout << "[InsulinDeliveryManager] [Warning] Basal already running at " << currentBasalRate << " U/hr. Updating.\n";
    }
    currentBasalRate = rate;
    basalRunning = true;
    std::cout << "[InsulinDeliveryManager] Basal started at " << rate << " U/hr\n";
}

void InsulinDeliveryManager::stopBasalDelivery() {
    basalRunning = false;
    std::cout << "[InsulinDeliveryManager] Basal stopped.\n";
}

void InsulinDeliveryManager::resumeBasalDelivery() {
    if (basalRunning) {
        std::cout << "[InsulinDeliveryManager] [Warning] Basal already running. Cannot resume.\n";
        return;
    }
    if (currentBasalRate <= 0.0) {
        std::cout << "[InsulinDeliveryManager] [Error] No previous basal rate stored.\n";
        return;
    }
    if (!battery || !cartridge || cartridge->getCurrentVolume() < 1.0) {
        std::cout << "[InsulinDeliveryManager] [Error] Cannot resume basal due to hardware issues.\n";
        return;
    }
    basalRunning = true;
    std::cout << "[InsulinDeliveryManager] Basal resumed at " << currentBasalRate << " U/hr\n";
}

void InsulinDeliveryManager::updateIOB(double elapsedTime) {
    double decayRate = 0.5;  // units per hour decay (placeholder)
    double reduceAmount = decayRate * elapsedTime;
    if (reduceAmount > insulinOnBoard)
        insulinOnBoard = 0.0;
    else
        insulinOnBoard -= reduceAmount;
}

bool InsulinDeliveryManager::hasSufficientInsulin(double requiredUnits) {
    return cartridge && (cartridge->getCurrentVolume() >= requiredUnits);
}

void InsulinDeliveryManager::onTick(double elapsedTime) {
    if (basalRunning) {
        double insulinRequired = currentBasalRate * (elapsedTime / 60.0);
        double availableInsulin = cartridge->getCurrentVolume();
        double insulinToDeliver = (availableInsulin < insulinRequired) ? availableInsulin : insulinRequired;
        if (cartridge->useInsulin(insulinToDeliver)) {
            insulinOnBoard += insulinToDeliver;
            std::cout << "[InsulinDeliveryManager] Delivered basal dose of " << insulinToDeliver << " units.\n";
        } else {
            std::cout << "[InsulinDeliveryManager] [Error] Failed to deliver basal dose.\n";
        }
    }
    updateIOB(elapsedTime);
}

double InsulinDeliveryManager::getCurrentBasalRate() const { return currentBasalRate; }
void InsulinDeliveryManager::setCurrentBasalRate(double rate) { currentBasalRate = rate; }
double InsulinDeliveryManager::getInsulinOnBoard() const { return insulinOnBoard; }
void InsulinDeliveryManager::setInsulinOnBoard(double iob) { insulinOnBoard = iob; }
bool InsulinDeliveryManager::isBasalRunning() const { return basalRunning; }
void InsulinDeliveryManager::setBasalRunning(bool running) { basalRunning = running; }
void InsulinDeliveryManager::setBattery(Battery* bat) { battery = bat; }
