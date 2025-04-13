#include "Profile.h"
#include "BasalSegment.h"

// Constructor initializes numeric fields to 0
Profile::Profile() : insulinToCarbRatio(0.0), correctionFactor(0.0), targetBG(0.0) {}

// Destructor deletes allocated BasalSegment pointers
Profile::~Profile() {
    for (auto* seg : basalSegments)
        delete seg;
    basalSegments.clear();
}

// Validates whether the profile has all required and meaningful values
bool Profile::isValid() const {
    if (name.empty() || insulinToCarbRatio <= 0 || correctionFactor <= 0 || targetBG <= 0)
        return false;

    // Each basal segment must have valid timing and non-negative insulin rate
    for (const auto& seg : basalSegments) {
        if (seg->getStartTime() >= seg->getEndTime() || seg->getUnitsPerHour() < 0)
            return false;
    }

    return true;
}

// Given an hour (e.g., 13.5 for 1:30pm), return the corresponding basal rate
double Profile::getBasalRateForTime(double hour) const {
    for (const auto& segment : basalSegments)
        if (segment->timeInSegment(hour))
            return segment->getUnitsPerHour();

    // If no segment applies, assume 0 U/hr (e.g., empty profile or gap in config)
    return 0.0;
}

// Getters and Setters
std::string Profile::getName() const { return name; }
void Profile::setName(const std::string &n) { name = n; }

double Profile::getInsulinToCarbRatio() const { return insulinToCarbRatio; }
void Profile::setInsulinToCarbRatio(double ratio) { insulinToCarbRatio = ratio; }

double Profile::getCorrectionFactor() const { return correctionFactor; }
void Profile::setCorrectionFactor(double factor) { correctionFactor = factor; }

double Profile::getTargetBG() const { return targetBG; }
void Profile::setTargetBG(double bg) { targetBG = bg; }

const std::vector<BasalSegment*>& Profile::getBasalSegments() const { return basalSegments; }
void Profile::addBasalSegment(BasalSegment* segment) { basalSegments.push_back(segment); }
