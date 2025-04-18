#include "CGMSensorInterface.h"
#include "Profile.h"

#include <algorithm>

CGMSensorInterface::CGMSensorInterface() : profile(nullptr), currentBG(6.0) {}
CGMSensorInterface::~CGMSensorInterface() {}

double CGMSensorInterface::getCurrentBG() const { return currentBG; }
void CGMSensorInterface::simulateNextReading() { currentBG = std::max(0.5, currentBG - 0.05); }
void CGMSensorInterface::setBG(double newValue) { currentBG = newValue; }
