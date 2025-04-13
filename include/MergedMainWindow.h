#ifndef MERGEDMAINWINDOW_H
#define MERGEDMAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>

#include "PumpSimulator.h"


class QTimer;
class QLabel;
class QWidget;
class QStackedWidget;

class DataLogger;
class Profile;
class ProfileCRUDController;
class ProfileManager;

class BolusManager;
class CGMSensorInterface;
class Battery;
class Cartridge;
class ControlIQController;
class AlertManager;

class MergedMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MergedMainWindow(PumpSimulator* simulator, ProfileManager* mgr, QWidget* parent = nullptr);
    ~MergedMainWindow();

private slots:
    void onSimulationTick();

private:
    // Home Page
    void setupHomePage();
    void showHomePage();

    // Options Page
    void setupOptionsPage();
    void showOptionsPage();

    // CRUD
    void setupProfilesPage();
    void showPersonalProfilesPage();
    void refreshProfilesList();
    void setupAddProfilePage();
    void showAddProfilePage();
    void setupViewProfilePage();
    void showViewProfilePage(Profile* profile);

    // Bolus Use Case
    void setupBolusInputPage();
    void showBolusPage();
    void showBolusConfirmationPage(double dose);
    void showExtendedBolusConfigPage(double dose);

    DataLogger* dataLogger = nullptr;

    PumpSimulator* pumpSimulator = nullptr;

    ProfileManager* profileManager = nullptr;
    ProfileCRUDController* crudController = nullptr;
    Profile* currentProfile = nullptr;

    InsulinDeliveryManager* insulinDeliveryMgr = nullptr;
    CGMSensorInterface* cgmInterface = nullptr;
    Battery* battery = nullptr;
    Cartridge* cartridge = nullptr;
    ControlIQController* controlIQ = nullptr;
    AlertManager* alertManager = nullptr;
    BolusManager* bolusManager = nullptr;

    QStackedWidget* stackedWidget = nullptr;
    QLabel* simTimeLabel = nullptr;
    QTimer* simulationTimer = nullptr;
    QTime simulationTime;

    QLabel* iobLabel = nullptr;
    QLabel* bgLabel = nullptr;

    QWidget* homePage = nullptr;
    QWidget* optionsPage = nullptr;

    QWidget* profilesPage = nullptr;
    QListWidget* profileList = nullptr;
    QWidget* addProfilePage = nullptr;
    QWidget* viewProfilePage = nullptr;

    QWidget* bolusInputPage = nullptr;
    QWidget* bolusConfirmationPage = nullptr;
    QLineEdit* bolusDoseEdit = nullptr;
    QWidget* extendedBolusPage = nullptr;
};

#endif // MERGEDMAINWINDOW_H
