/*
BasalSegment
    - Purpose: Represents a specific time window with a fixed basal insulin delivery rate (U/hr).
    - Spec Refs: 
        + Manage Personal Profiles (CRUD) – Users define basal schedules.
        + Start/Stop/Resume Basal – Delivery logic pulls from these segments.
        + Control IQ Automatic Adjustments – Basal can be increased/decreased based on CGM predictions.
    - Design Notes:
        + Each segment defines start and end time in hours (e.g., 8.0 to 12.0).
        + Intended to be stacked to cover a full 24h delivery plan.
        + Validated by Profile during creation or update.
    - Class Overview:
        + timeInSegment(hour) – Returns true if a time falls within the segment.
        + get/set methods – Access and modify segment parameters.
*/

#ifndef BASALSEGMENT_H
#define BASALSEGMENT_H

class BasalSegment {
private:
    double startTime;       // Hour of day the segment starts (0.0 to 24.0)
    double endTime;         // Hour of day the segment ends (exclusive)
    double unitsPerHour;    // Insulin delivery rate during this segment

public:
    BasalSegment();                                         // Default constructor
    BasalSegment(double start, double end, double units);   // Init with start/end/time

    bool timeInSegment(double hour) const;                  // Checks if given time falls in this segment

    double getStartTime() const;
    void setStartTime(double start);

    double getEndTime() const;
    void setEndTime(double end);

    double getUnitsPerHour() const;
    void setUnitsPerHour(double units);
};

#endif // BASALSEGMENT_H
