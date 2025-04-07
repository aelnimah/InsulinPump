#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>

class QPushButton;
class ProfileManager;

class OptionsWindow : public QWidget {
    Q_OBJECT

public:
    explicit OptionsWindow(ProfileManager* mgr, QWidget *parent = nullptr);

signals:
    void personalProfilesClicked();
    void homeClicked();

private:
    QPushButton* personalProfilesButton;
    ProfileManager* profileManager;
};

#endif // OPTIONSWINDOW_H
