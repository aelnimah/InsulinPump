/*
Profile
    - Purpose: Represents a personal insulin delivery profile used for basal and bolus calculations.
    - Spec Refs: Use Case - Manage Personal Profiles (CRUD); Project Spec Section 3 (Insulin Delivery Settings)
    - Design Notes: 
        + Stores time-based basal segments and key bolus parameters (IC ratio, correction factor, target BG).
        + Validated via isValid() before being used in calculations.
    - Class Overview:
        + isValid() – Validates the profile’s fields and basal segments.
        + getBasalRateForTime(hour) – Returns rate for a given hour from basal segments.
        + addBasalSegment() – Adds a new time-segmented basal rate.
*/

#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <vector>
#include "BasalSegment.h"

class Profile {
private:
    std::string name;  // Profile name (e.g., "Morning Routine")
    std::vector<BasalSegment*> basalSegments; // Time-based basal rate segments
    double insulinToCarbRatio;   // Grams of carbs covered by 1 unit of insulin
    double correctionFactor;     // BG drop per unit of insulin
    double targetBG;             // Target blood glucose level (mmol/L)

public:
    Profile();
    ~Profile();

    bool isValid() const;  // Check if all profile fields and segments are valid
    double getBasalRateForTime(double hour) const; // Returns basal rate for a specific time

    std::string getName() const;
    void setName(const std::string &n);

    double getInsulinToCarbRatio() const;
    void setInsulinToCarbRatio(double ratio);

    double getCorrectionFactor() const;
    void setCorrectionFactor(double factor);

    double getTargetBG() const;
    void setTargetBG(double bg);

    const std::vector<BasalSegment*>& getBasalSegments() const;
    void addBasalSegment(BasalSegment* segment);
};

#endif // PROFILE_H
