#include "PumpTester.h"

#include "PumpSimulator.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "Cartridge.h"
#include "Battery.h"
#include "CGMSensorInterface.h"
#include "ControlIQController.h"
#include "AlertManager.h"

#include <iostream>
#include <iomanip>
#include <string>

PumpTester::PumpTester() {
    simulator = new PumpSimulator();

    // Create all subsystems
    profileManager = new ProfileManager();
    bolusCalculator = new BolusCalculator();
    deliveryManager = new InsulinDeliveryManager();
    cartridge = new Cartridge();
    battery = new Battery();
    cgmSensor = new CGMSensorInterface();
    controlIQ = new ControlIQController();
    alertManager = new AlertManager();

    // Wire delivery dependencies
    deliveryManager->setCartridge(cartridge);
    deliveryManager->setBattery(battery);

    // Wire simulator dependencies
    simulator->setProfileManager(profileManager);
    simulator->setBolusCalculator(bolusCalculator);
    simulator->setInsulinDeliveryManager(deliveryManager);
    simulator->setCartridge(cartridge);
    simulator->setBattery(battery);
    simulator->setCGMSensorInterface(cgmSensor);
    simulator->setControlIQController(controlIQ);
    simulator->setAlertManager(alertManager);

    // Wire ControlIQ dependencies (optional here, but will be needed)
    controlIQ->setCGMSensor(cgmSensor);
    controlIQ->setInsulinDeliveryManager(deliveryManager);

    // Set up default profile
    activeProfile = new Profile();
    activeProfile->setName("Default");
    activeProfile->setInsulinToCarbRatio(10.0);     // 1U per 10g carbs
    activeProfile->setCorrectionFactor(2.0);        // 1U drops BG by 2 mmol/L
    activeProfile->setTargetBG(6.0);                // Target BG is 6.0 mmol/L

    profileManager->createProfile(activeProfile);
    profileManager->setActiveProfile("Default");

    // Start simulation engine
    simulator->startSimulation();
}

PumpTester::~PumpTester() {
    // Disconnect simulator references first
    simulator->setProfileManager(nullptr);
    simulator->setBolusCalculator(nullptr);
    simulator->setInsulinDeliveryManager(nullptr);
    simulator->setCartridge(nullptr);
    simulator->setBattery(nullptr);
    simulator->setCGMSensorInterface(nullptr);
    simulator->setControlIQController(nullptr);
    simulator->setAlertManager(nullptr);

    // Now delete in safe order
    delete simulator;
    delete profileManager;
    delete bolusCalculator;
    delete deliveryManager;
    delete cartridge;
    delete battery;
    delete cgmSensor;
    delete controlIQ;
    delete alertManager;
    delete activeProfile;
}

void PumpTester::runAllTests() {
    simulator->setCLIMode(true);
    testManualBolus();
    // testExtendedBolus();
    // testBasalControl();
    // testControlIQ();
    // testAlerts();
    // testIOBDecayWithExtendedBolus();
}

void PumpTester::testManualBolus() {
    printHeader("Manual Bolus Test");

    double currentBG = simulator->getCurrentBG();
    double iob = simulator->getIOB();
    double carbs = 50.0;

    std::cout << "Current BG: " << currentBG << " mmol/L\n";
    std::cout << "Carb intake: " << carbs << " g\n";
    std::cout << "IOB: " << iob << " U\n";

    double recommendedDose = bolusCalculator->calculateBolus(currentBG, carbs, iob, activeProfile);
    std::cout << "Recommended Dose: " << recommendedDose << " U\n";

    std::cout << "\nDelivering bolus...\n";
    deliveryManager->deliverBolus(recommendedDose, false);

    simulateTime(20);  // Trigger simulation loop for 5 minutes
}

void PumpTester::testExtendedBolus() {
    printHeader("Extended Bolus Test");

    double currentBG = 7.0;
    double carbs = 60.0;
    double iob = 0.0;

    std::cout << "Current BG: " << currentBG << " mmol/L\n";
    std::cout << "Carb intake: " << carbs << " g\n";
    std::cout << "IOB: " << iob << " U\n";

    // Use bolus calculator to get total dose
    double totalDose = bolusCalculator->calculateBolus(currentBG, carbs, iob, activeProfile);
    std::cout << "Total Recommended Dose: " << totalDose << " U\n";

    // Split: 2U immediate, rest extended
    double immediate = 2.0;
    double duration = 4.0;   // minutes
    int splits = 4;          // 1U every 1 min if 4U remaining

    std::cout << "\nDelivering extended bolus...\n";
    deliveryManager->deliverBolus(totalDose, true, immediate, duration, splits, simulator->getSimulatedMinutes());

    simulateTime(5);  // Should deliver the remaining 4U over 4 minutes
}

void PumpTester::testBasalControl() {
    printHeader("ControlIQ Basal & Prediction Test");

    std::cout << "Starting basal delivery at 1.0 U/hr...\n";
    deliveryManager->startBasalDelivery(1.0);

    // Simulate some initial IOB to cause BG drop
    deliveryManager->setInsulinOnBoard(3.0);
    std::cout << "IOB manually set to 3.0 U\n";

    // Simulate Time: should result in ControlIQ detecting drop
    std::cout << "\n[Simulating Time: 6 minutes]\n";
    simulateTime(6);

    std::cout << "\nReducing IOB to 1.0 U to simulate insulin fading...\n";
    deliveryManager->setInsulinOnBoard(1.0);

    std::cout << "\n[Simulating Time: 6 more minutes]\n";
    simulateTime(6);

    std::cout << "\nStopping basal delivery...\n";
    deliveryManager->stopBasalDelivery();

    std::cout << "\n[Simulating Time: 3 post-stop minutes]\n";
    simulateTime(3);

    std::cout << "\n[ControlIQ Basal Test Complete]\n";
}

void PumpTester::testIOBDecayWithExtendedBolus() {
    printHeader("IOB Decay with Extended Bolus");

    double currentBG = 7.0;
    double carbs = 60.0;
    double iob = 0.0;

    std::cout << "Starting with BG: " << currentBG << " mmol/L\n";
    std::cout << "Carb intake: " << carbs << " g\n";

    // Calculate total dose
    double totalDose = bolusCalculator->calculateBolus(currentBG, carbs, iob, activeProfile);
    std::cout << "Total Recommended Dose: " << totalDose << " U\n";

    // Schedule: 3U now, 4U over 4 mins
    double immediate = 3.0;
    double duration = 4.0;
    int splits = 4;

    std::cout << "\nDelivering extended bolus...\n";
    deliveryManager->deliverBolus(totalDose, true, immediate, duration, splits, simulator->getSimulatedMinutes());

    // Simulate time passing
    simulateTime(8); // Watch as IOB increases and then starts to decay
}

void PumpTester::simulateTime(double minutes) {
    std::cout << "\n[Simulating Time: " << minutes << " minutes]\n";
    for (int i = 0; i < static_cast<int>(minutes); ++i) {
        simulator->updateSimulationState();
    }
}

void PumpTester::printHeader(const std::string& title) {
    std::cout << "\n==== " << title << " ====\n";
}
