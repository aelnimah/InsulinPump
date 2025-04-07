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
    ui/MainWindow.cpp \
    ui/OptionsWindow.cpp \
    ui/PersonalProfilesWindow.cpp \
    ui/AddProfileWindow.cpp \
    ui/ViewProfileWindow.cpp \
    ui/BolusInputPage.cpp

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
    ui/MainWindow.h \
    ui/OptionsWindow.h \
    ui/PersonalProfilesWindow.h \
    ui/AddProfileWindow.h \
    ui/ViewProfileWindow.h \
    ui/BolusInputPage.h

# Include paths
INCLUDEPATH += include \
               ui
