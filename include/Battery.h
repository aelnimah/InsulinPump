#ifndef BATTERY_H
#define BATTERY_H

class Battery {
private:
    int level;
public:
    Battery();
    ~Battery();

    int getLevel() const;
    void setLevel(int lvl);
};

#endif // BATTERY_H
