#include <iostream>
#include "ProfileManager.h"
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "BolusManager.h"
#include "Profile.h"
#include "BasalSegment.h"
#include "Cartridge.h"
#include "BolusInputPage.h"
#include "BolusManager.h"
#include <QDebug>

int main() {
    // Create dependency instances.
    ProfileManager pm;
    BolusCalculator bc;
    InsulinDeliveryManager idm;
    CGMSensorInterface cgm;
    
    // Create and set up a Cartridge instance.
    Cartridge* cartridge = new Cartridge();
    idm.setCartridge(cartridge);
    idm.setBolusCalculator(&bc);

    // Set an initial BG value in the CGM.
    cgm.setBG(180.0); // 180 mg/dL

    // Create a test profile.
    Profile* testProfile = new Profile();
    testProfile->setName("TestProfile");
    testProfile->setInsulinToCarbRatio(10.0);    // 10 grams per unit.
    testProfile->setCorrectionFactor(30.0);        // 30 mg/dL per unit.
    testProfile->setTargetBG(100.0);               // Target BG.
    
    // Add a default basal segment covering full day.
    BasalSegment* seg = new BasalSegment(0.0, 24.0, 1.0);
    testProfile->addBasalSegment(seg);
    
    // Add and activate the test profile.
    pm.createProfile(testProfile);
    pm.setActiveProfile("TestProfile");

    // Create BolusManager with our dependencies.
    BolusManager bm(&pm, &bc, &idm, &cgm);
    
    // Testing immediate bolus.
    std::cout << "Testing immediate bolus..." << std::endl;
    double simulatedBG = 180.0;
    double simulatedCarbs = 60.0;
    double dose = bm.computeRecommendedDose(simulatedBG, simulatedCarbs);
    std::cout << "Recommended bolus dose (immediate): " << dose << " units." << std::endl;
    bm.deliverBolus(dose, false, 0.0);
    double currentBG = bm.getBGFromCGM();
    std::cout << "Current BG from CGM: " << currentBG << std::endl;

    // Testing extended bolus.
    std::cout << "\nTesting extended bolus simulation..." << std::endl;
    // For testing, assume a total dose of 10 units.
    double totalDose = 10.0;
    // User specifies 4 units immediately.
    double immediateAmount = 4.0;
    // Extended duration: 6 hours = 360 minutes.
    double duration = 360.0;
    // User chooses 6 splits for the remaining 6 units.
    int splits = 6;
    std::cout << "User-specified extended bolus: Total dose = " << totalDose 
              << " units, immediate = " << immediateAmount 
              << " units, duration = " << duration 
              << " minutes, splits = " << splits << "." << std::endl;
    bm.deliverBolus(totalDose, true, immediateAmount, duration, splits);

    // Now simulate the passage of time.
    double simulatedTime = 0.0;
    double simulationEnd = 400.0;  // simulate 400 minutes.
    while (simulatedTime < simulationEnd) {
        idm.processScheduledExtendedDoses(simulatedTime);
        simulatedTime += 1.0;
    }
    
    // Clean up.
    delete cartridge;
    // Note: Other objects are managed on the stack or elsewhere.
    return 0;
}
