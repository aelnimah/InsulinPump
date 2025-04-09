#ifndef MERGEDMAINWINDOW_H
#define MERGEDMAINWINDOW_H

#include <QMainWindow>
#include <QTime>

class QStackedWidget;
class QLabel;
class QListWidget;
class QTimer;
class QWidget;
class ProfileManager;
class BolusManager;
class InsulinDeliveryManager;
class CGMSensorInterface;
class Battery;
class Cartridge;
class ControlIQController;
class AlertManager;
class ProfileCRUDController;
class DataLogger;
class Profile;

class MergedMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MergedMainWindow(ProfileManager* mgr, QWidget* parent = nullptr);
    ~MergedMainWindow();

private slots:
    void onSimulationTick();
    void showHistoryPage();

private:
    void setupHomePage();
    void setupOptionsPage();
    void setupProfilesPage();
    void setupAddProfilePage();
    void setupViewProfilePage();
    void setupBolusInputPage();
    void setupBasalControlPage();
    void setupHistoryPage();
    void refreshProfilesList();
    void updateHistoryList();
    void showBolusConfirmationPage(double dose);
    void showExtendedBolusConfigPage(double dose);
    void showHomePage();
    void showOptionsPage();
    void showPersonalProfilesPage();
    void showAddProfilePage();
    void showViewProfilePage(Profile* profile);
    void showBolusPage();
    void showBasalControlPage();

    QTimer* simulationTimer = nullptr;
    QTime simulationTime;
    int tickCount = 0;
    QStackedWidget* stackedWidget = nullptr;
    QWidget* homePage = nullptr;
    QWidget* optionsPage = nullptr;
    QWidget* profilesPage = nullptr;
    QWidget* addProfilePage = nullptr;
    QWidget* viewProfilePage = nullptr;
    QWidget* bolusInputPage = nullptr;
    QWidget* basalControlPage = nullptr;
    QWidget* historyPage = nullptr;
    QLabel* simTimeLabel = nullptr;
    QListWidget* profileList = nullptr;
    QListWidget* historyList = nullptr;
    ProfileManager* profileManager = nullptr;
    BolusManager* bolusManager = nullptr;
    InsulinDeliveryManager* insulinDeliveryMgr = nullptr;
    CGMSensorInterface* cgmInterface = nullptr;
    Battery* battery = nullptr;
    Cartridge* cartridge = nullptr;
    ControlIQController* controlIQ = nullptr;
    AlertManager* alertManager = nullptr;
    ProfileCRUDController* crudController = nullptr;
    DataLogger* dataLogger = nullptr;
    Profile* currentProfile = nullptr;
};

#endif // MERGEDMAINWINDOW_H
