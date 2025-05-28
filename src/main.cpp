#include <QApplication>
#include "MergedMainWindow.h"
#include "PumpSimulator.h"
#include "ProfileManager.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "Battery.h"
#include "Cartridge.h"
#include "CGMSensorInterface.h"
#include "ControlIQController.h"
#include "AlertManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create components
    PumpSimulator* pumpSimulator = new PumpSimulator();
    ProfileManager* profileManager = new ProfileManager();
    BolusCalculator* bolusCalc = new BolusCalculator();
    InsulinDeliveryManager* deliveryMgr = new InsulinDeliveryManager();
    Battery* battery = new Battery();
    Cartridge* cartridge = new Cartridge();
    CGMSensorInterface* cgm = new CGMSensorInterface();
    ControlIQController* controlIQ = new ControlIQController();
    AlertManager* alerts = new AlertManager();

    // Wire components
    deliveryMgr->setBattery(battery);
    deliveryMgr->setCartridge(cartridge);
    controlIQ->setCGMSensor(cgm);
    controlIQ->setInsulinDeliveryManager(deliveryMgr);

    // Inject into simulator
    pumpSimulator->setProfileManager(profileManager);
    pumpSimulator->setBolusCalculator(bolusCalc);
    pumpSimulator->setInsulinDeliveryManager(deliveryMgr);
    pumpSimulator->setBattery(battery);
    pumpSimulator->setCartridge(cartridge);
    pumpSimulator->setCGMSensorInterface(cgm);
    pumpSimulator->setControlIQController(controlIQ);
    pumpSimulator->setAlertManager(alerts);

    // Start GUI
    MergedMainWindow w(pumpSimulator, profileManager);
    w.show();

    int ret = app.exec();

    delete profileManager;
    delete pumpSimulator;

    return ret;
}


/*
#include "PumpTester.h"

int main() {
    PumpTester tester;
    tester.runAllTests();
    return 0;
}
*/
