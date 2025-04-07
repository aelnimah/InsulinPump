#include <QApplication>
#include "MainWindow.h"
#include "ProfileManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ProfileManager* profileManager = new ProfileManager();
    MainWindow w(profileManager);
    w.show();

    return app.exec();
}
