#include <QApplication>
#include "MergedMainWindow.h"
#include "ProfileManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the ProfileManager (and load any existing profiles if needed).
    ProfileManager* profileManager = new ProfileManager();

    // Create and show the main window.
    MergedMainWindow w(profileManager);
    w.show();

    int ret = app.exec();

    // Cleanup.
    delete profileManager;
    return ret;
}
