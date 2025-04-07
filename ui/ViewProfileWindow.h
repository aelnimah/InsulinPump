#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "Profile.h"
#include "ProfileCRUDController.h"

class ProfileManager;

class ViewProfileWindow : public QWidget {
    Q_OBJECT

public:
ViewProfileWindow(Profile* profile, ProfileCRUDController* controller, ProfileManager* mgr, QWidget* parent = nullptr);
    void setProfile(Profile* profile);

signals:
    void backToProfiles();

private slots:
    void onUpdateClicked();
    void onSaveClicked();

private:
    void setupUI();
    void loadProfileData();

    Profile* currentProfile;
    ProfileCRUDController* controller;
    ProfileManager* profileManager;

    QLineEdit* nameEdit;
    QLineEdit* icrEdit;
    QLineEdit* corrEdit;
    QLineEdit* targetBGEdit;

    QPushButton* updateButton;
    QPushButton* saveButton;
    QPushButton* backButton;
};
