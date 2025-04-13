/*
Battery
    - Purpose: Simulates the insulin pump's battery for tracking charge level.
    - Spec Refs:
        + Start/Stop/Resume Basal – Must check battery level before delivery.
        + Handle Pump Malfunction – Triggers warnings/errors if battery < threshold.
        + View Pump Info & History – Can be shown in GUI (icon or log entry).
    - Design Notes:
        + Simple integer level (0 to 100%).
        + Expandable with charge/discharge logic in future.
    - Class Overview:
        + getLevel() – Returns battery level (0–100).
        + setLevel(lvl) – Manually set battery level.
*/

#ifndef BATTERY_H
#define BATTERY_H

class Battery {
private:
    int level;  // Battery charge level (percentage)

public:
    Battery();
    ~Battery();

    int getLevel() const;
    void setLevel(int lvl);
};

#endif // BATTERY_H
