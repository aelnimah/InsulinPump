#include "BolusCalculator.h"
#include "Profile.h"
#include <cmath>
#include <iostream>

BolusCalculator::BolusCalculator() {}
BolusCalculator::~BolusCalculator() {}

// Calculates recommended insulin dose based on carbs, BG, and IOB (insulin on board)
double BolusCalculator::calculateBolus(double currentBG, double carbIntake, double iob, Profile* profile) {
    double icr = profile->getInsulinToCarbRatio();     // Grams of carbs covered by 1U insulin
    double correctionFactor = profile->getCorrectionFactor(); // BG drop per unit insulin
    double targetBG = profile->getTargetBG();          // Desired BG level

    std::cout << "ICR: " << icr << ", correctionFactor: " << correctionFactor << ", targetBG: " << targetBG << "\n";

    // Carb bolus based on carb intake
    double carbBolus = carbIntake / icr;

    // Correction bolus based on BG deviation
    double correctionBolus = calculateCorrectionBolus(currentBG, targetBG, correctionFactor);

    // Total dose = carb bolus + correction - insulin already on board
    double dose = std::round(carbBolus + correctionBolus - iob);

    // Optionally add clamping or safety bounds here (e.g., min 0U)
    return dose < 0 ? 0 : dose;
}

// Splits an extended bolus 
double BolusCalculator::calculateExtendedBolusSplit(double totalBolus, double splits) {
    if (splits <= 0.0) return totalBolus;
    return std::round(totalBolus / splits);
}

// Determines how much insulin to correct high blood sugar
double BolusCalculator::calculateCorrectionBolus(double currentBG, double targetBG, double correctionFactor) {
    return (currentBG - targetBG) / correctionFactor;
}
