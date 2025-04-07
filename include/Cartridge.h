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
