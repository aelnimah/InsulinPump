Here is MergedMainWindow:
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

#include "MergedMainWindow.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QVariant>
#include <QTime>
#include "ProfileManager.h"
#include "ProfileCRUDController.h"
#include "Profile.h"
#include "BolusManager.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "Cartridge.h"
#include "Battery.h"
#include "ControlIQController.h"
#include "AlertManager.h"
#include "DataLogger.h"

MergedMainWindow::MergedMainWindow(ProfileManager* mgr, QWidget* parent)
    : QMainWindow(parent),
      profileManager(mgr),
      basalControlPage(nullptr),
      bolusInputPage(nullptr),
      historyPage(nullptr),
      currentProfile(nullptr)
{
    BolusCalculator* bolusCalc = new BolusCalculator();
    insulinDeliveryMgr = new InsulinDeliveryManager();
    cgmInterface = new CGMSensorInterface();
    cartridge = new Cartridge();
    insulinDeliveryMgr->setCartridge(cartridge);
    battery = new Battery();
    insulinDeliveryMgr->setBattery(battery);
    bolusManager = new BolusManager(profileManager, bolusCalc, insulinDeliveryMgr, cgmInterface);
    controlIQ = new ControlIQController();
    controlIQ->setCGMSensor(cgmInterface);
    controlIQ->setDeliveryManager(insulinDeliveryMgr);
    controlIQ->setIsActive(true);
    alertManager = new AlertManager();
    crudController = new ProfileCRUDController(profileManager);
    dataLogger = new DataLogger();
    stackedWidget = new QStackedWidget(this);
    setupHomePage();
    setupOptionsPage();
    setupProfilesPage();
    setupAddProfilePage();
    setupViewProfilePage();
    setupBolusInputPage();
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(optionsPage);
    stackedWidget->addWidget(profilesPage);
    stackedWidget->addWidget(addProfilePage);
    stackedWidget->addWidget(viewProfilePage);
    stackedWidget->addWidget(bolusInputPage);
    stackedWidget->setCurrentWidget(homePage);
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    simTimeLabel = new QLabel("00:00", this);
    simTimeLabel->setAlignment(Qt::AlignCenter);
    simTimeLabel->setStyleSheet("font-weight: bold; font-size: 18px;");
    mainLayout->addWidget(simTimeLabel);
    mainLayout->addWidget(stackedWidget);
    container->setLayout(mainLayout);
    setCentralWidget(container);
    setWindowTitle("Insulin Pump Simulator");
    resize(480, 320);
    simulationTime = QTime(0, 0, 0);
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MergedMainWindow::onSimulationTick);
    simulationTimer->start(2000);
}

MergedMainWindow::~MergedMainWindow()
{
    delete bolusManager;
    delete insulinDeliveryMgr;
    delete cgmInterface;
    delete battery;
    delete cartridge;
    delete controlIQ;
    delete alertManager;
    delete crudController;
    delete dataLogger;
}

void MergedMainWindow::setupHomePage()
{
    homePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(homePage);
    QPushButton* optionsButton = new QPushButton("Options", homePage);
    layout->addWidget(optionsButton);
    connect(optionsButton, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);
    QPushButton* bolusButton = new QPushButton("Bolus", homePage);
    layout->addWidget(bolusButton);
    connect(bolusButton, &QPushButton::clicked, this, &MergedMainWindow::showBolusPage);
    QPushButton* historyButton = new QPushButton("History", homePage);
    layout->addWidget(historyButton);
    connect(historyButton, &QPushButton::clicked, this, &MergedMainWindow::showHistoryPage);
    homePage->setLayout(layout);
}

void MergedMainWindow::setupOptionsPage()
{
    optionsPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(optionsPage);
    QPushButton* profilesButton = new QPushButton("Personal Profiles", optionsPage);
    layout->addWidget(profilesButton);
    connect(profilesButton, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);
    QHBoxLayout* basalLayout = new QHBoxLayout();
    QPushButton* basalControlButton = new QPushButton("Basal Control", optionsPage);
    QPushButton* stopBasalButton = new QPushButton("Stop Basal", optionsPage);
    QPushButton* resumeBasalButton = new QPushButton("Resume Basal", optionsPage);
    basalLayout->addWidget(basalControlButton);
    basalLayout->addWidget(stopBasalButton);
    basalLayout->addWidget(resumeBasalButton);
    layout->addLayout(basalLayout);
    connect(basalControlButton, &QPushButton::clicked, this, &MergedMainWindow::showBasalControlPage);
    connect(stopBasalButton, &QPushButton::clicked, [=]() {
        if (insulinDeliveryMgr) {
            insulinDeliveryMgr->stopBasalDelivery();
            QMessageBox::information(this, "Basal Control", "Basal delivery stopped.");
            dataLogger->logEvent("BasalStop", "Basal delivery stopped.");
        }
    });
    connect(resumeBasalButton, &QPushButton::clicked, [=]() {
        if (insulinDeliveryMgr) {
            insulinDeliveryMgr->resumeBasalDelivery();
            QMessageBox::information(this, "Basal Control", "Basal delivery resumed.");
            dataLogger->logEvent("BasalResume", "Basal delivery resumed.");
        }
    });
    QPushButton* controlIQButton = new QPushButton("Control IQ", optionsPage);
    layout->addWidget(controlIQButton);
    connect(controlIQButton, &QPushButton::clicked, [=]() {
        if (cgmInterface && controlIQ) {
            double currentBG = cgmInterface->getCurrentBG();
            controlIQ->processSensorReading(currentBG);
            controlIQ->predictBGTrend();
            controlIQ->applyAutomaticAdjustments();
            QMessageBox::information(this, "Control IQ", QString("Control IQ applied. Predicted BG: %1 mmol/L").arg(controlIQ->getPredictedBG()));
            dataLogger->logEvent("ControlIQ", "Control IQ adjustments applied.");
        }
    });
    QHBoxLayout* fixLayout = new QHBoxLayout();
    QPushButton* chargeBatteryButton = new QPushButton("Charge Battery", optionsPage);
    QPushButton* refillCartridgeButton = new QPushButton("Refill Cartridge", optionsPage);
    fixLayout->addWidget(chargeBatteryButton);
    fixLayout->addWidget(refillCartridgeButton);
    layout->addLayout(fixLayout);
    connect(chargeBatteryButton, &QPushButton::clicked, [=]() {
        if (battery) {
            battery->setLevel(100);
            QMessageBox::information(this, "Battery", "Battery charged to 100%.");
            dataLogger->logEvent("BatteryCharge", "Battery charged.");
        }
    });
    connect(refillCartridgeButton, &QPushButton::clicked, [=]() {
        if (cartridge) {
            cartridge->refill();
            QMessageBox::information(this, "Cartridge", "Cartridge refilled.");
            dataLogger->logEvent("CartridgeRefill", "Cartridge refilled.");
        }
    });
    QPushButton* checkAlertsButton = new QPushButton("Check Alerts", optionsPage);
    layout->addWidget(checkAlertsButton);
    connect(checkAlertsButton, &QPushButton::clicked, [=]() {
        if (alertManager && battery && cartridge) {
            alertManager->checkBattery(battery);
            alertManager->checkCartridge(cartridge);
            QMessageBox::information(this, "Alerts", "Alerts checked (see console output).");
            dataLogger->logEvent("AlertsChecked", "Alerts checked.");
        }
    });
    QPushButton* homeButton = new QPushButton("Home", optionsPage);
    layout->addWidget(homeButton);
    connect(homeButton, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);
    optionsPage->setLayout(layout);
}

void MergedMainWindow::setupProfilesPage()
{
    profilesPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(profilesPage);
    profileList = new QListWidget(profilesPage);
    layout->addWidget(profileList);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add", profilesPage);
    QPushButton* setActiveBtn = new QPushButton("Set Active", profilesPage);
    QPushButton* deleteBtn = new QPushButton("Delete", profilesPage);
    QPushButton* backBtn = new QPushButton("Back", profilesPage);
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(setActiveBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(backBtn);
    layout->addLayout(buttonLayout);
    connect(addBtn, &QPushButton::clicked, this, &MergedMainWindow::showAddProfilePage);
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);
    connect(setActiveBtn, &QPushButton::clicked, [=]() {
        QListWidgetItem* selected = profileList->currentItem();
        if (selected) {
            QString name = selected->text();
            profileManager->setActiveProfile(name.toStdString());
            QMessageBox::information(this, "Active Profile", "Profile '" + name + "' is now active.");
            dataLogger->logEvent("ProfileActive", "Active profile set to " + name.toStdString());
            refreshProfilesList();
        } else {
            QMessageBox::warning(this, "No Selection", "Please select a profile.");
        }
    });
    connect(deleteBtn, &QPushButton::clicked, [=]() {
        QListWidgetItem* selected = profileList->currentItem();
        if (selected) {
            QString name = selected->text();
            if (QMessageBox::question(this, "Confirm Delete", "Delete profile '" + name + "'?") == QMessageBox::Yes) {
                profileManager->deleteProfile(name.toStdString());
                dataLogger->logEvent("ProfileDelete", "Profile " + name.toStdString() + " deleted.");
                refreshProfilesList();
            }
        } else {
            QMessageBox::warning(this, "No Selection", "Please select a profile to delete.");
        }
    });
    connect(profileList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
        if (item) {
            Profile* p = profileManager->getProfileByName(item->text().toStdString());
            if (p)
                showViewProfilePage(p);
        }
    });
    profilesPage->setLayout(layout);
    refreshProfilesList();
}

void MergedMainWindow::setupAddProfilePage()
{
    addProfilePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(addProfilePage);
    QFormLayout* formLayout = new QFormLayout();
    QLineEdit* nameEdit = new QLineEdit(addProfilePage);
    QDoubleSpinBox* icrSpin = new QDoubleSpinBox(addProfilePage);
    icrSpin->setRange(0.1, 100.0);
    QDoubleSpinBox* corrSpin = new QDoubleSpinBox(addProfilePage);
    corrSpin->setRange(0.1, 100.0);
    QDoubleSpinBox* targetBGSpin = new QDoubleSpinBox(addProfilePage);
    targetBGSpin->setRange(1.0, 300.0);
    formLayout->addRow("Profile Name:", nameEdit);
    formLayout->addRow("Insulin/Carb Ratio:", icrSpin);
    formLayout->addRow("Correction Factor:", corrSpin);
    formLayout->addRow("Target BG:", targetBGSpin);
    layout->addLayout(formLayout);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* saveBtn = new QPushButton("Save Profile", addProfilePage);
    QPushButton* cancelBtn = new QPushButton("Cancel", addProfilePage);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);
    addProfilePage->setLayout(layout);
    connect(saveBtn, &QPushButton::clicked, [=]() {
        if (nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Profile name is required.");
            return;
        }
        crudController->createProfile(nameEdit->text().toStdString(), icrSpin->value(), corrSpin->value(), targetBGSpin->value());
        dataLogger->logEvent("ProfileCreate", "Profile " + nameEdit->text().toStdString() + " created.");
        refreshProfilesList();
        stackedWidget->setCurrentWidget(profilesPage);
    });
    connect(cancelBtn, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);
}

void MergedMainWindow::setupViewProfilePage()
{
    viewProfilePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(viewProfilePage);
    QFormLayout* formLayout = new QFormLayout();
    QLineEdit* nameEdit = new QLineEdit(viewProfilePage);
    nameEdit->setReadOnly(true);
    QDoubleSpinBox* icrSpin = new QDoubleSpinBox(viewProfilePage);
    icrSpin->setRange(0.1, 100.0);
    icrSpin->setDecimals(2);
    icrSpin->setSingleStep(0.1);
    icrSpin->setReadOnly(true);
    QDoubleSpinBox* corrSpin = new QDoubleSpinBox(viewProfilePage);
    corrSpin->setRange(0.1, 100.0);
    corrSpin->setDecimals(2);
    corrSpin->setSingleStep(0.1);
    corrSpin->setReadOnly(true);
    QDoubleSpinBox* targetBGSpin = new QDoubleSpinBox(viewProfilePage);
    targetBGSpin->setRange(1.0, 300.0);
    targetBGSpin->setDecimals(2);
    targetBGSpin->setSingleStep(0.1);
    targetBGSpin->setReadOnly(true);
    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("ICR:", icrSpin);
    formLayout->addRow("Correction Factor:", corrSpin);
    formLayout->addRow("Target BG:", targetBGSpin);
    layout->addLayout(formLayout);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* updateBtn = new QPushButton("Update", viewProfilePage);
    QPushButton* saveBtn = new QPushButton("Save", viewProfilePage);
    QPushButton* backBtn = new QPushButton("Back", viewProfilePage);
    saveBtn->setEnabled(false);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(backBtn);
    layout->addLayout(btnLayout);
    viewProfilePage->setLayout(layout);
    connect(updateBtn, &QPushButton::clicked, [=]() {
        nameEdit->setReadOnly(false);
        icrSpin->setReadOnly(false);
        corrSpin->setReadOnly(false);
        targetBGSpin->setReadOnly(false);
        saveBtn->setEnabled(true);
        updateBtn->setEnabled(false);
    });
    connect(saveBtn, &QPushButton::clicked, [=]() {
        if (!currentProfile)
            return;
        std::string oldName = currentProfile->getName();
        std::string newName = nameEdit->text().toStdString();
        double newICR = icrSpin->value();
        double newCorr = corrSpin->value();
        double newTBG = targetBGSpin->value();
        crudController->updateProfile(oldName, newName, newICR, newCorr, newTBG);
        currentProfile->setName(newName);
        currentProfile->setInsulinToCarbRatio(newICR);
        currentProfile->setCorrectionFactor(newCorr);
        currentProfile->setTargetBG(newTBG);
        nameEdit->setReadOnly(true);
        icrSpin->setReadOnly(true);
        corrSpin->setReadOnly(true);
        targetBGSpin->setReadOnly(true);
        saveBtn->setEnabled(false);
        updateBtn->setEnabled(true);
        QMessageBox::information(viewProfilePage, "Profile Updated", "Profile has been updated.");
        dataLogger->logEvent("ProfileUpdate", "Profile updated: " + newName);
    });
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);
    viewProfilePage->setProperty("nameEditPtr", QVariant::fromValue(static_cast<void*>(nameEdit)));
    viewProfilePage->setProperty("icrEditPtr", QVariant::fromValue(static_cast<void*>(icrSpin)));
    viewProfilePage->setProperty("corrEditPtr", QVariant::fromValue(static_cast<void*>(corrSpin)));
    viewProfilePage->setProperty("targetBGEditPtr", QVariant::fromValue(static_cast<void*>(targetBGSpin)));
}

void MergedMainWindow::setupBolusInputPage()
{
    if (!bolusInputPage) {
        bolusInputPage = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(bolusInputPage);
        QFormLayout* formLayout = new QFormLayout();
        QLabel* bgLabel = new QLabel("Blood Glucose (mg/dL):", bolusInputPage);
        QLineEdit* bgLineEdit = new QLineEdit(bolusInputPage);
        bgLineEdit->setText("180");
        bgLineEdit->setReadOnly(true);
        formLayout->addRow(bgLabel, bgLineEdit);
        QLabel* carbLabel = new QLabel("Carbohydrate Intake (g):", bolusInputPage);
        QLineEdit* carbLineEdit = new QLineEdit(bolusInputPage);
        formLayout->addRow(carbLabel, carbLineEdit);
        mainLayout->addLayout(formLayout);
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* manualBGButton = new QPushButton("Manual BG", bolusInputPage);
        buttonLayout->addWidget(manualBGButton);
        QPushButton* calculateButton = new QPushButton("Calculate Bolus", bolusInputPage);
        buttonLayout->addWidget(calculateButton);
        QPushButton* backButton = new QPushButton("Back", bolusInputPage);
        buttonLayout->addWidget(backButton);
        mainLayout->addLayout(buttonLayout);
        connect(manualBGButton, &QPushButton::clicked, [=]() {
            bgLineEdit->setReadOnly(false);
        });
        connect(calculateButton, &QPushButton::clicked, [=]() {
            double bg = bgLineEdit->text().toDouble();
            double carbs = carbLineEdit->text().toDouble();
            qDebug() << "Calculate Bolus requested with BG:" << bg << "and Carbs:" << carbs;
            Profile* activeProfile = profileManager->getActiveProfile();
            if (!activeProfile) {
                QMessageBox::warning(bolusInputPage, "Bolus Calculation Error", "No active profile is set. Please set an active profile.");
                dataLogger->logEvent("BolusCalc", "Bolus calculation attempted with no active profile.");
                return;
            }
            double recommendedDose = bolusManager->computeRecommendedDose(bg, carbs);
            qDebug() << "Calculated recommended bolus:" << recommendedDose << "units.";
            dataLogger->logEvent("BolusCalc", "Recommended bolus: " + std::to_string(recommendedDose));
            showBolusConfirmationPage(recommendedDose);
        });
        connect(backButton, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);
        bolusInputPage->setLayout(mainLayout);
        stackedWidget->addWidget(bolusInputPage);
    }
}

void MergedMainWindow::showBolusConfirmationPage(double dose)
{
    QWidget* confirmationPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(confirmationPage);
    QFormLayout* formLayout = new QFormLayout();
    QLabel* doseLabel = new QLabel("Recommended Dose (units):", confirmationPage);
    QLineEdit* doseEdit = new QLineEdit(confirmationPage);
    doseEdit->setText(QString::number(dose));
    doseEdit->setReadOnly(true);
    formLayout->addRow(doseLabel, doseEdit);
    layout->addLayout(formLayout);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* overrideButton = new QPushButton("Override", confirmationPage);
    QPushButton* immediateButton = new QPushButton("Immediate Release", confirmationPage);
    QPushButton* extendedButton = new QPushButton("Extended Release", confirmationPage);
    QPushButton* cancelButton = new QPushButton("Cancel", confirmationPage);
    buttonLayout->addWidget(overrideButton);
    buttonLayout->addWidget(immediateButton);
    buttonLayout->addWidget(extendedButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    connect(overrideButton, &QPushButton::clicked, [doseEdit]() {
        doseEdit->setReadOnly(false);
    });
    connect(immediateButton, &QPushButton::clicked, [=]() {
        double finalDose = doseEdit->text().toDouble();
        int ret = QMessageBox::information(confirmationPage, "Confirm Immediate Bolus",
            QString("Deliver immediate bolus of %1 units?").arg(finalDose),
            QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok) {
            bolusManager->deliverBolus(finalDose, false, 0.0);
            dataLogger->logEvent("BolusDelivery", "Immediate bolus delivered: " + std::to_string(finalDose));
            stackedWidget->setCurrentWidget(homePage);
        }
    });
    connect(extendedButton, &QPushButton::clicked, [=]() {
        double finalDose = doseEdit->text().toDouble();
        qDebug() << "Extended Release requested with dose:" << finalDose << "units.";
        showExtendedBolusConfigPage(finalDose);
    });
    connect(cancelButton, &QPushButton::clicked, [=]() {
        dataLogger->logEvent("BolusCancel", "User canceled bolus confirmation");
        stackedWidget->setCurrentWidget(bolusInputPage);
    });
    confirmationPage->setLayout(layout);
    stackedWidget->addWidget(confirmationPage);
    stackedWidget->setCurrentWidget(confirmationPage);
}

void MergedMainWindow::showExtendedBolusConfigPage(double dose)
{
    QWidget* extPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(extPage);
    QLabel* totalDoseLabel = new QLabel(QString("Total Dose: %1 units").arg(dose), extPage);
    mainLayout->addWidget(totalDoseLabel);
    QFormLayout* formLayout = new QFormLayout();
    QLineEdit* immediateEdit = new QLineEdit(extPage);
    immediateEdit->setPlaceholderText("Enter immediate amount");
    formLayout->addRow("Immediate Amount (units):", immediateEdit);
    QLineEdit* splitsEdit = new QLineEdit(extPage);
    splitsEdit->setPlaceholderText("Enter number of splits");
    formLayout->addRow("Number of Splits:", splitsEdit);
    QLineEdit* durationEdit = new QLineEdit(extPage);
    durationEdit->setPlaceholderText("Enter extended duration (min)");
    formLayout->addRow("Extended Duration (min):", durationEdit);
    mainLayout->addLayout(formLayout);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* confirmButton = new QPushButton("Confirm", extPage);
    QPushButton* backButton = new QPushButton("Back", extPage);
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(backButton);
    mainLayout->addLayout(buttonLayout);
    connect(confirmButton, &QPushButton::clicked, [=]() {
        bool ok1, ok2, ok3;
        double immediateAmount = immediateEdit->text().toDouble(&ok1);
        int splits = splitsEdit->text().toInt(&ok2);
        double duration = durationEdit->text().toDouble(&ok3);
        if (!ok1 || !ok2 || !ok3) {
            QMessageBox::warning(extPage, "Input Error", "Please enter valid numerical values.");
            return;
        }
        if (immediateAmount > dose) {
            QMessageBox::warning(extPage, "Input Error", "Immediate amount cannot exceed total dose.");
            return;
        }
        double remaining = dose - immediateAmount;
        double splitDose = (splits > 0) ? remaining / splits : 0.0;
        QString summary = QString("Immediate Amount: %1 units\nRemaining Extended Dose: %2 units\nNumber of Splits: %3\nDose per Split: %4 units\nTotal Extended Duration: %5 minutes")
                              .arg(immediateAmount)
                              .arg(remaining)
                              .arg(splits)
                              .arg(splitDose)
                              .arg(duration);
        QMessageBox::information(extPage, "Extended Bolus Summary", summary);
        bolusManager->deliverBolus(dose, true, immediateAmount, duration, splits);
        dataLogger->logEvent("BolusDelivery", "Extended bolus delivered: " + std::to_string(dose));
        stackedWidget->setCurrentWidget(homePage);
    });
    connect(backButton, &QPushButton::clicked, [=]() {
        stackedWidget->setCurrentWidget(bolusInputPage);
    });
    extPage->setLayout(mainLayout);
    stackedWidget->addWidget(extPage);
    stackedWidget->setCurrentWidget(extPage);
}

void MergedMainWindow::setupBasalControlPage()
{
    basalControlPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(basalControlPage);
    QFormLayout* formLayout = new QFormLayout();
    QDoubleSpinBox* basalRateSpin = new QDoubleSpinBox(basalControlPage);
    basalRateSpin->setRange(0.1, 10.0);
    basalRateSpin->setDecimals(2);
    basalRateSpin->setSingleStep(0.1);
    basalRateSpin->setValue(1.0);
    formLayout->addRow("Set Basal Rate (U/hr):", basalRateSpin);
    layout->addLayout(formLayout);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* startBasalBtn = new QPushButton("Start Basal", basalControlPage);
    QPushButton* backBtn = new QPushButton("Back", basalControlPage);
    btnLayout->addWidget(startBasalBtn);
    btnLayout->addWidget(backBtn);
    layout->addLayout(btnLayout);
    connect(startBasalBtn, &QPushButton::clicked, [=]() {
        double rate = basalRateSpin->value();
        if (insulinDeliveryMgr) {
            insulinDeliveryMgr->startBasalDelivery(rate);
            QMessageBox::information(basalControlPage, "Basal Control", QString("Basal started at %1 U/hr.").arg(rate));
            dataLogger->logEvent("BasalStart", "Basal started at " + std::to_string(rate) + " U/hr");
        }
    });
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);
    basalControlPage->setLayout(layout);
    stackedWidget->addWidget(basalControlPage);
}

void MergedMainWindow::setupHistoryPage()
{
    historyPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(historyPage);
    historyList = new QListWidget(historyPage);
    layout->addWidget(historyList);
    QPushButton* backBtn = new QPushButton("Back", historyPage);
    layout->addWidget(backBtn);
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);
    historyPage->setLayout(layout);
    stackedWidget->addWidget(historyPage);
    updateHistoryList();
}

void MergedMainWindow::updateHistoryList()
{
    if (!historyList)
        return;
    historyList->clear();
    auto events = dataLogger->getEvents();
    if (events.empty()) {
        historyList->addItem("No History Available");
    } else {
        for (const auto &e : events) {
            historyList->addItem(QString::fromStdString(e));
        }
    }
}

void MergedMainWindow::showHomePage()
{
    stackedWidget->setCurrentWidget(homePage);
}

void MergedMainWindow::showOptionsPage()
{
    stackedWidget->setCurrentWidget(optionsPage);
}

void MergedMainWindow::showPersonalProfilesPage()
{
    refreshProfilesList();
    stackedWidget->setCurrentWidget(profilesPage);
}

void MergedMainWindow::showAddProfilePage()
{
    stackedWidget->setCurrentWidget(addProfilePage);
}

void MergedMainWindow::showViewProfilePage(Profile* profile)
{
    if (!profile)
        return;
    currentProfile = profile;
    QLineEdit* nameEdit = static_cast<QLineEdit*>(viewProfilePage->property("nameEditPtr").value<void*>());
    QDoubleSpinBox* icrSpin = static_cast<QDoubleSpinBox*>(viewProfilePage->property("icrEditPtr").value<void*>());
    QDoubleSpinBox* corrSpin = static_cast<QDoubleSpinBox*>(viewProfilePage->property("corrEditPtr").value<void*>());
    QDoubleSpinBox* targetBGSpin = static_cast<QDoubleSpinBox*>(viewProfilePage->property("targetBGEditPtr").value<void*>());
    if (nameEdit && icrSpin && corrSpin && targetBGSpin) {
        nameEdit->setText(QString::fromStdString(profile->getName()));
        icrSpin->setValue(profile->getInsulinToCarbRatio());
        corrSpin->setValue(profile->getCorrectionFactor());
        targetBGSpin->setValue(profile->getTargetBG());
        nameEdit->setReadOnly(true);
        icrSpin->setReadOnly(true);
        corrSpin->setReadOnly(true);
        targetBGSpin->setReadOnly(true);
        Profile* active = profileManager->getActiveProfile();
        if (active && active->getName() == profile->getName())
            nameEdit->setStyleSheet("background-color: lightgreen;");
        else
            nameEdit->setStyleSheet("");
    }
    stackedWidget->setCurrentWidget(viewProfilePage);
}

void MergedMainWindow::showBolusPage()
{
    setupBolusInputPage();
    stackedWidget->setCurrentWidget(bolusInputPage);
}

void MergedMainWindow::showBasalControlPage()
{
    if (!basalControlPage)
        setupBasalControlPage();
    stackedWidget->setCurrentWidget(basalControlPage);
}

void MergedMainWindow::showHistoryPage()
{
    if (!historyPage)
        setupHistoryPage();
    else
        updateHistoryList();
    stackedWidget->setCurrentWidget(historyPage);
}

void MergedMainWindow::onSimulationTick()
{
    simulationTime = simulationTime.addSecs(60);
    simTimeLabel->setText(simulationTime.toString("hh:mm"));

    // NEW: Update the backend simulation time
    if (controlIQ)
        controlIQ->getSimulator()->setGUISimTime(tickCount);

    if (insulinDeliveryMgr)
        insulinDeliveryMgr->onTick(1.0);

    tickCount++;

    if (tickCount % 5 == 0 && controlIQ && cgmInterface) {
        double currentBG = cgmInterface->getCurrentBG();
        controlIQ->processSensorReading(currentBG);
        controlIQ->predictBGTrend();
        controlIQ->applyAutomaticAdjustments();
    }
}

void MergedMainWindow::refreshProfilesList()
{
    if (!profileList)
        return;
    profileList->clear();
    auto profiles = profileManager->getAllProfiles();
    Profile* activeProfile = profileManager->getActiveProfile();
    for (const auto* profile : profiles) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(profile->getName()));
        if (activeProfile && activeProfile->getName() == profile->getName()) {
            QPixmap pixmap(10, 10);
            pixmap.fill(Qt::green);
            item->setIcon(QIcon(pixmap));
        }
        profileList->addItem(item);
    }
}
