#include "BolusCalculator.h"
#include "Profile.h"
#include <cmath>
#include <iostream>

BolusCalculator::BolusCalculator() {}
BolusCalculator::~BolusCalculator() {}

double BolusCalculator::calculateBolus(double currentBG, double carbIntake, double iob, Profile* profile) {
    double icr = profile->getInsulinToCarbRatio();
    double correctionFactor = profile->getCorrectionFactor();
    double targetBG = profile->getTargetBG();
    std::cout << "ICR: " << icr << ", correctionFactor: " << correctionFactor << ", targetBG: " << targetBG << "\n";
    double carbBolus = carbIntake / icr;
    double correctionBolus = calculateCorrectionBolus(currentBG, targetBG, correctionFactor);
    // Note: Adjust the formula as needed; here we subtract correctionBolus.
    double dose = std::round(carbBolus - correctionBolus);
    return dose;
}

double BolusCalculator::calculateExtendedBolusSplit(double totalBolus, double fraction) {
    return std::round(totalBolus / fraction);
}

double BolusCalculator::calculateCorrectionBolus(double currentBG, double targetBG, double correctionFactor) {
    return (currentBG - targetBG) / correctionFactor;
}
