#include "MainWindow.h"
#include "OptionsWindow.h"
#include "PersonalProfilesWindow.h"
#include "AddProfileWindow.h"
#include "ViewProfileWindow.h"
#include "BolusInputPage.h"
#include "ProfileManager.h"
#include "ProfileCRUDController.h"
#include "BolusManager.h"
#include "Profile.h"

// Bolus dependencies
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTimer>
#include <QLabel>
#include <QTime>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(ProfileManager* mgr, QWidget* parent)
    : QMainWindow(parent), profileManager(mgr)
{
    crudController = new ProfileCRUDController(profileManager);
    stackedWidget = new QStackedWidget(this);

    // Create pages
    setupHomePage();
    optionsPage = new OptionsWindow(profileManager);
    profilesPage = new PersonalProfilesWindow(profileManager, crudController);
    addProfilePage = new AddProfileWindow(crudController);
    viewProfilePage = new ViewProfileWindow(nullptr, crudController, profileManager, this);

    // Add pages to the stacked widget
    stackedWidget->addWidget(homePage);        // index 0
    stackedWidget->addWidget(optionsPage);       // index 1
    stackedWidget->addWidget(profilesPage);      // index 2
    stackedWidget->addWidget(addProfilePage);    // index 3
    stackedWidget->addWidget(viewProfilePage);   // index 4

    // --- Setup Simulation Clock ---
    simulationTime = QTime(0, 0, 0);
    simTimeLabel = new QLabel(simulationTime.toString("hh:mm"), this);
    simTimeLabel->setAlignment(Qt::AlignCenter);
    simTimeLabel->setStyleSheet("font-weight: bold; font-size: 18px;");

    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::updateSimulationClock);
    simulationTimer->start(2000);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(simTimeLabel);
    mainLayout->addWidget(stackedWidget);
    QWidget* container = new QWidget(this);
    container->setLayout(mainLayout);
    setCentralWidget(container);

    setWindowTitle("Insulin Pump Simulator");
    resize(480, 320);

    // --- Navigation Connections ---
    connect(optionsPage, &OptionsWindow::personalProfilesClicked, this, &MainWindow::showProfilesPage);
    connect(optionsPage, &OptionsWindow::homeClicked, this, &MainWindow::showHomePage);
    connect(profilesPage, &PersonalProfilesWindow::goToAddProfile, this, &MainWindow::showAddProfilePage);
    connect(profilesPage, &PersonalProfilesWindow::backToOptions, this, &MainWindow::showOptionsPage);
    connect(profilesPage, &PersonalProfilesWindow::viewProfile, this, &MainWindow::showViewProfilePage);

    connect(addProfilePage, &AddProfileWindow::profileAdded, this, [=]() {
        profilesPage->refreshProfileList();
        stackedWidget->setCurrentWidget(profilesPage);
    });

    connect(addProfilePage, &AddProfileWindow::cancelled, this, [=]() {
        stackedWidget->setCurrentWidget(profilesPage);
    });

    connect(viewProfilePage, &ViewProfileWindow::backToProfiles, this, &MainWindow::showProfilesPage);

    // --- Initialize BolusManager and its dependencies ---
    // Create instances for bolus-related components.
    BolusCalculator* bolusCalc = new BolusCalculator();
    InsulinDeliveryManager* insulinDeliveryMgr = new InsulinDeliveryManager();
    CGMSensorInterface* cgmInterface = new CGMSensorInterface();
    // Optionally, you could initialize the cartridge and battery for insulinDeliveryMgr here.
    // For example, if you have a Cartridge class instance, set it:
    // Cartridge* cartridge = new Cartridge();
    // insulinDeliveryMgr->setCartridge(cartridge);

    // Now, create the BolusManager instance.
    bolusManager = new BolusManager(profileManager, bolusCalc, insulinDeliveryMgr, cgmInterface);

    // Note: In a real application, you might want to manage these pointers (delete them in the destructor, or use smart pointers).
}

MainWindow::~MainWindow() {
    // Remember to delete any heap-allocated objects if you are not using smart pointers.
    // For example, you may want to delete bolusManager, bolusCalc, insulinDeliveryMgr, cgmInterface, etc.
    // This example does not show full cleanup.
}

void MainWindow::setupHomePage() {
    homePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(homePage);

    QPushButton* optionsButton = new QPushButton("Options", this);
    layout->addWidget(optionsButton);
    connect(optionsButton, &QPushButton::clicked, this, &MainWindow::showOptionsPage);

    QPushButton* bolusButton = new QPushButton("Bolus", this);
    layout->addWidget(bolusButton);
    connect(bolusButton, &QPushButton::clicked, this, &MainWindow::showBolusPage);
}

void MainWindow::showOptionsPage() {
    stackedWidget->setCurrentWidget(optionsPage);
}

void MainWindow::showBolusPage() {
    // Create a new BolusInputPage.
    BolusInputPage *bolusPage = new BolusInputPage(this);
    // Add it to the stacked widget.
    stackedWidget->addWidget(bolusPage);
    
    // Set the CGM reading (hard-coded for now).
    bolusPage->setCGMReading(180.0);
    
    // Connect the signal to calculate the bolus.
    connect(bolusPage, &BolusInputPage::calculateBolusRequested, this, [=](double bg, double carbs){
        qDebug() << "Calculating bolus with BG:" << bg << "and Carbs:" << carbs;
        double dose = bolusManager->computeRecommendedDose(bg, carbs);
        qDebug() << "Recommended bolus dose:" << dose << "units.";
        // For now, print to console. In future, navigate to a confirmation page.
    });
    
    stackedWidget->setCurrentWidget(bolusPage);
}

void MainWindow::showProfilesPage() {
    profilesPage->refreshProfileList();
    stackedWidget->setCurrentWidget(profilesPage);
}

void MainWindow::showAddProfilePage() {
    stackedWidget->setCurrentWidget(addProfilePage);
}

void MainWindow::showViewProfilePage(Profile* profile) {
    viewProfilePage->setProfile(profile);
    stackedWidget->setCurrentWidget(viewProfilePage);
}

void MainWindow::showHomePage() {
    stackedWidget->setCurrentWidget(homePage);
}

void MainWindow::updateSimulationClock() {
    simulationTime = simulationTime.addSecs(60);
    simTimeLabel->setText(simulationTime.toString("hh:mm"));
}
