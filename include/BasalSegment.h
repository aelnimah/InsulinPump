#ifndef BASALSEGMENT_H
#define BASALSEGMENT_H

// Represents time segment for basal insulin delivery
class BasalSegment {
private:
    double startTime;
    double endTime;
    double unitsPerHour;

public:
    BasalSegment();
    BasalSegment(double start, double end, double units);

    bool timeInSegment(double hour) const;

    double getStartTime() const;
    void setStartTime(double start);
    double getEndTime() const;
    void setEndTime(double end);
    double getUnitsPerHour() const;
    void setUnitsPerHour(double units);
};

#endif // BASALSEGMENT_H
