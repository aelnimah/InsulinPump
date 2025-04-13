/*
ProfileManager
    - Purpose: Manages a collection of personal insulin profiles and controls the active profile.
    - Spec Refs: Use Case - Manage Personal Profiles (CRUD)
    - Design Notes:
        + Handles profile creation, retrieval, update, deletion, and active profile selection.
        + Deletes old profiles when replaced or removed to manage memory.
    - Class Overview:
        + createProfile() – Adds a new validated profile.
        + getProfileByName(name) – Looks up profile by name.
        + updateProfile() – Replaces a profile with same name.
        + deleteProfile() – Removes and deletes profile.
        + setActiveProfile(name) – Sets current active profile.
*/

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <string>
#include <vector>

class Profile;

class ProfileManager {
private:
    std::vector<Profile*> profiles;     // All stored profiles
    Profile* activeProfile;             // Currently selected profile in use

public:
    ProfileManager();
    ~ProfileManager();

    void createProfile(Profile* newProfile);              // Add a new profile
    Profile* getProfileByName(const std::string& name);   // Lookup by name
    void updateProfile(Profile* updatedProfile);          // Replace profile if exists
    void deleteProfile(const std::string& name);          // Delete by name

    Profile* getActiveProfile() const;                    // Get active profile
    void setActiveProfile(const std::string& profileName);// Set active by name

    std::vector<Profile*> getAllProfiles() const;         // Return all profiles
};

#endif // PROFILEMANAGER_H
