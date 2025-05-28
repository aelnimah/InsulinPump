#ifndef PUMPSIMULATOR_H
#define PUMPSIMULATOR_H

class ProfileManager;
class BolusCalculator;
class InsulinDeliveryManager;
class CGMSensorInterface;
class ControlIQController;
class AlertManager;
class Battery;
class Cartridge;

class PumpSimulator {
private:
    bool isRunning;

    // Core references
    ProfileManager* profileManager;
    BolusCalculator* bolusCalculator;
    InsulinDeliveryManager* deliveryManager;
    CGMSensorInterface* cgmSensor;

    // System extensions
    ControlIQController* controlIQ;
    AlertManager* alertManager;
    Battery* battery;
    Cartridge* cartridge;

    // Time tracking
    double simulatedMinutes = 0.0;     // For CLI
    int guiSimulatedMinutes = 0;       // For GUI
    bool cliMode = false;

public:
    PumpSimulator();
    ~PumpSimulator();

    void startSimulation();
    void stopSimulation();
    void updateSimulationState(); // Simulates one tick (1 min)
    void shutdown();

    // Setters
    void setProfileManager(ProfileManager* mgr);
    void setBolusCalculator(BolusCalculator* bc);
    void setInsulinDeliveryManager(InsulinDeliveryManager* idm);
    void setCGMSensorInterface(CGMSensorInterface* cgm);
    void setControlIQController(ControlIQController* ctrl);
    void setAlertManager(AlertManager* a);
    void setBattery(Battery* b);
    void setCartridge(Cartridge* c);

    // Getters
    bool getIsRunning() const;
    void setIsRunning(bool running); 
    ProfileManager* getProfileManager();
    BolusCalculator* getBolusCalculator() const;
    InsulinDeliveryManager* getInsulinDeliveryManager() const;
    CGMSensorInterface* getCGMSensorInterface() const;
    ControlIQController* getControlIQController() const;
    AlertManager* getAlertManager() const;
    Battery* getBattery() const;
    Cartridge* getCartridge() const;

    // CLI Testing support
    void setCLIMode(bool enabled) { cliMode = enabled; }
    void incrementSimTime(double minutes);     // CLI: Add time
    double getSimulatedMinutes() const;        // CLI: Read time
    void setGUISimTime(int minutes);           // GUI: Update GUI time

    int getCurrentSimTime() const; // Shared between CLI and GUI

    double getCurrentBG() const;
    double getIOB() const;

};

#endif // PUMPSIMULATOR_H
