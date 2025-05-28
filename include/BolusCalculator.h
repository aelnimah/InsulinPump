/*
BolusCalculator
    - Purpose: Computes insulin doses for manual bolus delivery based on a user profile.
    - Spec Refs: 
        + Deliver Manual Bolus – Calculates recommended bolus using BG, carbs, and IOB.
        + Control IQ Auto Adjustments – Shares logic with automated correction bolus algorithms.
    - Design Notes:
        + Uses user profile parameters: ICR, Correction Factor, and Target BG.
        + Can compute extended bolus splits for spread-out insulin delivery.
        + Future expansion: incorporate safety checks, Control IQ conditions, logging.
    - Class Overview:
        + calculateBolus() – Combines carb and correction bolus, adjusts for IOB.
        + calculateExtendedBolusSplit() – Splits total bolus into fractions.
        + calculateCorrectionBolus() – Calculates correction dose based on BG delta.
*/

#ifndef BOLUSCALCULATOR_H
#define BOLUSCALCULATOR_H

#include <cmath>

class Profile;

class BolusCalculator {
public:
    BolusCalculator();
    ~BolusCalculator();

    double calculateBolus(double currentBG, double carbIntake, double iob, Profile* profile);
    double calculateExtendedBolusSplit(double totalBolus, double splits);
    double calculateCorrectionBolus(double currentBG, double targetBG, double correctionFactor);
};

#endif // BOLUSCALCULATOR_H
