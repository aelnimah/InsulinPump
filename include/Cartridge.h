/*
Cartridge
    - Purpose: Simulates the physical insulin cartridge in the pump, tracks volume and usage.
    - Spec Refs:
        + Deliver Manual Bolus – Insulin must be deducted from cartridge.
        + Start/Stop/Resume Basal – Used to validate sufficient insulin before delivery.
        + Handle Pump Malfunction – Triggers warnings/errors when low or empty.
        + View Pump Info & History – Could log cartridge refills or depletion events.
    - Design Notes:
        + Supports consumption, refill, and low-level detection.
        + Default capacity is 200 units (modifiable).
        + Could be extended to simulate occlusion, leaks, or expiry.
    - Class Overview:
        + useInsulin(amount) – Deducts units; returns success/failure.
        + refill() – Resets to full capacity.
        + isLow() – Returns true if under 10% capacity.
        + get/set methods – For capacity and volume.
*/

#ifndef CARTRIDGE_H
#define CARTRIDGE_H

class Cartridge {
private:
    double capacity;
    double currentVolume;

public:
    Cartridge();
    ~Cartridge();

    bool useInsulin(double amount);
    void refill();
    bool isLow() const;

    double getCapacity() const;
    void setCapacity(double cap);
    double getCurrentVolume() const;
    void setCurrentVolume(double vol);
};

#endif // CARTRIDGE_H
