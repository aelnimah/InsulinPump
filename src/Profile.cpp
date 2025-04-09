#include "Profile.h"
#include "BasalSegment.h"

Profile::Profile() : insulinToCarbRatio(0.0), correctionFactor(0.0), targetBG(0.0) {}

Profile::~Profile() {
    for (auto* seg : basalSegments)
        delete seg;
    basalSegments.clear();
}

bool Profile::isValid() const {
    if (name.empty() || insulinToCarbRatio <= 0 || correctionFactor <= 0 || targetBG <= 0)
        return false;
    for (const auto& seg : basalSegments) {
        if (seg->getStartTime() >= seg->getEndTime() || seg->getUnitsPerHour() < 0)
            return false;
    }
    return true;
}

double Profile::getBasalRateForTime(double hour) const {
    for (const auto& segment : basalSegments)
        if (segment->timeInSegment(hour))
            return segment->getUnitsPerHour();
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
const std::vector<BasalSegment*>& Profile::getBasalSegments() const { return basalSegments; }
void Profile::addBasalSegment(BasalSegment* segment) { basalSegments.push_back(segment); }
