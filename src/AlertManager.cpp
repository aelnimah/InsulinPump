#include "AlertManager.h"
#include "Alarm.h"
#include "Battery.h"
#include "Cartridge.h"
#include "Profile.h"
#include <iostream>
#include <sstream>

AlertManager::AlertManager() : profile(nullptr) {}

AlertManager::~AlertManager() {
    for (Alarm* alarm : activeAlarms) {
        delete alarm;
    }
    activeAlarms.clear();
}

// Raises "BAT_LOW" if battery level < 20%
void AlertManager::checkBattery(Battery* batt) {
    if (!batt) return;
    int level = batt->getLevel();
    if (level < 20) {
        Alarm* alarm = new Alarm();
        alarm->setAlarmId("BAT_LOW");
        std::ostringstream msg;
        msg << "Battery level is low: " << level << "%.";
        alarm->setMessage(msg.str());
        alarm->setSeverity("critical");
        raiseAlarm(alarm);
    }
}

// Raises "CARTRIDGE_EMPTY" if insulin < 1.0 units
void AlertManager::checkCartridge(Cartridge* cart) {
    if (!cart) return;
    double volume = cart->getCurrentVolume();
    if (volume < 1.0) {
        Alarm* alarm = new Alarm();
        alarm->setAlarmId("CARTRIDGE_EMPTY");
        std::ostringstream msg;
        msg << "Cartridge nearly empty: " << volume << " units remaining.";
        alarm->setMessage(msg.str());
        alarm->setSeverity("warning");
        raiseAlarm(alarm);
    }
}

// Only raise a new alarm if it's not already active
void AlertManager::raiseAlarm(Alarm* alarm) {
    for (Alarm* a : activeAlarms) {
        if (a->getAlarmId() == alarm->getAlarmId() && a->getIsActive()) {
            std::cout << "[AlertManager] Alarm " << alarm->getAlarmId() << " already active.\n";
            delete alarm;
            return;
        }
    }
    activeAlarms.push_back(alarm);
    std::cout << "[AlertManager] Alarm raised: " << alarm->getMessage() << "\n";
}

// Acknowledge and deactivate an alarm by ID
void AlertManager::clearAlarm(const std::string &alarmId) {
    for (Alarm* alarm : activeAlarms) {
        if (alarm->getAlarmId() == alarmId && alarm->getIsActive()) {
            alarm->acknowledge();
            std::cout << "[AlertManager] Alarm " << alarmId << " acknowledged and cleared.\n";
            return;
        }
    }
    std::cout << "[AlertManager] Alarm " << alarmId << " not found.\n";
}

// Print all currently active alarms
void AlertManager::update() {
    std::cout << "[AlertManager] Updating alarms. Active alarms:\n";
    for (Alarm* a : activeAlarms) {
        if (a->getIsActive()) {
            std::cout << "  Alarm ID: " << a->getAlarmId()
                      << ", Message: " << a->getMessage()
                      << ", Severity: " << a->getSeverity() << "\n";
        }
    }
}

Profile* AlertManager::getProfile() const { return profile; }
void AlertManager::setProfile(Profile* p) { profile = p; }

