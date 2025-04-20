QT       += core gui widgets charts

CONFIG   += c++17
TEMPLATE = app
TARGET   = InsulinSimulator

# Source files
SOURCES += \
    src/main.cpp \
    src/PumpSimulator.cpp \
    src/DataLogger.cpp \
    src/Alarm.cpp \
    src/ControlIQController.cpp \
    src/AlertManager.cpp \
    src/BasalSegment.cpp \
    src/Profile.cpp \
    src/ProfileManager.cpp \
    src/CGMSensorInterface.cpp \
    src/ProfileCRUDController.cpp \
    src/BolusCalculator.cpp \
    src/InsulinDeliveryManager.cpp \
    src/BolusManager.cpp \
    src/Cartridge.cpp \
    src/Battery.cpp \
    src/MergedMainWindow.cpp

# Header files
HEADERS += \
    include/Alarm.h \
    include/AlertManager.h \
    include/DataLogger.h \
    include/PumpSimulator.h \
    include/BasalSegment.h \
    include/Profile.h \
    include/ControlIQController.h \
    include/ProfileManager.h \
    include/ProfileCRUDController.h \
    include/BolusCalculator.h \
    include/InsulinDeliveryManager.h \
    include/CGMSensorInterface.h \
    include/BolusManager.h \
    include/Cartridge.h \
    include/Battery.h \
    include/MergedMainWindow.h

# Include paths
INCLUDEPATH += include \
               ui
