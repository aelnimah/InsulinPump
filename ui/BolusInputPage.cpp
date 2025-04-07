#include "BolusInputPage.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

BolusInputPage::BolusInputPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    // Blood Glucose field: locked by default.
    bgLabel = new QLabel("Blood Glucose (mg/dL):", this);
    bgLineEdit = new QLineEdit(this);
    bgLineEdit->setReadOnly(true);
    formLayout->addRow(bgLabel, bgLineEdit);

    // Carbohydrate Intake field: always editable.
    carbLabel = new QLabel("Carbohydrate Intake (g):", this);
    carbLineEdit = new QLineEdit(this);
    carbLineEdit->setReadOnly(false);
    formLayout->addRow(carbLabel, carbLineEdit);

    mainLayout->addLayout(formLayout);

    // Horizontal layout for buttons.
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Button to enable editing the BG value.
    editBGButton = new QPushButton("Edit BG", this);
    // Button to save the BG value (lock it again).
    saveBGButton = new QPushButton("Save BG", this);
    // Initially, the Save button is disabled.
    saveBGButton->setEnabled(false);

    // Calculate Bolus button.
    calculateButton = new QPushButton("Calculate Bolus", this);

    buttonLayout->addWidget(editBGButton);
    buttonLayout->addWidget(saveBGButton);
    buttonLayout->addWidget(calculateButton);

    mainLayout->addLayout(buttonLayout);

    connect(editBGButton, &QPushButton::clicked, this, &BolusInputPage::onEditBGClicked);
    connect(saveBGButton, &QPushButton::clicked, this, &BolusInputPage::onSaveBGClicked);
    connect(calculateButton, &QPushButton::clicked, this, &BolusInputPage::onCalculateClicked);
}

void BolusInputPage::setCGMReading(double bg)
{
    bgLineEdit->setText(QString::number(bg));
}

double BolusInputPage::getBloodGlucose() const
{
    return bgLineEdit->text().toDouble();
}

double BolusInputPage::getCarbIntake() const
{
    return carbLineEdit->text().toDouble();
}

void BolusInputPage::onEditBGClicked()
{
    bgLineEdit->setReadOnly(false);
    editBGButton->setEnabled(false);
    saveBGButton->setEnabled(true);
}

void BolusInputPage::onSaveBGClicked()
{
    bgLineEdit->setReadOnly(true);
    editBGButton->setEnabled(true);
    saveBGButton->setEnabled(false);
}

void BolusInputPage::onCalculateClicked()
{
    double bg = getBloodGlucose();
    double carbs = getCarbIntake();
    qDebug() << "Calculate Bolus requested with BG:" << bg << "and Carbs:" << carbs;
    emit calculateBolusRequested(bg, carbs);
}
