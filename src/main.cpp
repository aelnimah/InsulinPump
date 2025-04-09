#include <QApplication>
#include "MergedMainWindow.h"
#include "ProfileManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ProfileManager* profileManager = new ProfileManager();

    MergedMainWindow w(profileManager);
    w.show();

    int ret = app.exec();

    delete profileManager;
    return ret;
}
