#ifndef MERGEDMAINWINDOW_H
#define MERGEDMAINWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "Cartridge.h"

class QTimer;
class QStackedWidget;
class QPushButton;
class QLabel;

// Forward declarations for domain (logic) classes:
class Profile;
class ProfileManager;
class ProfileCRUDController;
class BolusManager;

// This merged main window will hold our different pages for a CRUD flow.
class MergedMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MergedMainWindow(ProfileManager* mgr, QWidget *parent = nullptr);
    ~MergedMainWindow();

private slots:
    void updateSimulationClock();
    void showHomePage();
    void showOptionsPage();
    void showPersonalProfilesPage();
    void showAddProfilePage();
    void showViewProfilePage(Profile* profile);

    void showBolusPage();
    void showBolusConfirmationPage(double dose);
    void showExtendedBolusConfigPage(double dose);
private:
    // The stacked widget that holds all pages.
    QStackedWidget* stackedWidget;

    // Pages in the UI:
    QWidget* homePage;
    QWidget* optionsPage;
    QWidget* profilesPage;
    QWidget* addProfilePage;
    QWidget* viewProfilePage;

    QWidget* bolusInputPage;

    // Widgets used on the home page
    QLabel* simTimeLabel;
    QTimer* simulationTimer;
    QTime simulationTime;

    // Domain logic pointers for profiles.
    ProfileManager* profileManager;
    ProfileCRUDController* crudController;

    Profile* currentProfile;

    BolusManager* bolusManager;


    // Member widgets on the profiles page (so we can refresh the list)
    // (For simplicity, we assume profilesPage contains a QListWidget pointer stored as member.)
    class QListWidget* profileList;

    // Setup helper functions
    void setupHomePage();
    void setupOptionsPage();
    void setupProfilesPage();
    void setupAddProfilePage();
    void setupViewProfilePage();
    void setupBolusInputPage();

    // Refresh the profiles list (called when returning to the profiles page)
    void refreshProfilesList();
};

#endif // MERGEDMAINWINDOW_H
