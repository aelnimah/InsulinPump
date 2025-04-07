#include "ViewProfileWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleValidator>

#include "ProfileManager.h"

ViewProfileWindow::ViewProfileWindow(Profile* profile, ProfileCRUDController* controller, ProfileManager* mgr, QWidget* parent)
    : QWidget(parent), currentProfile(profile), controller(controller), profileManager(mgr)
{
    setupUI();
    loadProfileData();
}

void ViewProfileWindow::setupUI() {
    auto* layout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout();

    nameEdit = new QLineEdit(this);
    nameEdit->setReadOnly(true);
    formLayout->addRow("Name:", nameEdit);

    icrEdit = new QLineEdit(this);
    icrEdit->setReadOnly(true);
    icrEdit->setValidator(new QDoubleValidator(0, 100, 2, this));
    formLayout->addRow("ICR:", icrEdit);

    corrEdit = new QLineEdit(this);
    corrEdit->setReadOnly(true);
    corrEdit->setValidator(new QDoubleValidator(0, 100, 2, this));
    formLayout->addRow("Correction Factor:", corrEdit);

    targetBGEdit = new QLineEdit(this);
    targetBGEdit->setReadOnly(true);
    targetBGEdit->setValidator(new QDoubleValidator(0, 500, 2, this));
    formLayout->addRow("Target BG:", targetBGEdit);

    layout->addLayout(formLayout);

    auto* buttonLayout = new QHBoxLayout();

    updateButton = new QPushButton("Update", this);
    connect(updateButton, &QPushButton::clicked, this, &ViewProfileWindow::onUpdateClicked);
    buttonLayout->addWidget(updateButton);

    saveButton = new QPushButton("Save", this);
    saveButton->setEnabled(false);
    connect(saveButton, &QPushButton::clicked, this, &ViewProfileWindow::onSaveClicked);
    buttonLayout->addWidget(saveButton);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &ViewProfileWindow::backToProfiles);
    buttonLayout->addWidget(backButton);

    layout->addLayout(buttonLayout);
}

void ViewProfileWindow::loadProfileData() {
    if (!currentProfile) return;

    nameEdit->setText(QString::fromStdString(currentProfile->getName()));
    icrEdit->setText(QString::number(currentProfile->getInsulinToCarbRatio()));
    corrEdit->setText(QString::number(currentProfile->getCorrectionFactor()));
    targetBGEdit->setText(QString::number(currentProfile->getTargetBG()));

    // Lock fields and disable save button
    nameEdit->setReadOnly(true);
    icrEdit->setReadOnly(true);
    corrEdit->setReadOnly(true);
    targetBGEdit->setReadOnly(true);
    saveButton->setEnabled(false);

    // Check if this profile is the active profile
    if (profileManager && profileManager->getActiveProfile() &&
        profileManager->getActiveProfile()->getName() == currentProfile->getName())
    {
        // Set a light-green background as an indicator
        nameEdit->setStyleSheet("background-color: lightgreen;");
    } else {
        // Clear any indicator if not active
        nameEdit->setStyleSheet("");
    }
}

void ViewProfileWindow::setProfile(Profile* profile) {
    currentProfile = profile;
    loadProfileData();
}

void ViewProfileWindow::onUpdateClicked() {
    nameEdit->setReadOnly(false);
    icrEdit->setReadOnly(false);
    corrEdit->setReadOnly(false);
    targetBGEdit->setReadOnly(false);
    saveButton->setEnabled(true);
}

void ViewProfileWindow::onSaveClicked() {
    if (!currentProfile) return;

    std::string oldName = currentProfile->getName();
    std::string newName = nameEdit->text().toStdString();
    double newICR = icrEdit->text().toDouble();
    double newCorr = corrEdit->text().toDouble();
    double newTBG = targetBGEdit->text().toDouble();

    controller->updateProfile(oldName, newName, newICR, newCorr, newTBG);

    // Update currentProfile fields directly
    currentProfile->setName(newName);
    currentProfile->setInsulinToCarbRatio(newICR);
    currentProfile->setCorrectionFactor(newCorr);
    currentProfile->setTargetBG(newTBG);

    // Reset view
    loadProfileData();

    emit backToProfiles();
}


