#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <vector>
#include "BasalSegment.h"

// Represents Personal Profile
class Profile {
private:
    std::string name;
    std::vector<BasalSegment*> basalSegments;
    double insulinToCarbRatio;
    double correctionFactor;
    double targetBG;

public:
    Profile();
    ~Profile(); 

    bool isValid() const;
    double getBasalRateForTime(double hour) const;

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
