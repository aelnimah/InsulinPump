#include "Alarm.h"
#include <ctime>

// Alarm is active by default and timestamped upon creation
Alarm::Alarm() 
    : timestamp(std::time(nullptr)), isActive(true) {}

Alarm::~Alarm() {}

// Marks the alarm as acknowledged/inactive
void Alarm::acknowledge() {
    isActive = false;
}

// Returns true if this alarm is critical
bool Alarm::isCritical() const {
    return severity == "critical";
}

// Getters and setters
std::string Alarm::getAlarmId() const { return alarmId; }
void Alarm::setAlarmId(const std::string &id) { alarmId = id; }

std::string Alarm::getMessage() const { return message; }
void Alarm::setMessage(const std::string &msg) { message = msg; }

std::string Alarm::getSeverity() const { return severity; }
void Alarm::setSeverity(const std::string &sev) { severity = sev; }

std::time_t Alarm::getTimestamp() const { return timestamp; }
void Alarm::setTimestamp(std::time_t ts) { timestamp = ts; }

bool Alarm::getIsActive() const { return isActive; }
void Alarm::setIsActive(bool active) { isActive = active; }
