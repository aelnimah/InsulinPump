/*
AlertManager
    - Purpose: Monitors hardware states and raises alerts using Alarm objects.
    - Spec Refs:
        + Handle Pump Malfunction – Triggers and manages alerts for low battery, cartridge issues, etc.
        + View Pump Info & History – Supplies active alarm info for review or display.
    - Design Notes:
        + Avoids duplicate alarms.
        + Can be extended with custom thresholds (via Profile).
        + Prepares for future GUI integration and DataLogger connectivity.
    - Class Overview:
        + checkBattery() – Checks battery and raises "BAT_LOW" if < 20%.
        + checkCartridge() – Checks cartridge volume and raises "CARTRIDGE_EMPTY" if < 1.0U.
        + raiseAlarm() – Adds new active alarm if not already present.
        + clearAlarm() – Acknowledges alarm by ID.
        + update() – Outputs current alarm statuses.
*/

#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H

#include <string>
#include <vector>

class Alarm;
class Profile;
class Battery;
class Cartridge;

class AlertManager {
private:
    std::vector<Alarm*> activeAlarms;
    Profile* profile;

public:
    AlertManager();
    ~AlertManager();

    void checkBattery(Battery* batt);
    void checkCartridge(Cartridge* cart);
    void raiseAlarm(Alarm* alarm);
    void clearAlarm(const std::string &alarmId);
    void update(); // Output or refresh active alarms

    Profile* getProfile() const;
    void setProfile(Profile* p);
};

#endif // ALERTMANAGER_H
