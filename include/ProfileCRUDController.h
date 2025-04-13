/*
ProfileCRUDController
    - Purpose: Acts as a controller between the GUI and ProfileManager to handle Create, Update, and Delete actions.
    - Spec Refs: Use Case - Manage Personal Profiles (CRUD)
    - Design Notes:
        + Uses ProfileManager to handle business logic.
        + Creates a default basal segment from 0 to 24h at 1.0 U/hr (can be refined via UI later).
    - Class Overview:
        + createProfile() – Creates a profile with initial values and default basal.
        + updateProfile() – Updates profile fields (name, ratios, targets).
        + deleteProfile() – Deletes profile by name.
*/

#ifndef PROFILECRUDCONTROLLER_H
#define PROFILECRUDCONTROLLER_H

#include <string>

class ProfileManager; // Forward declaration

class ProfileCRUDController {
private:
    ProfileManager* profileManager; // Handles the actual profile logic

public:
    ProfileCRUDController(ProfileManager* mgr);

    void createProfile(const std::string& name, double icr, double corr, double tbg);
    void updateProfile(const std::string& oldName, const std::string& newName, double icr, double corr, double tbg);
    void deleteProfile(const std::string& name);
};

#endif // PROFILECRUDCONTROLLER_H
