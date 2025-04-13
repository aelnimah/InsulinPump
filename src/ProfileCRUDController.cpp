#include "ProfileCRUDController.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "BasalSegment.h"
#include <iostream>

// Constructor: injects a reference to ProfileManager
ProfileCRUDController::ProfileCRUDController(ProfileManager* mgr) : profileManager(mgr) {}

// Creates a new profile with default 24-hour basal rate (1.0 U/hr)
void ProfileCRUDController::createProfile(const std::string& name, double icr, double corr, double tbg) {
    Profile* p = new Profile();
    p->setName(name);
    p->setInsulinToCarbRatio(icr);
    p->setCorrectionFactor(corr);
    p->setTargetBG(tbg);

    // TODO: eventually allow user-defined basal segments (GUI input)
    BasalSegment* seg = new BasalSegment(0.0, 24.0, 1.0);
    p->addBasalSegment(seg);

    profileManager->createProfile(p);
}

// Updates profile values (does not replace or delete old segments)
void ProfileCRUDController::updateProfile(const std::string& oldName, const std::string& newName, double icr, double corr, double tbg) {
    Profile* profile = profileManager->getProfileByName(oldName);
    if (profile) {
        profile->setName(newName);  // Allows renaming profile
        profile->setInsulinToCarbRatio(icr);
        profile->setCorrectionFactor(corr);
        profile->setTargetBG(tbg);

        // NOTE: Basal segments are not updated here – might need a separate method or GUI control
    }
}

// Deletes a profile by name
void ProfileCRUDController::deleteProfile(const std::string& name) {
    profileManager->deleteProfile(name);
}
