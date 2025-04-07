#include "AddProfileWindow.h"
#include "ProfileCRUDController.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>

AddProfileWindow::AddProfileWindow(ProfileCRUDController* controller, QWidget* parent)
    : QWidget(parent), crudController(controller)
{
    setWindowTitle("Add New Profile");

    nameEdit = new QLineEdit(this);

    icrSpin = new QDoubleSpinBox(this);
    icrSpin->setMinimum(0.1);
    icrSpin->setMaximum(100);

    corrSpin = new QDoubleSpinBox(this);
    corrSpin->setMinimum(0.1);
    corrSpin->setMaximum(100);

    targetBGSpin = new QDoubleSpinBox(this);
    targetBGSpin->setMinimum(1.0);
    targetBGSpin->setMaximum(20.0);

    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Profile Name:", nameEdit);
    formLayout->addRow("Insulin/Carb Ratio:", icrSpin);
    formLayout->addRow("Correction Factor:", corrSpin);
    formLayout->addRow("Target BG:", targetBGSpin);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &AddProfileWindow::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddProfileWindow::onCancelClicked);
}

void AddProfileWindow::onSaveClicked() {
    QString name = nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Profile name cannot be empty.");
        return;
    }

    double icr = icrSpin->value();
    double corr = corrSpin->value();
    double tbg = targetBGSpin->value();

    crudController->createProfile(name.toStdString(), icr, corr, tbg);
    emit profileAdded();  // Let parent know a profile was added
    this->close();
}

void AddProfileWindow::onCancelClicked() {
    emit cancelled(); 
}

