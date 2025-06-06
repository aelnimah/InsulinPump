#include "MergedMainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStackedWidget>
#include <QLineEdit>
#include "DataLogger.h"
#include "ProfileManager.h"
#include "ProfileCRUDController.h"
#include "Profile.h"
#include "PumpSimulator.h"
#include "BolusManager.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "Battery.h"
#include "Cartridge.h"
#include "ControlIQController.h"
#include "AlertManager.h"

#include <QListWidget>
#include <QFormLayout>
#include <QDoubleSpinBox>

#include <QtCharts/QValueAxis>
QT_CHARTS_USE_NAMESPACE



MergedMainWindow::MergedMainWindow(PumpSimulator* simulator, ProfileManager* mgr, QWidget* parent)
    : QMainWindow(parent), 
      pumpSimulator(simulator), 
      profileManager(mgr)
{
    // Init logging and CRUD support
    dataLogger = new DataLogger();
    crudController = new ProfileCRUDController(profileManager);

    // Extract components from simulator (used across pages)
    if (pumpSimulator) {
        insulinDeliveryMgr = pumpSimulator->getInsulinDeliveryManager();
        cgmInterface        = pumpSimulator->getCGMSensorInterface();
        controlIQ           = pumpSimulator->getControlIQController();
        alertManager        = pumpSimulator->getAlertManager();
        battery = pumpSimulator->getBattery();
        cartridge = pumpSimulator->getCartridge();

        BolusCalculator* calc = pumpSimulator->getBolusCalculator();
        bolusManager = new BolusManager(profileManager, calc, insulinDeliveryMgr, cgmInterface);

        cgmInterface->setDeliveryManager(insulinDeliveryMgr);
        
    }

    // Init GUI simulation clock
    simulationTime = QTime(0, 0, 0);
    simTimeLabel = new QLabel(simulationTime.toString("hh:mm"), this);
    simTimeLabel->setAlignment(Qt::AlignCenter);
    simTimeLabel->setStyleSheet("font-weight: bold; font-size: 18px;");

    // Page navigation stack
    stackedWidget = new QStackedWidget(this);
    setupHomePage();              stackedWidget->addWidget(homePage);
    setupOptionsPage();           stackedWidget->addWidget(optionsPage);
    setupProfilesPage();          stackedWidget->addWidget(profilesPage);
    setupViewProfilePage();       stackedWidget->addWidget(viewProfilePage);
    setupAddProfilePage();        stackedWidget->addWidget(addProfilePage);
    setupBolusInputPage();        stackedWidget->addWidget(bolusInputPage);
    stackedWidget->setCurrentWidget(homePage);

    // Layout wrapper
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addWidget(simTimeLabel);
    layout->addWidget(stackedWidget);
    container->setLayout(layout);
    setCentralWidget(container);

    // Basic window properties
    setWindowTitle("Insulin Pump Simulator");
    resize(480, 320);

    // Timer for simulation tick
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MergedMainWindow::onSimulationTick);
    simulationTimer->start(2000);  // 1 simulated minute = 2 real seconds

    // Start backend simulation
    if (pumpSimulator) {
        pumpSimulator->startSimulation();
    }
}


MergedMainWindow::~MergedMainWindow() 
{
    delete dataLogger;
    delete crudController;
    delete bolusManager;
    delete battery;
    delete cartridge;

    // Important: Do NOT delete simulator or profileManager here – they are passed in and deleted externally.
}


void MergedMainWindow::onSimulationTick()
{
    // Advance simulation time by 1 simulated minute (every 2 real seconds)
    simulationTime = simulationTime.addSecs(60);
    simTimeLabel->setText(simulationTime.toString("hh:mm"));

    // Update backend with GUI time
    static int tickCount = 0;
    ++tickCount;

    pumpSimulator->setGUISimTime(tickCount);
    pumpSimulator->updateSimulationState();

    // Refresh labels
    if (iobLabel)
        iobLabel->setText("IOB: " + QString::number(insulinDeliveryMgr->getInsulinOnBoard(), 'f', 2) + " U");

    if (bgLabel)
        bgLabel->setText("BG: " + QString::number(cgmInterface->getCurrentBG(), 'f', 1) + " mmol/L");

    if (batteryLabel && pumpSimulator->getBattery())
        batteryLabel->setText("Battery: " + QString::number(pumpSimulator->getBattery()->getLevel()) + "%");

    if (cartridgeLabel && pumpSimulator->getCartridge())
        cartridgeLabel->setText("Cartridge: " + QString::number(pumpSimulator->getCartridge()->getCurrentVolume()) + " IU");

    if (bgSeries && pumpSimulator) {
        int time = pumpSimulator->getCurrentSimTime();
        double bg = cgmInterface->getCurrentBG();
        bgSeries->append(time, bg);
    
        QChart* chart = chartView->chart();
        QValueAxis* axisX = static_cast<QValueAxis*>(chart->axisX());
        if (axisX && time > 30)
            axisX->setRange(time - 30, time); // show last 30 minutes
    }             
}

void MergedMainWindow::setupHomePage()
{
    homePage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(homePage);

    // Top layout split into left and right sections
    QHBoxLayout* topLayout = new QHBoxLayout();

    // --- LEFT (IOB & BG)
    QVBoxLayout* leftTopLayout = new QVBoxLayout();
    iobLabel = new QLabel("IOB: " + QString::number(insulinDeliveryMgr->getInsulinOnBoard(), 'f', 2) + " U", homePage);
    bgLabel = new QLabel("BG: " + QString::number(cgmInterface->getCurrentBG(), 'f', 2) + " mmol/L", homePage);

    leftTopLayout->addWidget(iobLabel);
    leftTopLayout->addWidget(bgLabel);

    // --- RIGHT (Battery & Cartridge)
    QVBoxLayout* rightTopLayout = new QVBoxLayout();
    batteryLabel = new QLabel("Battery: " + QString::number(pumpSimulator->getBattery()->getLevel()) + "%", homePage);
    cartridgeLabel = new QLabel("Cartridge: " + QString::number(pumpSimulator->getCartridge()->getCurrentVolume(), 'f', 1) + " IU", homePage);
    rightTopLayout->addWidget(batteryLabel);
    rightTopLayout->addWidget(cartridgeLabel);
    rightTopLayout->setAlignment(Qt::AlignRight);
 
    topLayout->addLayout(leftTopLayout);
    topLayout->addStretch();
    topLayout->addLayout(rightTopLayout);

    mainLayout->addLayout(topLayout);

    // Buttons
    QPushButton* optionsBtn = new QPushButton("Options", homePage);
    QPushButton* bolusBtn = new QPushButton("Bolus", homePage);
    QPushButton* bgGraphBtn = new QPushButton("View BG Graph", homePage);
    QPushButton* historyBtn = new QPushButton("History", homePage);

    mainLayout->addWidget(optionsBtn);
    mainLayout->addWidget(bolusBtn);
    mainLayout->addWidget(bgGraphBtn);
    mainLayout->addWidget(historyBtn);

    connect(optionsBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);
    connect(bolusBtn, &QPushButton::clicked, this, &MergedMainWindow::showBolusPage);
    connect(bgGraphBtn, &QPushButton::clicked, this, &MergedMainWindow::showBGGraphPage);
    connect(historyBtn, &QPushButton::clicked, this, &MergedMainWindow::showHistoryPage);

    homePage->setLayout(mainLayout);
}

void MergedMainWindow::setupPumpPage() {
    pumpPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(pumpPage);

    // --- Status Labels ---
    batteryStatusLabel = new QLabel("Battery: --%", pumpPage);
    cartridgeStatusLabel = new QLabel("Cartridge: -- IU", pumpPage);
    layout->addWidget(batteryStatusLabel);
    layout->addWidget(cartridgeStatusLabel);

    // --- Action Buttons ---
    QPushButton* chargeBtn = new QPushButton("Charge Battery", pumpPage);
    QPushButton* refillBtn = new QPushButton("Refill Insulin", pumpPage);
    QPushButton* backBtn = new QPushButton("Back", pumpPage);

    layout->addWidget(chargeBtn);
    layout->addWidget(refillBtn);
    layout->addWidget(backBtn);

    connect(chargeBtn, &QPushButton::clicked, [=]() {
        Battery* battery = pumpSimulator->getBattery();
        if (battery) battery->setLevel(100);
        updatePumpStatusLabels();
    });

    connect(refillBtn, &QPushButton::clicked, [=]() {
        Cartridge* cart = pumpSimulator->getCartridge();
        if (cart) cart->refill();  // Assumes you have a refill() method
        updatePumpStatusLabels();
    });

    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);

    pumpPage->setLayout(layout);
    stackedWidget->addWidget(pumpPage);
}

void MergedMainWindow::updatePumpStatusLabels() {
    if (batteryStatusLabel && pumpSimulator->getBattery()) {
        batteryStatusLabel->setText("Battery: " +
            QString::number(pumpSimulator->getBattery()->getLevel()) + "%");
    }

    if (cartridgeStatusLabel && pumpSimulator->getCartridge()) {
        cartridgeStatusLabel->setText("Cartridge: " +
            QString::number(pumpSimulator->getCartridge()->getCurrentVolume(), 'f', 1) + " IU");
    }
}

void MergedMainWindow::setupHistoryPage()
{
    historyPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(historyPage);

    historyList = new QListWidget(historyPage);
    layout->addWidget(historyList);

    QPushButton* backBtn = new QPushButton("Back", historyPage);
    layout->addWidget(backBtn);
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);

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
        for (const auto& e : events)
            historyList->addItem(QString::fromStdString(e));
    }
}

void MergedMainWindow::setupOptionsPage()
{
    optionsPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(optionsPage);

    QPushButton* profilesButton = new QPushButton("Personal Profiles", optionsPage);
    layout->addWidget(profilesButton);
    connect(profilesButton, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);

    QPushButton* basalBtn = new QPushButton("Basal Control", optionsPage);
    layout->addWidget(basalBtn);
    connect(basalBtn, &QPushButton::clicked, this, &MergedMainWindow::showBasalControlPage);

    QPushButton* homeButton = new QPushButton("Home", optionsPage);
    layout->addWidget(homeButton);
    connect(homeButton, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);

    QPushButton* pumpButton = new QPushButton("Pump", optionsPage);
    layout->addWidget(pumpButton);
    connect(pumpButton, &QPushButton::clicked, this, &MergedMainWindow::showPumpPage);

    optionsPage->setLayout(layout);
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

    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);

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
    QDoubleSpinBox* corrSpin = new QDoubleSpinBox(addProfilePage);
    QDoubleSpinBox* targetBGSpin = new QDoubleSpinBox(addProfilePage);

    icrSpin->setRange(0.1, 100.0);
    corrSpin->setRange(0.1, 100.0);
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

    connect(saveBtn, &QPushButton::clicked, [=]() {
        if (nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Profile name is required.");
            return;
        }

        crudController->createProfile(
            nameEdit->text().toStdString(),
            icrSpin->value(),
            corrSpin->value(),
            targetBGSpin->value()
        );

        dataLogger->logEvent("ProfileCreate", "Created profile " + nameEdit->text().toStdString());
        refreshProfilesList();
        showPersonalProfilesPage();
    });

    connect(cancelBtn, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);

    addProfilePage->setLayout(layout);
    stackedWidget->addWidget(addProfilePage);
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
        if (!currentProfile) return;

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

// Bolus
void MergedMainWindow::setupBolusInputPage()
{
    if (bolusInputPage) return;

    bolusInputPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(bolusInputPage);

    // --- Form inputs ---
    QFormLayout* formLayout = new QFormLayout();

    bgLineEdit = new QLineEdit(bolusInputPage);  // Don't set text yet
    bgLineEdit->setReadOnly(true);

    QLineEdit* carbLineEdit = new QLineEdit(bolusInputPage);

    formLayout->addRow("Blood Glucose (mmol/L):", bgLineEdit);
    formLayout->addRow("Carbohydrate Intake (g):", carbLineEdit);
    mainLayout->addLayout(formLayout);

    // --- Buttons ---
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* manualBGButton = new QPushButton("Manual BG", bolusInputPage);
    QPushButton* calculateButton = new QPushButton("Calculate Bolus", bolusInputPage);
    QPushButton* backButton = new QPushButton("Back", bolusInputPage);

    buttonLayout->addWidget(manualBGButton);
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(backButton);
    mainLayout->addLayout(buttonLayout);

    // --- Connections ---
    connect(manualBGButton, &QPushButton::clicked, [=]() {
        bgLineEdit->setReadOnly(false);
        double currentBG = cgmInterface->getCurrentBG();
        bgLineEdit->setText(QString::number(currentBG));
    });

    connect(calculateButton, &QPushButton::clicked, [=]() {
        double bg = bgLineEdit->text().toDouble();
        double carbs = carbLineEdit->text().toDouble();

        qDebug() << "[Bolus Input] BG:" << bg << "Carbs:" << carbs;

        Profile* activeProfile = profileManager->getActiveProfile();
        if (!activeProfile) {
            QMessageBox::warning(bolusInputPage, "No Active Profile", "Please set an active profile before calculating bolus.");
            dataLogger->logEvent("BolusCalc", "Attempted without active profile.");
            return;
        }

        if (!bolusManager) {
            qDebug() << "[ERROR] bolusManager is null!";
            return;
        }

        double recommendedDose = bolusManager->computeRecommendedDose(bg, carbs);
        qDebug() << "[Bolus Input] Recommended dose:" << recommendedDose << "units";
        dataLogger->logEvent("BolusCalc", "Recommended bolus: " + std::to_string(recommendedDose));

        cgmInterface->addCarbs(carbs);
        showBolusConfirmationPage(recommendedDose);
    });

    connect(backButton, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);

    bolusInputPage->setLayout(mainLayout);
    stackedWidget->addWidget(bolusInputPage);
}

void MergedMainWindow::showBolusConfirmationPage(double dose)
{
    if (!bolusConfirmationPage) {
        bolusConfirmationPage = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(bolusConfirmationPage);

        QFormLayout* formLayout = new QFormLayout();
        QLabel* doseLabel = new QLabel("Recommended Dose (units):", bolusConfirmationPage);
        bolusDoseEdit = new QLineEdit(bolusConfirmationPage);
        bolusDoseEdit->setReadOnly(true);
        formLayout->addRow(doseLabel, bolusDoseEdit);
        layout->addLayout(formLayout);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* overrideButton = new QPushButton("Override", bolusConfirmationPage);
        QPushButton* immediateButton = new QPushButton("Immediate Release", bolusConfirmationPage);
        QPushButton* extendedButton = new QPushButton("Extended Release", bolusConfirmationPage);
        QPushButton* cancelButton = new QPushButton("Cancel", bolusConfirmationPage);
        buttonLayout->addWidget(overrideButton);
        buttonLayout->addWidget(immediateButton);
        buttonLayout->addWidget(extendedButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);

        connect(overrideButton, &QPushButton::clicked, [=]() {
            bolusDoseEdit->setReadOnly(false);
        });

        connect(immediateButton, &QPushButton::clicked, [=]() {
            double finalDose = bolusDoseEdit->text().toDouble();
            int ret = QMessageBox::information(bolusConfirmationPage, "Confirm Immediate Bolus",
                QString("Deliver immediate bolus of %1 units?").arg(finalDose),
                QMessageBox::Ok | QMessageBox::Cancel);
            if (ret == QMessageBox::Ok) {
                bolusManager->deliverBolus(finalDose, false, 0.0);
                dataLogger->logEvent("BolusDelivery", "Immediate bolus delivered: " + std::to_string(finalDose));
                stackedWidget->setCurrentWidget(homePage);
            }
        });

        connect(extendedButton, &QPushButton::clicked, [=]() {
            double finalDose = bolusDoseEdit->text().toDouble();
            qDebug() << "[Bolus] Extended Release requested:" << finalDose << "units";
            showExtendedBolusConfigPage(finalDose);
        });

        connect(cancelButton, &QPushButton::clicked, [=]() {
            dataLogger->logEvent("BolusCancel", "User canceled bolus confirmation");
            stackedWidget->setCurrentWidget(bolusInputPage);
        });

        bolusConfirmationPage->setLayout(layout);
        stackedWidget->addWidget(bolusConfirmationPage);
    }

    bolusDoseEdit->setText(QString::number(dose));
    bolusDoseEdit->setReadOnly(true);
    stackedWidget->setCurrentWidget(bolusConfirmationPage);
}

void MergedMainWindow::showExtendedBolusConfigPage(double dose)
{
    if (!extendedBolusPage) {
        extendedBolusPage = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(extendedBolusPage);

        QLabel* totalDoseLabel = new QLabel(extendedBolusPage);
        totalDoseLabel->setObjectName("totalDoseLabel");  // So we can update it later
        mainLayout->addWidget(totalDoseLabel);

        QFormLayout* formLayout = new QFormLayout();
        QLineEdit* immediateEdit = new QLineEdit(extendedBolusPage);
        QLineEdit* splitsEdit = new QLineEdit(extendedBolusPage);
        QLineEdit* durationEdit = new QLineEdit(extendedBolusPage);

        immediateEdit->setPlaceholderText("Immediate (units)");
        splitsEdit->setPlaceholderText("Splits");
        durationEdit->setPlaceholderText("Duration (min)");

        formLayout->addRow("Immediate Amount:", immediateEdit);
        formLayout->addRow("Number of Splits:", splitsEdit);
        formLayout->addRow("Extended Duration:", durationEdit);

        mainLayout->addLayout(formLayout);

        QHBoxLayout* btnLayout = new QHBoxLayout();
        QPushButton* confirmBtn = new QPushButton("Confirm", extendedBolusPage);
        QPushButton* backBtn = new QPushButton("Back", extendedBolusPage);
        btnLayout->addWidget(confirmBtn);
        btnLayout->addWidget(backBtn);
        mainLayout->addLayout(btnLayout);

        connect(confirmBtn, &QPushButton::clicked, [=]() {
            bool ok1, ok2, ok3;
            double immediate = immediateEdit->text().toDouble(&ok1);
            int splits = splitsEdit->text().toInt(&ok2);
            double duration = durationEdit->text().toDouble(&ok3);

            if (!ok1 || !ok2 || !ok3 || immediate > dose) {
                QMessageBox::warning(extendedBolusPage, "Input Error", "Please enter valid values.");
                return;
            }

            double remaining = dose - immediate;
            double perSplit = (splits > 0) ? remaining / splits : 0.0;
            QString summary = QString("Immediate: %1U\nExtended: %2U\nSplits: %3\nPer Split: %4U\nDuration: %5min")
                                .arg(immediate).arg(remaining).arg(splits).arg(perSplit).arg(duration);

            QMessageBox::information(extendedBolusPage, "Extended Bolus", summary);
            bolusManager->deliverBolus(dose, true, immediate, duration, splits, pumpSimulator->getCurrentSimTime());
            dataLogger->logEvent("BolusDelivery", "Extended bolus delivered: " + std::to_string(dose));
            stackedWidget->setCurrentWidget(homePage);
        });

        connect(backBtn, &QPushButton::clicked, [=]() {
            stackedWidget->setCurrentWidget(bolusConfirmationPage);
        });

        extendedBolusPage->setLayout(mainLayout);
        stackedWidget->addWidget(extendedBolusPage);
    }

    QLabel* doseLabel = extendedBolusPage->findChild<QLabel*>("totalDoseLabel");
    if (doseLabel)
        doseLabel->setText(QString("Total Dose: %1 units").arg(dose));

    stackedWidget->setCurrentWidget(extendedBolusPage);
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
            showHomePage();  // optional: auto return
        }
    });    

    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);

    basalControlPage->setLayout(layout);
    stackedWidget->addWidget(basalControlPage);
}

void MergedMainWindow::setupBGGraphPage() {
    bgGraphPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(bgGraphPage);

    bgSeries = new QLineSeries();
    bgSeries->setName("BG Level");

    QChart* chart = new QChart();
    chart->addSeries(bgSeries);
    chart->setTitle("Blood Glucose Over Time");

    QValueAxis* axisX = new QValueAxis();
    axisX->setTitleText("Time (min)");
    axisX->setLabelFormat("%d");
    axisX->setRange(0, 60); 

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("BG (mmol/L)");
    axisY->setLabelFormat("%.1f");
    axisY->setRange(3.0, 10.0); 

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    bgSeries->attachAxis(axisX);
    bgSeries->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    QPushButton* backBtn = new QPushButton("Back", bgGraphPage);
    layout->addWidget(backBtn);
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);

    bgGraphPage->setLayout(layout);
    stackedWidget->addWidget(bgGraphPage);
}

//--------------
void MergedMainWindow::showHomePage() { stackedWidget->setCurrentWidget(homePage); }
void MergedMainWindow::showPumpPage() {
    if (!pumpPage) setupPumpPage();
    updatePumpStatusLabels();
    stackedWidget->setCurrentWidget(pumpPage);
}
void MergedMainWindow::showHistoryPage()
{
    if (!historyPage)
        setupHistoryPage();
    else
        updateHistoryList();

    stackedWidget->setCurrentWidget(historyPage);
}
void MergedMainWindow::showOptionsPage() { stackedWidget->setCurrentWidget(optionsPage); }
void MergedMainWindow::showPersonalProfilesPage()
{
    refreshProfilesList();
    stackedWidget->setCurrentWidget(profilesPage);
}
void MergedMainWindow::showAddProfilePage()
{
    if (!addProfilePage)
        setupAddProfilePage();

    stackedWidget->setCurrentWidget(addProfilePage);
}
void MergedMainWindow::showViewProfilePage(Profile* profile)
{
    if (!profile) return;
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

        Profile* active = profileManager->getActiveProfile();
        if (active && active->getName() == profile->getName()) {
            nameEdit->setStyleSheet("background-color: lightgreen;");
        } else {
            nameEdit->setStyleSheet("");
        }
    }

    stackedWidget->setCurrentWidget(viewProfilePage);
}
void MergedMainWindow::showBolusPage() {
    setupBolusInputPage();  // One-time setup
    if (bgLineEdit && cgmInterface)
        bgLineEdit->setText(QString::number(cgmInterface->getCurrentBG()));
    stackedWidget->setCurrentWidget(bolusInputPage);
}

void MergedMainWindow::showBasalControlPage() {
    if (!basalControlPage)
        setupBasalControlPage();
    stackedWidget->setCurrentWidget(basalControlPage);
}

void MergedMainWindow::showBGGraphPage() {
    if (!bgGraphPage)
        setupBGGraphPage();
    stackedWidget->setCurrentWidget(bgGraphPage);
}
