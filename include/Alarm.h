#ifndef ALARM_H
#define ALARM_H

#include <string>
#include <ctime>

/*
 * Alarm
 * ----- 
 * This class represents an alarm or alert triggered by the simulation system.
 * It stores details such as the alarm ID, message, severity, timestamp, and status.
 *
 * Use Cases Supported:
 * - Handling and acknowledging errors in the pump system.
 */
class Alarm {
private:
    std::string alarmId;
    std::string message;
    std::string severity;
    std::time_t timestamp;
    bool isActive;

public:
    Alarm();
    ~Alarm();

    // Acknowledge this alarm.
    void acknowledge();
    // Check if the alarm is critical.
    bool isCritical() const;

    // Getters and setters.
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
