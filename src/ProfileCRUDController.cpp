#include "ProfileCRUDController.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "BasalSegment.h"
#include <iostream>

// Constructor: Stores ProfileManager instance
ProfileCRUDController::ProfileCRUDController(ProfileManager* mgr) : profileManager(mgr) {}

// Create a new profile, add default basal segment, delegate to ProfileManager
void ProfileCRUDController::createProfile(const std::string& name, double icr, double corr, double tbg) {
    Profile* p = new Profile();
    p->setName(name);
    p->setInsulinToCarbRatio(icr);
    p->setCorrectionFactor(corr);
    p->setTargetBG(tbg);

    // Add default basal segment covering full day
    BasalSegment* seg = new BasalSegment(0.0, 24.0, 1.0);
    p->addBasalSegment(seg);

    profileManager->createProfile(p);
}

// Update profile settings for profile with specified old name
void ProfileCRUDController::updateProfile(const std::string& oldName, const std::string& newName, double icr, double corr, double tbg) {
    Profile* profile = profileManager->getProfileByName(oldName);
    if (profile) {
        profile->setName(newName);
        profile->setInsulinToCarbRatio(icr);
        profile->setCorrectionFactor(corr);
        profile->setTargetBG(tbg);
    }
}

// Delete profile with given name via ProfileManager
void ProfileCRUDController::deleteProfile(const std::string& name) {
    profileManager->deleteProfile(name);
}
