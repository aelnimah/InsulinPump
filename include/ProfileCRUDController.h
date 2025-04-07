#ifndef PROFILECRUDCONTROLLER_H
#define PROFILECRUDCONTROLLER_H

#include <string>

class ProfileManager; // Forward declaration

// Provides CRUD operations for profiles via ProfileManager
class ProfileCRUDController {
private:
    ProfileManager* profileManager; // Manager to delegate profile operations

public:
    ProfileCRUDController(ProfileManager* mgr);

    void createProfile(const std::string& name, double icr, double corr, double tbg);
    void updateProfile(const std::string& oldName, const std::string& newName, double icr, double corr, double tbg);
    void deleteProfile(const std::string& name);
};

#endif // PROFILECRUDCONTROLLER_H
