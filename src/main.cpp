#include <QApplication>
#include "MergedMainWindow.h"
#include "PumpSimulator.h"
#include "ProfileManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Instantiate backend components
    PumpSimulator* pumpSimulator = new PumpSimulator();
    ProfileManager* profileManager = new ProfileManager();

    // Launch GUI
    MergedMainWindow w(pumpSimulator, profileManager);
    w.show();

    int ret = app.exec();

    // Clean up
    delete pumpSimulator;
    delete profileManager;
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
