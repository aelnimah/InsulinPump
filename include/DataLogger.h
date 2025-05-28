/*
DataLogger
    - Purpose: Records simulation events (e.g., bolus, basal, errors) for review in logs or GUI history.
    - Spec Refs:
        + View Pump Info & History – Stores all relevant events for user inspection.
        + Handle Pump Malfunction – Records alerts and system errors.
        + Deliver Manual Bolus, Start/Stop Basal – Captures actions for verification/debug.
    - Design Notes:
        + Stores timestamp-free strings (can be extended later).
        + Lightweight API for logEvent and event retrieval.
        + Can be injected into subsystems (delivery manager, Control IQ, etc.).
    - Class Overview:
        + logEvent(type, details) – Adds new event to log.
        + getEvents() – Returns all logged entries as a vector of strings.
*/

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <string>
#include <vector>

class DataLogger {
private:
    std::vector<std::string> events;

public:
    DataLogger();
    ~DataLogger();

    void logEvent(const std::string &eventType, const std::string &details);
    std::vector<std::string> getEvents() const;
};

#endif // DATALOGGER_H
