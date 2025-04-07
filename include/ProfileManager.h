#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <string>
#include <vector>

class Profile;

// Manages collection of profiles and active profile
class ProfileManager {
private:
    std::vector<Profile*> profiles;  // List of all profiles
    Profile* activeProfile;          // Current active profile

public:
    ProfileManager();
    ~ProfileManager();

    void createProfile(Profile* newProfile);
    Profile* getProfileByName(const std::string& name);
    void updateProfile(Profile* updatedProfile);
    void deleteProfile(const std::string& name);

    Profile* getActiveProfile() const;
    void setActiveProfile(const std::string& profileName);

    std::vector<Profile*> getAllProfiles() const;
};

#endif // PROFILEMANAGER_H
