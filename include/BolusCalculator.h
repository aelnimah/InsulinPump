#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include <cmath>

class Profile;

/*
 * BolusCalculator
 * ---------------
 * This class calculates the recommended insulin bolus dose based on inputs such as
 * current blood glucose, carbohydrate intake, and insulin on board. It supports the
 * "Deliver Manual Bolus" use case.
 */

class BolusCalculator {
public:
    BolusCalculator();
    ~BolusCalculator();

    // Calculate the recommended bolus dose.
    double calculateBolus(double currentBG, double carbIntake, double iob, Profile* profile);
    // Calculate the split amount for an extended bolus.
    double calculateExtendedBolusSplit(double totalBolus, double fraction);
    // Calculate a correction bolus based on blood glucose deviation.
    double calculateCorrectionBolus(double currentBG, double targetBG, double correctionFactor);
};

#endif // BOLUSCALCULATOR_H