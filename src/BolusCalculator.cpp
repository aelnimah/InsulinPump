#include "BolusCalculator.h"
#include "Profile.h"
#include <cmath>

BolusCalculator::BolusCalculator() {}
BolusCalculator::~BolusCalculator() {}

double BolusCalculator::calculateBolus(double currentBG, double carbIntake, double iob, Profile* profile) {
    double carbBolus = carbIntake / profile->getInsulinToCarbRatio();
    double correctionBolus = calculateCorrectionBolus(currentBG, profile->getTargetBG(), profile->getCorrectionFactor());
    return std::round(carbBolus - correctionBolus);
}

double BolusCalculator::calculateExtendedBolusSplit(double totalBolus, double fraction) {
    return std::round(totalBolus / fraction);
}

double BolusCalculator::calculateCorrectionBolus(double currentBG, double targetBG, double correctionFactor) {
    return (currentBG - targetBG) / correctionFactor;
}