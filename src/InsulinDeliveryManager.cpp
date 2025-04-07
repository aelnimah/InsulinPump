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
      cartridge(nullptr)
{
}

InsulinDeliveryManager::~InsulinDeliveryManager() {
    // Cleanup if needed.
}

void InsulinDeliveryManager::deliverBolus(double amount, bool extended, double duration) {
    // Immediate bolus overload (3 arguments)
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
        // If extended flag is true but detailed parameters not provided,
        // we can print an error or fallback.
        std::cout << "[InsulinDeliveryManager] [Error] Extended bolus parameters not provided.\n";
    }
}

void InsulinDeliveryManager::deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits) {
    // Extended bolus overload (5 arguments)
    if (!extended) {
        // Fallback to immediate delivery.
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
    // Deliver the immediate portion.
    if (!cartridge->useInsulin(immediateAmount)) {
        std::cout << "[InsulinDeliveryManager] [Error] Cartridge usage failed for immediate portion.\n";
        return;
    }
    insulinOnBoard -= immediateAmount;
    std::cout << "[InsulinDeliveryManager] Delivered immediate portion of " 
              << immediateAmount << " units." << std::endl;
    
    // Calculate remaining dose.
    double remainingDose = totalDose - immediateAmount;
    double perSplitDose = remainingDose / splits;
    double interval = duration / splits; // in minutes

    std::cout << "[InsulinDeliveryManager] Scheduling extended bolus: " 
              << remainingDose << " units, divided into " 
              << splits << " splits (" << perSplitDose 
              << " units every " << interval << " minutes)." << std::endl;

    // Schedule the extended doses.
    for (int i = 1; i <= splits; i++) {
        ExtendedDoseEvent event;
        event.dose = perSplitDose;
        event.scheduledTime = i * interval;  // i-th split delivered at i*interval minutes.
        extendedSchedule.push_back(event);
    }
}

void InsulinDeliveryManager::processScheduledExtendedDoses(double currentSimTime) {
    // Process and deliver scheduled extended doses if due.
    for (auto it = extendedSchedule.begin(); it != extendedSchedule.end(); ) {
        if (currentSimTime >= it->scheduledTime) {
            if (!cartridge) {
                std::cout << "[InsulinDeliveryManager] [Error] No cartridge present during scheduled delivery.\n";
                it = extendedSchedule.erase(it);
                continue;
            }
            if (cartridge->useInsulin(it->dose)) {
                insulinOnBoard += it->dose;
                std::cout << "[InsulinDeliveryManager] Delivered scheduled extended dose of " 
                          << it->dose << " units at simulated time " 
                          << currentSimTime << " minutes.\n";
            } else {
                std::cout << "[InsulinDeliveryManager] [Error] Cartridge usage failed for scheduled extended dose.\n";
            }
            it = extendedSchedule.erase(it);
        } else {
            ++it;
        }
    }
}


void InsulinDeliveryManager::startBasalDelivery(double rate) {
    if (!battery) {
        std::cout << "[InsulinDeliveryManager] [Error] No battery present. Cannot start basal.\n";
        return;
    }
    if (battery->getLevel() < 20) {
        std::cout << "[InsulinDeliveryManager] [Error] Battery level is low (" << battery->getLevel() << "%). Cannot start basal.\n";
        return;
    }
    if (!cartridge) {
        std::cout << "[InsulinDeliveryManager] [Error] No cartridge present. Cannot start basal.\n";
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
        std::cout << "[InsulinDeliveryManager] [Warning] Basal is already running at " << currentBasalRate << " U/hr. Updating to " << rate << " U/hr.\n";
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
        std::cout << "[InsulinDeliveryManager] [Warning] Basal is already running. Cannot resume.\n";
        return;
    }
    if (currentBasalRate <= 0.0) {
        std::cout << "[InsulinDeliveryManager] [Error] No previous basal rate stored. Cannot resume basal.\n";
        return;
    }
    if (!battery) {
        std::cout << "[InsulinDeliveryManager] [Error] No battery present. Cannot resume basal.\n";
        return;
    }
    if (!cartridge) {
        std::cout << "[InsulinDeliveryManager] [Error] No cartridge present. Cannot resume basal.\n";
        return;
    }
    if (cartridge->getCurrentVolume() < 1.0) {
        std::cout << "[InsulinDeliveryManager] [Error] Cartridge nearly empty. Basal not resumed.\n";
        return;
    }
    basalRunning = true;
    std::cout << "[InsulinDeliveryManager] Basal resumed at " << currentBasalRate << " U/hr\n";
}

void InsulinDeliveryManager::updateIOB(double elapsedTime) {
    double decayRate = 0.5; // 0.5 units/hr decay (placeholder)
    double reduceAmount = decayRate * elapsedTime;
    if (reduceAmount > insulinOnBoard) {
        insulinOnBoard = 0.0;
    } else {
        insulinOnBoard -= reduceAmount;
    }
}

bool InsulinDeliveryManager::hasSufficientInsulin(double requiredUnits) {
    if (!cartridge) return false;
    return (cartridge->getCurrentVolume() >= requiredUnits);
}

void InsulinDeliveryManager::onTick(double elapsedTime) {
    if (basalRunning) {
        double insulinRequired = currentBasalRate * (elapsedTime / 60.0);
        double availableInsulin = cartridge->getCurrentVolume();
        double insulinToDeliver = insulinRequired;
        if (availableInsulin < insulinRequired) {
            std::cout << "[InsulinDeliveryManager] [Warning] Insufficient insulin for full basal rate. Delivering partial dose: "
                      << availableInsulin << " units instead of " << insulinRequired << " units.\n";
            insulinToDeliver = availableInsulin;
        }
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
