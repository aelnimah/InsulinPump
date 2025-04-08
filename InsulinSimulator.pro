QT       += core gui widgets

CONFIG   += c++17
TEMPLATE = app
TARGET   = InsulinSimulator

# Source files
SOURCES += \
    main.cpp \
    src/PumpSimulator.cpp \
    src/BasalSegment.cpp \
    src/Profile.cpp \
    src/ProfileManager.cpp \
    src/ProfileCRUDController.cpp \
    src/BolusCalculator.cpp \
    src/InsulinDeliveryManager.cpp \
    src/CGMSensorInterface.cpp \
    src/BolusManager.cpp \
    src/Cartridge.cpp \
    src/Battery.cpp \
    ui/MergedMainWindow.cpp 

# Header files
HEADERS += \
    include/PumpSimulator.h \
    include/BasalSegment.h \
    include/Profile.h \
    include/ProfileManager.h \
    include/ProfileCRUDController.h \
    include/BolusCalculator.h \
    include/InsulinDeliveryManager.h \
    include/CGMSensorInterface.h \
    include/BolusManager.h \
    include/Cartridge.h \
    include/Battery.h \
    ui/MergedMainWindow.h 

# Include paths
INCLUDEPATH += include \
               ui

FORMS += \
    untitled/BolusInputPage.ui
