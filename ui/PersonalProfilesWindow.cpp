#include "PersonalProfilesWindow.h"
#include "ProfileManager.h"
#include "ProfileCRUDController.h"
#include "AddProfileWindow.h"
#include "Profile.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QListWidgetItem>

#include <QDebug>

PersonalProfilesWindow::PersonalProfilesWindow(ProfileManager* mgr, ProfileCRUDController* crud, QWidget* parent)
    : QWidget(parent), profileManager(mgr), controller(crud)
{
    setWindowTitle("Personal Profiles");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    profileList = new QListWidget(this);
    mainLayout->addWidget(profileList);

    connect(profileList, &QListWidget::itemDoubleClicked, this, &PersonalProfilesWindow::onProfileDoubleClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add", this);
    setActiveButton = new QPushButton("Set Active", this);
    deleteButton = new QPushButton("Delete", this);
    backButton = new QPushButton("Back", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(setActiveButton); 
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(backButton);

    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &PersonalProfilesWindow::onAddClicked);
    connect(setActiveButton, &QPushButton::clicked, this, &PersonalProfilesWindow::onSetActiveClicked);
    connect(deleteButton, &QPushButton::clicked, this, &PersonalProfilesWindow::onDeleteClicked);
    connect(backButton, &QPushButton::clicked, this, &PersonalProfilesWindow::onBackClicked);

    refreshProfileList();
}

void PersonalProfilesWindow::refreshProfileList() {
    profileList->clear();
    auto profiles = profileManager->getAllProfiles();
    Profile* activeProfile = profileManager->getActiveProfile();  // Get current active profile

    for (const auto* profile : profiles) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(profile->getName()));
        if (activeProfile && activeProfile->getName() == profile->getName()) {
            // Create a small green dot icon
            QPixmap pixmap(10, 10);
            pixmap.fill(Qt::green);
            item->setIcon(QIcon(pixmap));
        }
        profileList->addItem(item);
    }
}

void PersonalProfilesWindow::onAddClicked() {
    emit goToAddProfile();
}

void PersonalProfilesWindow::onSetActiveClicked() {
    qDebug() << "Set Active button clicked.";
    QListWidgetItem* selected = profileList->currentItem();
    if (!selected) {
        QMessageBox::warning(this, "No Selection", "Please select a profile to set active.");
        return;
    }
    QString name = selected->text();
    profileManager->setActiveProfile(name.toStdString());
    QMessageBox::information(this, "Active Profile", "Profile '" + name + "' is now active.");
    refreshProfileList();
}

void PersonalProfilesWindow::onDeleteClicked() {
    QListWidgetItem* selected = profileList->currentItem();
    if (!selected) return;

    QString name = selected->text();
    auto confirm = QMessageBox::question(this, "Confirm Delete",
                      "Delete profile '" + name + "'?",
                      QMessageBox::Yes | QMessageBox::No);

    if (confirm == QMessageBox::Yes) {
        profileManager->deleteProfile(name.toStdString());
        refreshProfileList();
    }
}

void PersonalProfilesWindow::onBackClicked() {
    emit backToOptions();
}

void PersonalProfilesWindow::onProfileDoubleClicked(QListWidgetItem* item) {
    auto* p = profileManager->getProfileByName(item->text().toStdString());
    if (p) emit viewProfile(p);
}
