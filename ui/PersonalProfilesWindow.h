#ifndef PERSONALPROFILESWINDOW_H
#define PERSONALPROFILESWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "Profile.h"

class ProfileManager;
class ProfileCRUDController;
class QListWidgetItem;

class PersonalProfilesWindow : public QWidget {
    Q_OBJECT

public:
    PersonalProfilesWindow(ProfileManager* manager, ProfileCRUDController* crud, QWidget* parent = nullptr);
    
    void refreshProfileList();

private slots:
    void onAddClicked();
    void onSetActiveClicked();
    void onDeleteClicked();
    void onBackClicked();
    void onProfileDoubleClicked(QListWidgetItem* item);

private:
    ProfileManager* profileManager;
    ProfileCRUDController* controller;

    QListWidget* profileList;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* setActiveButton;
    QPushButton* backButton;

signals:
    void backToOptions();
    void goToAddProfile(); 
    void viewProfile(Profile* profile);
};

#endif
