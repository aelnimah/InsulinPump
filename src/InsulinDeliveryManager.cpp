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
    // Nothing dynamically owned directly here
}

// Handles a quick/immediate bolus (or error if extended params omitted)
void InsulinDeliveryManager::deliverBolus(double amount, bool extended, double duration) {
    if (!cartridge) {
        std::cout << "[Error] No cartridge present.\n";
        return;
    }
    if (cartridge->getCurrentVolume() < amount) {
        std::cout << "[Error] Insufficient insulin. Bolus canceled.\n";
        return;
    }

    if (!extended) {
        if (!cartridge->useInsulin(amount)) {
            std::cout << "[Error] Cartridge usage failed. Bolus not delivered.\n";
            return;
        }
        insulinOnBoard += amount;
        std::cout << "[Bolus] Delivered immediate bolus of " << amount << " units.\n";
    } else {
        std::cout << "[Error] Extended bolus parameters not provided.\n";
    }
}

// Handles extended bolus: immediate portion + scheduled split delivery
void InsulinDeliveryManager::deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits) {
    if (!extended) {
        deliverBolus(totalDose, false, 0.0);
        return;
    }

    if (!cartridge) {
        std::cout << "[Error] No cartridge present.\n";
        return;
    }
    if (immediateAmount > totalDose) {
        std::cout << "[Error] Immediate portion exceeds total dose.\n";
        return;
    }
    if (splits <= 0) {
        std::cout << "[Error] Invalid split count.\n";
        return;
    }

    if (!cartridge->useInsulin(immediateAmount)) {
        std::cout << "[Error] Failed to deliver immediate portion.\n";
        return;
    }

    insulinOnBoard += immediateAmount;
    std::cout << "[Bolus] Delivered immediate portion of " << immediateAmount << " units.\n";

    double remainingDose = totalDose - immediateAmount;
    double perSplit = remainingDose / splits;
    double interval = duration / splits;

    for (int i = 1; i <= splits; ++i) {
        extendedSchedule.push_back({ perSplit, i * interval });
    }

    std::cout << "[Bolus] Scheduled " << remainingDose << " units across " << splits
              << " splits (" << perSplit << " U every " << interval << " min).\n";
}

// CLI-only: Extended bolus with explicit simulation time for debug output
void InsulinDeliveryManager::deliverBolus(double totalDose, bool extended, double immediateAmount, double duration, int splits, double currentSimTime) {
    if (!extended) {
        deliverBolus(totalDose, false, 0.0);
        return;
    }

    if (!cartridge) {
        std::cout << "[Error] No cartridge present.\n";
        return;
    }

    if (immediateAmount > totalDose) {
        std::cout << "[Error] Immediate portion exceeds total dose.\n";
        return;
    }

    if (splits <= 0) {
        std::cout << "[Error] Invalid split count.\n";
        return;
    }

    if (!cartridge->useInsulin(immediateAmount)) {
        std::cout << "[Error] Failed to deliver immediate portion.\n";
        return;
    }

    insulinOnBoard += immediateAmount;
    std::cout << "[Bolus] Delivered immediate portion of " << immediateAmount << " units.\n";

    double remainingDose = totalDose - immediateAmount;
    double perSplit = remainingDose / splits;
    double interval = duration / splits;

    for (int i = 1; i <= splits; ++i) {
        double scheduled = currentSimTime + i * interval;
        extendedSchedule.push_back({ perSplit, scheduled });
    }

    std::cout << "[Bolus] Scheduled " << remainingDose << " units across " << splits
              << " splits (" << perSplit << " U every " << interval << " min starting at t=" << currentSimTime << ").\n";
}

// Executes scheduled bolus segments based on simulation time
void InsulinDeliveryManager::processScheduledExtendedDoses(double currentSimTime) {
    for (auto it = extendedSchedule.begin(); it != extendedSchedule.end(); ) {
        if (currentSimTime >= it->scheduledTime) {
            if (!cartridge) {
                std::cout << "[Error] No cartridge during scheduled delivery.\n";
                it = extendedSchedule.erase(it);
                continue;
            }

            if (cartridge->useInsulin(it->dose)) {
                insulinOnBoard += it->dose;
                std::cout << "[Bolus] Delivered scheduled extended dose of "
                          << it->dose << " units at t=" << currentSimTime << " min.\n";
            } else {
                std::cout << "[Error] Failed to deliver scheduled extended dose.\n";
            }
            it = extendedSchedule.erase(it);
        } else {
            ++it;
        }
    }
}

// Begins continuous basal delivery
void InsulinDeliveryManager::startBasalDelivery(double rate) {
    if (!battery || battery->getLevel() < 20) {
        std::cout << "[Error] Battery too low.\n";
        return;
    }
    if (!cartridge || cartridge->getCurrentVolume() < 1.0) {
        std::cout << "[Error] Cartridge too low.\n";
        return;
    }
    if (rate <= 0.0) {
        std::cout << "[Error] Invalid basal rate.\n";
        return;
    }

    if (basalRunning) {
        if (std::abs(currentBasalRate - rate) < 0.0001) {
            // Rate is the same, no need to log or restart
            return;
        }
        std::cout << "[Warning] Basal already running at " << currentBasalRate << " U/hr. Updating...\n";
    }    

    currentBasalRate = rate;
    basalRunning = true;
    std::cout << "[Basal] Rate: " << rate << " U/hr.\n";
}

void InsulinDeliveryManager::stopBasalDelivery() {
    basalRunning = false;
    std::cout << "[Basal] Stopped.\n";
}

void InsulinDeliveryManager::resumeBasalDelivery() {
    if (basalRunning) {
        std::cout << "[Warning] Basal already running.\n";
        return;
    }
    if (currentBasalRate <= 0.0 || !battery || !cartridge || cartridge->getCurrentVolume() < 1.0) {
        std::cout << "[Error] Cannot resume basal — invalid state.\n";
        return;
    }

    basalRunning = true;
    std::cout << "[Basal] Resumed at " << currentBasalRate << " U/hr.\n";
}

// Simulates IOB decay based on elapsed time
void InsulinDeliveryManager::updateIOB(double elapsedTime) {
    double decayRate = 0.05; // 3U/hour
    double decay = decayRate * elapsedTime;

    insulinOnBoard = std::max(0.0, insulinOnBoard - decay);

    std::cout << "[IOB] Current insulin on board: " << insulinOnBoard << " units.\n";
}

// Returns true if cartridge has enough insulin
bool InsulinDeliveryManager::hasSufficientInsulin(double requiredUnits) {
    return cartridge && (cartridge->getCurrentVolume() >= requiredUnits);
}

// Called every tick: handles basal dose and IOB update
void InsulinDeliveryManager::onTick(double elapsedTime) {
    updateIOB(elapsedTime);

    if (!basalRunning) {
        std::cout << "[Basal] Skipped (not running).\n";
        return;
    }

    if (basalRunning) {
        double dose = currentBasalRate * (elapsedTime / 60.0); // U per tick
        if (cartridge->useInsulin(dose)) {
            insulinOnBoard += dose;
            std::cout << "[Basal] Delivered " << dose << " units.\n";
        } else {
            std::cout << "[Error] Failed basal delivery.\n";
        }
    }
}

// Accessors
double InsulinDeliveryManager::getCurrentBasalRate() const { return currentBasalRate; }
void InsulinDeliveryManager::setCurrentBasalRate(double rate) { currentBasalRate = rate; }

double InsulinDeliveryManager::getInsulinOnBoard() const { return insulinOnBoard; }
void InsulinDeliveryManager::setInsulinOnBoard(double iob) { insulinOnBoard = iob; }

bool InsulinDeliveryManager::isBasalRunning() const { return basalRunning; }
void InsulinDeliveryManager::setBasalRunning(bool running) { basalRunning = running; }

void InsulinDeliveryManager::setCartridge(Cartridge* cart) { cartridge = cart; }
void InsulinDeliveryManager::setBolusCalculator(BolusCalculator* bc) { bolusCalculator = bc; }
void InsulinDeliveryManager::setBattery(Battery* bat) { battery = bat; }
