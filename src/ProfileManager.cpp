#include "ProfileManager.h"
#include "Profile.h"
#include <iostream>
#include <algorithm>

// Constructor: Start with no active profile
ProfileManager::ProfileManager() : activeProfile(nullptr) {}

// Destructor: Delete all stored profiles
ProfileManager::~ProfileManager() {
    for (auto profile : profiles) {
        delete profile;
    }
    profiles.clear();
}

// Create a new profile if valid
void ProfileManager::createProfile(Profile* newProfile) {
    if (newProfile && newProfile->isValid()) {
        profiles.push_back(newProfile);
        std::cout << "Profile '" << newProfile->getName() << "' created.\n";
    } else {
        std::cout << "Invalid profile. Not created.\n";
    }
}

// Return profile with matching name
Profile* ProfileManager::getProfileByName(const std::string& name) {
    for (auto* profile : profiles) {
        if (profile->getName() == name) {
            return profile;
        }
    }
    return nullptr;
}

// Update existing profile with new data
void ProfileManager::updateProfile(Profile* updatedProfile) {
    for (size_t i = 0; i < profiles.size(); ++i) {
        if (profiles[i]->getName() == updatedProfile->getName()) {
            delete profiles[i];  // Clean up old data
            profiles[i] = updatedProfile;
            std::cout << "Profile '" << updatedProfile->getName() << "' updated.\n";
            return;
        }
    }
    std::cout << "Profile not found for update.\n";
}

// Delete profile with given name
void ProfileManager::deleteProfile(const std::string& name) {
    auto it = std::remove_if(profiles.begin(), profiles.end(), [&](Profile* p) {
        if (p->getName() == name) {
            // Reset active profile if neccesary
            if (activeProfile == p) {
                activeProfile = nullptr;
            }
            delete p;
            std::cout << "Profile '" << name << "' deleted.\n";
            return true;
        }
        return false;
    });
    profiles.erase(it, profiles.end());
}

// Return active profile
Profile* ProfileManager::getActiveProfile() const {
    return activeProfile;
}

// Set active profile if exists
void ProfileManager::setActiveProfile(const std::string& profileName) {
    Profile* found = getProfileByName(profileName);
    if (found) {
        activeProfile = found;
        std::cout << "Active profile set to '" << profileName << "'.\n";
    } else {
        std::cout << "Profile not found.\n";
    }
}

// Return list of all profiles
std::vector<Profile*> ProfileManager::getAllProfiles() const {
    return profiles;
}
