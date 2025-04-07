#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

class QTimer;
class QStackedWidget;
class QPushButton;
class QLabel;

class Profile;
class ProfileManager;
class ProfileCRUDController;
class OptionsWindow;
class PersonalProfilesWindow;
class AddProfileWindow;
class ViewProfileWindow;
class BolusManager;  // New
// Forward declarations for bolus dependencies (if needed)
class BolusCalculator;
class InsulinDeliveryManager;
class CGMSensorInterface;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(ProfileManager* mgr, QWidget* parent = nullptr);
    ~MainWindow();

private:
    ProfileManager* profileManager;
    ProfileCRUDController* crudController;
    QStackedWidget* stackedWidget;
    QWidget* homePage;
    OptionsWindow* optionsPage;
    PersonalProfilesWindow* profilesPage;
    AddProfileWindow* addProfilePage;
    ViewProfileWindow* viewProfilePage;

    BolusManager* bolusManager;  // New

    QTime simulationTime;
    QTimer* simulationTimer;
    QLabel* simTimeLabel;

    void setupHomePage();

private slots:
    void updateSimulationClock();
    void showOptionsPage();
    void showProfilesPage();
    void showHomePage();
    void showAddProfilePage();
    void showViewProfilePage(Profile* profile);
    void showBolusPage();
};

#endif // MAINWINDOW_H
