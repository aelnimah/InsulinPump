#include "BasalSegment.h"

// Default: zero-duration segment, zero rate
BasalSegment::BasalSegment() 
    : startTime(0.0), endTime(0.0), unitsPerHour(0.0) {}

// Parametrized: define full segment window and delivery rate
BasalSegment::BasalSegment(double start, double end, double units)
    : startTime(start), endTime(end), unitsPerHour(units) {}

// Returns true if given hour is within the segment range
bool BasalSegment::timeInSegment(double hour) const {
    return (hour >= startTime && hour < endTime);
}

// Accessors and mutators
double BasalSegment::getStartTime() const { return startTime; }
void BasalSegment::setStartTime(double start) { startTime = start; }

double BasalSegment::getEndTime() const { return endTime; }
void BasalSegment::setEndTime(double end) { endTime = end; }

double BasalSegment::getUnitsPerHour() const { return unitsPerHour; }
void BasalSegment::setUnitsPerHour(double units) { unitsPerHour = units; }
