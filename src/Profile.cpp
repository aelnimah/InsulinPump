#include "Profile.h"
#include "BasalSegment.h"

// Constructor
Profile::Profile() : insulinToCarbRatio(0.0), correctionFactor(0.0), targetBG(0.0) {}

// Destructor: Clean up all basal segments
Profile::~Profile() {
    for (auto* seg : basalSegments) {
        delete seg;
    }
    basalSegments.clear();
}

// Validate profile settings and basal segments
bool Profile::isValid() const {
    if (name.empty() || insulinToCarbRatio <= 0 || correctionFactor <= 0 || targetBG <= 0)
        return false;
    for (const auto& seg : basalSegments) {
        if (seg->getStartTime() >= seg->getEndTime() || seg->getUnitsPerHour() < 0)
            return false;
    }
    return true;
}

// Return basal rate for specified hour based on defined segments
double Profile::getBasalRateForTime(double hour) const {
    for (const auto& segment : basalSegments) {
        if (segment->timeInSegment(hour))
            return segment->getUnitsPerHour();
    }
    return 0.0;
}

std::string Profile::getName() const { return name; }
void Profile::setName(const std::string &n) { name = n; }
double Profile::getInsulinToCarbRatio() const { return insulinToCarbRatio; }
void Profile::setInsulinToCarbRatio(double ratio) { insulinToCarbRatio = ratio; }
double Profile::getCorrectionFactor() const { return correctionFactor; }
void Profile::setCorrectionFactor(double factor) { correctionFactor = factor; }
double Profile::getTargetBG() const { return targetBG; }
void Profile::setTargetBG(double bg) { targetBG = bg; }

const std::vector<BasalSegment*>& Profile::getBasalSegments() const {
    return basalSegments;
}

// Add basal segment to profile
void Profile::addBasalSegment(BasalSegment* segment) {
    basalSegments.push_back(segment);
}
