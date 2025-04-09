#include "ProfileCRUDController.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "BasalSegment.h"
#include <iostream>

ProfileCRUDController::ProfileCRUDController(ProfileManager* mgr) : profileManager(mgr) {}

void ProfileCRUDController::createProfile(const std::string& name, double icr, double corr, double tbg) {
    Profile* p = new Profile();
    p->setName(name);
    p->setInsulinToCarbRatio(icr);
    p->setCorrectionFactor(corr);
    p->setTargetBG(tbg);
    BasalSegment* seg = new BasalSegment(0.0, 24.0, 1.0);
    p->addBasalSegment(seg);
    profileManager->createProfile(p);
}

void ProfileCRUDController::updateProfile(const std::string& oldName, const std::string& newName, double icr, double corr, double tbg) {
    Profile* profile = profileManager->getProfileByName(oldName);
    if (profile) {
        profile->setName(newName);
        profile->setInsulinToCarbRatio(icr);
        profile->setCorrectionFactor(corr);
        profile->setTargetBG(tbg);
    }
}

void ProfileCRUDController::deleteProfile(const std::string& name) {
    profileManager->deleteProfile(name);
}
