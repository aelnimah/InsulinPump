/*
Alarm
    - Purpose: Represents an alert or error raised during simulation (e.g., low battery, occlusion, etc.).
    - Spec Refs:
        + Handle Pump Malfunction – Triggered for battery, insulin, occlusion, and CGM failures.
        + View Pump Info & History – Can be logged and reviewed as part of the alert history.
    - Design Notes:
        + Timestamped with standard C++ time.
        + Severity can be "info", "warning", "critical".
        + Supports acknowledgement (clears active flag).
    - Class Overview:
        + acknowledge() – Marks alarm as no longer active.
        + isCritical() – Returns true if severity is "critical".
        + Get/set methods for ID, message, severity, timestamp, and status.
*/

#ifndef ALARM_H
#define ALARM_H

#include <string>
#include <ctime>

class Alarm {
private:
    std::string alarmId;     // Unique identifier (e.g., "LOW_BATTERY")
    std::string message;     // Display message for UI/log
    std::string severity;    // "info", "warning", "critical"
    std::time_t timestamp;   // Time alarm was raised
    bool isActive;           // True if alarm is unacknowledged

public:
    Alarm();
    ~Alarm();

    void acknowledge();
    bool isCritical() const;

    std::string getAlarmId() const;
    void setAlarmId(const std::string &id);

    std::string getMessage() const;
    void setMessage(const std::string &msg);

    std::string getSeverity() const;
    void setSeverity(const std::string &sev);

    std::time_t getTimestamp() const;
    void setTimestamp(std::time_t ts);

    bool getIsActive() const;
    void setIsActive(bool active);
};

#endif // ALARM_H
