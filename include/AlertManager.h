#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H

#include <string>
#include <vector>

class Alarm;
class Profile;
class Battery;
class Cartridge;

/*
 * AlertManager
 * ------------
 * This class handles alerts and error conditions.
 * It "uses" Alarm objects to create and manage alarms and also references a Profile
 * to check against user-specific settings.
 *
 * Use Cases Supported:
 * - Handling pump malfunctions (e.g., low battery, low insulin, CGM issues).
 */
class AlertManager {
private:
    std::vector<Alarm*> activeAlarms;
    Profile* profile;  // Used for user-specific alert configuration.

public:
    AlertManager();
    ~AlertManager();

    // Check the battery and raise an alarm if needed.
    void checkBattery(Battery* batt);
    // Check the cartridge and raise an alarm if needed.
    void checkCartridge(Cartridge* cart);
    // Raise an alarm.
    void raiseAlarm(Alarm* alarm);
    // Clear an alarm by its identifier.
    void clearAlarm(const std::string &alarmId);
    // Update the alert status (e.g., recheck conditions periodically).
    void update();

    // Getter and setter for the Profile.
    Profile* getProfile() const;
    void setProfile(Profile* p);
};

#endif // ALERTMANAGER_H
