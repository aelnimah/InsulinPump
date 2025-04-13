#include "ProfileManager.h"
#include "Profile.h"
#include <iostream>
#include <algorithm>

// Constructor – start with no active profile
ProfileManager::ProfileManager() : activeProfile(nullptr) {}

// Destructor – delete all dynamically allocated profiles
ProfileManager::~ProfileManager() {
    for (auto profile : profiles)
        delete profile;
    profiles.clear();
}

// Adds a new profile to the list if it's valid
void ProfileManager::createProfile(Profile* newProfile) {
    if (newProfile && newProfile->isValid()) {
        profiles.push_back(newProfile);
        std::cout << "Profile '" << newProfile->getName() << "' created.\n";
    } else {
        std::cout << "Invalid profile. Not created.\n";
    }
}

// Retrieves a profile object by its name
Profile* ProfileManager::getProfileByName(const std::string& name) {
    for (auto* profile : profiles)
        if (profile->getName() == name)
            return profile;
    return nullptr;
}

// Updates an existing profile by name – replaces and deletes the old one
void ProfileManager::updateProfile(Profile* updatedProfile) {
    for (size_t i = 0; i < profiles.size(); ++i) {
        if (profiles[i]->getName() == updatedProfile->getName()) {
            delete profiles[i]; // Delete old profile to avoid memory leak
            profiles[i] = updatedProfile;
            std::cout << "Profile '" << updatedProfile->getName() << "' updated.\n";
            return;
        }
    }
    std::cout << "Profile not found for update.\n";
}

// Deletes a profile by name, resets activeProfile if needed
void ProfileManager::deleteProfile(const std::string& name) {
    auto it = std::remove_if(profiles.begin(), profiles.end(), [&](Profile* p) {
        if (p->getName() == name) {
            if (activeProfile == p)
                activeProfile = nullptr;
            delete p;
            std::cout << "Profile '" << name << "' deleted.\n";
            return true;
        }
        return false;
    });
    profiles.erase(it, profiles.end());
}

// Returns the currently selected profile
Profile* ProfileManager::getActiveProfile() const {
    return activeProfile;
}

// Sets a profile as active using its name
void ProfileManager::setActiveProfile(const std::string& profileName) {
    Profile* found = getProfileByName(profileName);
    if (found) {
        activeProfile = found;
        std::cout << "Active profile set to '" << profileName << "'.\n";
    } else {
        std::cout << "Profile not found.\n";
    }
}

// Returns all profiles (const reference might be better in future)
std::vector<Profile*> ProfileManager::getAllProfiles() const {
    return profiles;
}
