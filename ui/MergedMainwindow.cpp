#include "MergedMainWindow.h"

// Include Qt widgets headers required for building the pages:
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QDebug>

// Include your domain headers (ensure these are in your include paths):
#include "ProfileManager.h"
#include "ProfileCRUDController.h"
#include "Profile.h"

#include "BolusManager.h"
// Bolus dependencies
#include "BolusCalculator.h"
#include "InsulinDeliveryManager.h"
#include "CGMSensorInterface.h"
#include "Cartridge.h"


// ---------------------- MergedMainWindow Implementation ----------------------

MergedMainWindow::MergedMainWindow(ProfileManager* mgr, QWidget* parent)
    : QMainWindow(parent), profileManager(mgr)
{
     // ---- Initialize BolusManager and its dependencies ----
     BolusCalculator* bolusCalc = new BolusCalculator();
     InsulinDeliveryManager* insulinDeliveryMgr = new InsulinDeliveryManager();
     CGMSensorInterface* cgmInterface = new CGMSensorInterface();
 
     // Optionally, initialize a Cartridge and assign it to the InsulinDeliveryManager.
     Cartridge* cartridge = new Cartridge();
     insulinDeliveryMgr->setCartridge(cartridge);
 
     // Create bolusManager properly with all needed dependencies.
     bolusManager = new BolusManager(profileManager, bolusCalc, insulinDeliveryMgr, cgmInterface);
 


    // Create the domain helper for CRUD operations.
    crudController = new ProfileCRUDController(profileManager);

    // Create the stacked widget
    stackedWidget = new QStackedWidget(this);

    // Call our page setup functions
    setupHomePage();
    setupOptionsPage();
    setupProfilesPage();
    setupAddProfilePage();
    setupViewProfilePage();

    // Add pages (order matters)
    stackedWidget->addWidget(homePage);      // index 0
    stackedWidget->addWidget(optionsPage);     // index 1
    stackedWidget->addWidget(profilesPage);    // index 2
    stackedWidget->addWidget(addProfilePage);  // index 3
    stackedWidget->addWidget(viewProfilePage); // index 4

    // Start with home page
    stackedWidget->setCurrentWidget(homePage);

    // Create the overall layout that includes a simulation clock
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    simTimeLabel = new QLabel("00:00", this);
    simTimeLabel->setAlignment(Qt::AlignCenter);
    simTimeLabel->setStyleSheet("font-weight: bold; font-size: 18px;");
    mainLayout->addWidget(simTimeLabel);
    mainLayout->addWidget(stackedWidget);
    container->setLayout(mainLayout);
    setCentralWidget(container);

    setWindowTitle("Insulin Pump Simulator - CRUD");
    resize(480, 320);

    // Set up simulation clock timer (for demonstration)
    simulationTime = QTime(0, 0, 0);
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MergedMainWindow::updateSimulationClock);
    simulationTimer->start(2000);
}

MergedMainWindow::~MergedMainWindow()
{
    // Any additional cleanup if needed.
}

// ---------------------- Page Setup Functions ----------------------

void MergedMainWindow::setupHomePage()
{
    homePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(homePage);

    // A button to go to options
    QPushButton* optionsButton = new QPushButton("Options", homePage);
    layout->addWidget(optionsButton);
    connect(optionsButton, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);

    // Bolus button
    QPushButton* bolusButton = new QPushButton("Bolus", homePage);
    layout->addWidget(bolusButton);
    connect(bolusButton, &QPushButton::clicked, this, &MergedMainWindow::showBolusPage);

    homePage->setLayout(layout);
}

void MergedMainWindow::setupOptionsPage()
{
    optionsPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(optionsPage);

    // Button to go to the personal profiles page
    QPushButton* profilesButton = new QPushButton("Personal Profiles", optionsPage);
    layout->addWidget(profilesButton);
    connect(profilesButton, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);

    // Button to return to home
    QPushButton* homeButton = new QPushButton("Home", optionsPage);
    layout->addWidget(homeButton);
    connect(homeButton, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);

    optionsPage->setLayout(layout);
}

void MergedMainWindow::setupProfilesPage()
{
    profilesPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(profilesPage);

    // Create the list widget to display profiles
    profileList = new QListWidget(profilesPage);
    layout->addWidget(profileList);

    // Create button layout for actions
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add", profilesPage);
    QPushButton* setActiveBtn = new QPushButton("Set Active", profilesPage);
    QPushButton* deleteBtn = new QPushButton("Delete", profilesPage);
    QPushButton* backBtn = new QPushButton("Back", profilesPage);
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(setActiveBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(backBtn);
    layout->addLayout(buttonLayout);

    // Connect signals:
    connect(addBtn, &QPushButton::clicked, this, &MergedMainWindow::showAddProfilePage);
    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showOptionsPage);
    connect(setActiveBtn, &QPushButton::clicked, [=]() {
        QListWidgetItem* selected = profileList->currentItem();
        if (selected) {
            QString name = selected->text();
            profileManager->setActiveProfile(name.toStdString());
            QMessageBox::information(this, "Active Profile", "Profile '" + name + "' is now active.");
            refreshProfilesList();
        } else {
            QMessageBox::warning(this, "No Selection", "Please select a profile.");
        }
    });
    connect(deleteBtn, &QPushButton::clicked, [=]() {
        QListWidgetItem* selected = profileList->currentItem();
        if (selected) {
            QString name = selected->text();
            // Use a confirmation dialog.
            if (QMessageBox::question(this, "Confirm Delete", "Delete profile '" + name + "'?") == QMessageBox::Yes) {
                profileManager->deleteProfile(name.toStdString());
                refreshProfilesList();
            }
        } else {
            QMessageBox::warning(this, "No Selection", "Please select a profile to delete.");
        }
    });

    connect(profileList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
        if (item) {
            Profile* p = profileManager->getProfileByName(item->text().toStdString()); // For demonstration, you might get the profile by name.
            if (p)
                showViewProfilePage(p);
            
        }
    });

    profilesPage->setLayout(layout);
    refreshProfilesList();
}

void MergedMainWindow::setupAddProfilePage()
{
    addProfilePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(addProfilePage);

    // Create form fields using QFormLayout
    QFormLayout* formLayout = new QFormLayout();

    QLineEdit* nameEdit = new QLineEdit(addProfilePage);
    QDoubleSpinBox* icrSpin = new QDoubleSpinBox(addProfilePage);
    icrSpin->setRange(0.1, 100.0);
    QDoubleSpinBox* corrSpin = new QDoubleSpinBox(addProfilePage);
    corrSpin->setRange(0.1, 100.0);
    QDoubleSpinBox* targetBGSpin = new QDoubleSpinBox(addProfilePage);
    targetBGSpin->setRange(1.0, 20.0);

    formLayout->addRow("Profile Name:", nameEdit);
    formLayout->addRow("Insulin/Carb Ratio:", icrSpin);
    formLayout->addRow("Correction Factor:", corrSpin);
    formLayout->addRow("Target BG:", targetBGSpin);

    layout->addLayout(formLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* saveBtn = new QPushButton("Save Profile", addProfilePage);
    QPushButton* cancelBtn = new QPushButton("Cancel", addProfilePage);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    addProfilePage->setLayout(layout);

    connect(saveBtn, &QPushButton::clicked, [=]() {
        if (nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Profile name is required.");
            return;
        }
        crudController->createProfile(nameEdit->text().toStdString(),
                                      icrSpin->value(),
                                      corrSpin->value(),
                                      targetBGSpin->value());
        refreshProfilesList();
        stackedWidget->setCurrentWidget(profilesPage);
    });
    connect(cancelBtn, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);
}

void MergedMainWindow::setupViewProfilePage()
{
    viewProfilePage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(viewProfilePage);
    QFormLayout* formLayout = new QFormLayout();

    // Widgets
    QLineEdit* nameEdit = new QLineEdit(viewProfilePage);
    nameEdit->setReadOnly(true);

    QDoubleSpinBox* icrSpin = new QDoubleSpinBox(viewProfilePage);
    icrSpin->setRange(0.1, 100.0);
    icrSpin->setDecimals(2);
    icrSpin->setSingleStep(0.1);
    icrSpin->setReadOnly(true);

    QDoubleSpinBox* corrSpin = new QDoubleSpinBox(viewProfilePage);
    corrSpin->setRange(0.1, 100.0);
    corrSpin->setDecimals(2);
    corrSpin->setSingleStep(0.1);
    corrSpin->setReadOnly(true);

    QDoubleSpinBox* targetBGSpin = new QDoubleSpinBox(viewProfilePage);
    targetBGSpin->setRange(1.0, 30.0);
    targetBGSpin->setDecimals(2);
    targetBGSpin->setSingleStep(0.1);
    targetBGSpin->setReadOnly(true);

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("ICR:", icrSpin);
    formLayout->addRow("Correction Factor:", corrSpin);
    formLayout->addRow("Target BG:", targetBGSpin);
    layout->addLayout(formLayout);

    // Buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* updateBtn = new QPushButton("Update", viewProfilePage);
    QPushButton* saveBtn = new QPushButton("Save", viewProfilePage);
    QPushButton* backBtn = new QPushButton("Back", viewProfilePage);
    saveBtn->setEnabled(false);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(backBtn);
    layout->addLayout(btnLayout);

    viewProfilePage->setLayout(layout);

    // Connect buttons
    connect(updateBtn, &QPushButton::clicked, [=]() {
        nameEdit->setReadOnly(false);
        icrSpin->setReadOnly(false);
        corrSpin->setReadOnly(false);
        targetBGSpin->setReadOnly(false);
        saveBtn->setEnabled(true);
        updateBtn->setEnabled(false);
    });

    connect(saveBtn, &QPushButton::clicked, [=]() {
        if (!currentProfile) return;
        std::string oldName = currentProfile->getName();
        std::string newName = nameEdit->text().toStdString();
        double newICR = icrSpin->value();
        double newCorr = corrSpin->value();
        double newTBG = targetBGSpin->value();

        crudController->updateProfile(oldName, newName, newICR, newCorr, newTBG);
        currentProfile->setName(newName);
        currentProfile->setInsulinToCarbRatio(newICR);
        currentProfile->setCorrectionFactor(newCorr);
        currentProfile->setTargetBG(newTBG);

        nameEdit->setReadOnly(true);
        icrSpin->setReadOnly(true);
        corrSpin->setReadOnly(true);
        targetBGSpin->setReadOnly(true);
        saveBtn->setEnabled(false);
        updateBtn->setEnabled(true);

        QMessageBox::information(this, "Profile Updated", "Profile has been updated.");
    });

    connect(backBtn, &QPushButton::clicked, this, &MergedMainWindow::showPersonalProfilesPage);

    // Store widgets in properties for later access in showViewProfilePage
    viewProfilePage->setProperty("nameEditPtr", QVariant::fromValue(static_cast<void*>(nameEdit)));
    viewProfilePage->setProperty("icrEditPtr", QVariant::fromValue(static_cast<void*>(icrSpin)));
    viewProfilePage->setProperty("corrEditPtr", QVariant::fromValue(static_cast<void*>(corrSpin)));
    viewProfilePage->setProperty("targetBGEditPtr", QVariant::fromValue(static_cast<void*>(targetBGSpin)));
}


void MergedMainWindow::refreshProfilesList()
{
    // Assuming profileList is the QListWidget on the profiles page.
    if (!profileList)
        return;
    profileList->clear();
    auto profiles = profileManager->getAllProfiles();
    Profile* activeProfile = profileManager->getActiveProfile();
    for (const auto* profile : profiles) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(profile->getName()));
        if (activeProfile && activeProfile->getName() == profile->getName()) {
            QPixmap pixmap(10, 10);
            pixmap.fill(Qt::green);
            item->setIcon(QIcon(pixmap));
        }
        profileList->addItem(item);
    }
}

// Flow 2
// This member variable is declared in your header:
// QWidget* bolusInputPage;

void MergedMainWindow::setupBolusInputPage()
{
    // Create the bolus page if it doesn't exist yet.
    if (!bolusInputPage) {
        bolusInputPage = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(bolusInputPage);
        QFormLayout* formLayout = new QFormLayout();

        // Blood Glucose (BG) field (locked by default)
        QLabel* bgLabel = new QLabel("Blood Glucose (mg/dL):", bolusInputPage);
        QLineEdit* bgLineEdit = new QLineEdit(bolusInputPage);
        bgLineEdit->setText("180"); // preset value; update from CGM as needed.
        bgLineEdit->setReadOnly(true);
        formLayout->addRow(bgLabel, bgLineEdit);

        // Carbohydrate Intake field (editable)
        QLabel* carbLabel = new QLabel("Carbohydrate Intake (g):", bolusInputPage);
        QLineEdit* carbLineEdit = new QLineEdit(bolusInputPage);
        formLayout->addRow(carbLabel, carbLineEdit);

        mainLayout->addLayout(formLayout);

        // Horizontal layout for buttons.
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        // "Manual BG" button unlocks the BG field.
        QPushButton* manualBGButton = new QPushButton("Manual BG", bolusInputPage);
        buttonLayout->addWidget(manualBGButton);

        // "Calculate Bolus" button.
        QPushButton* calculateButton = new QPushButton("Calculate Bolus", bolusInputPage);
        buttonLayout->addWidget(calculateButton);

        // "Back" button to return to home.
        QPushButton* backButton = new QPushButton("Back", bolusInputPage);
        buttonLayout->addWidget(backButton);

        mainLayout->addLayout(buttonLayout);

        // Connections for unlocking BG field.
        connect(manualBGButton, &QPushButton::clicked, [=]() {
            bgLineEdit->setReadOnly(false);
        });

        // Connection for "Calculate Bolus":
        // Instead of calculating here, BolusInputPage should emit a signal.
        // In a merged approach, we connect the bolusInputPage's signal to a lambda below.
        // (We assume BolusInputPage is used here as a widget; if you already integrated its code into this helper, 
        // you can extract the BG and carb values directly.)
        // For demonstration, we'll connect the calculate button to emit a signal:
        connect(calculateButton, &QPushButton::clicked, [=]() {
            double bg = bgLineEdit->text().toDouble();
            double carbs = carbLineEdit->text().toDouble();
            qDebug() << "Calculate Bolus requested with BG:" << bg << "and Carbs:" << carbs;
            // Instead of a dummy formula, we now call bolusManager's computeRecommendedDose.
            Profile* activeProfile = profileManager->getActiveProfile();
            if (!activeProfile) {
                QMessageBox::warning(this, "Bolus Calculation Error", "No active profile is set. Please set an active profile.");
                return;
            }
            double recommendedDose = bolusManager->computeRecommendedDose(bg, carbs);
            qDebug() << "Calculated recommended bolus:" << recommendedDose << "units.";
            
            // Now go to the confirmation page.
            showBolusConfirmationPage(recommendedDose);
        });

        // Back button returns to the Home page.
        connect(backButton, &QPushButton::clicked, this, &MergedMainWindow::showHomePage);

        bolusInputPage->setLayout(mainLayout);

        // Add the bolus page to the stacked widget.
        stackedWidget->addWidget(bolusInputPage);
    }
}


void MergedMainWindow::showBolusConfirmationPage(double dose)
{
    // Create a new widget to serve as the confirmation page.
    QWidget* confirmationPage = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(confirmationPage);

    // Create a form layout with a label and a QLineEdit to display the recommended dose.
    QFormLayout* formLayout = new QFormLayout();
    QLabel* doseLabel = new QLabel("Recommended Dose (units):", confirmationPage);
    QLineEdit* doseEdit = new QLineEdit(confirmationPage);
    doseEdit->setText(QString::number(dose));
    doseEdit->setReadOnly(true);  // Locked by default.
    formLayout->addRow(doseLabel, doseEdit);
    layout->addLayout(formLayout);

    // Create a horizontal layout for the buttons.
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    QPushButton* overrideButton = new QPushButton("Override", confirmationPage);
    QPushButton* immediateButton = new QPushButton("Immediate Release", confirmationPage);
    QPushButton* extendedButton = new QPushButton("Extended Release", confirmationPage);
    QPushButton* backButton = new QPushButton("Back", confirmationPage);

    buttonLayout->addWidget(overrideButton);
    buttonLayout->addWidget(immediateButton);
    buttonLayout->addWidget(extendedButton);
    buttonLayout->addWidget(backButton);
    layout->addLayout(buttonLayout);

    // When "Override" is pressed, unlock the dose field.
    connect(overrideButton, &QPushButton::clicked, [doseEdit]() {
        doseEdit->setReadOnly(false);
    });

    // When "Immediate Release" is pressed, confirm via a popup and then deliver bolus.
    connect(immediateButton, &QPushButton::clicked, [this, doseEdit, confirmationPage]() {
        double finalDose = doseEdit->text().toDouble();
        int ret = QMessageBox::information(confirmationPage, "Confirm Immediate Bolus",
            QString("Deliver immediate bolus of %1 units?").arg(finalDose),
            QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok) {
            bolusManager->deliverBolus(finalDose, false, 0.0);
            // After delivery, return to Home.
            stackedWidget->setCurrentWidget(homePage);
        }
    });

    // Assuming doseEdit is the QLineEdit that contains the suggested dose:
    connect(extendedButton, &QPushButton::clicked, [this, doseEdit]() {
        double finalDose = doseEdit->text().toDouble();
        qDebug() << "Extended Release requested with dose:" << finalDose << "units.";
        showExtendedBolusConfigPage(finalDose);
    });

    
    // The "Back" button returns to the bolus input page.
    connect(backButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(bolusInputPage);
    });

    confirmationPage->setLayout(layout);
    stackedWidget->addWidget(confirmationPage);
    stackedWidget->setCurrentWidget(confirmationPage);
}

void MergedMainWindow::showExtendedBolusConfigPage(double dose)
{
    // Create a new widget to host the extended configuration UI.
    QWidget* extPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(extPage);

    // Display the total dose.
    QLabel* totalDoseLabel = new QLabel(QString("Total Dose: %1 units").arg(dose), extPage);
    mainLayout->addWidget(totalDoseLabel);

    // Form layout for user input.
    QFormLayout* formLayout = new QFormLayout();

    // Immediate amount field: user specifies the portion delivered immediately.
    QLineEdit* immediateEdit = new QLineEdit(extPage);
    immediateEdit->setPlaceholderText("Enter immediate amount");
    formLayout->addRow("Immediate Amount (units):", immediateEdit);

    // Splits field: number of doses (splits) for the remaining (extended) dose.
    QLineEdit* splitsEdit = new QLineEdit(extPage);
    splitsEdit->setPlaceholderText("Enter number of splits");
    formLayout->addRow("Number of Splits:", splitsEdit);

    // Duration field: total extended duration in minutes.
    QLineEdit* durationEdit = new QLineEdit(extPage);
    durationEdit->setPlaceholderText("Enter extended duration (min)");
    formLayout->addRow("Extended Duration (min):", durationEdit);

    mainLayout->addLayout(formLayout);

    // Buttons layout.
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* confirmButton = new QPushButton("Confirm", extPage);
    QPushButton* backButton = new QPushButton("Back", extPage);
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(backButton);
    mainLayout->addLayout(buttonLayout);

    // Connect confirm button.
    connect(confirmButton, &QPushButton::clicked, [this, extPage, immediateEdit, splitsEdit, durationEdit, dose]() {
        bool ok1, ok2, ok3;
        double immediateAmount = immediateEdit->text().toDouble(&ok1);
        int splits = splitsEdit->text().toInt(&ok2);
        double duration = durationEdit->text().toDouble(&ok3);

        if (!ok1 || !ok2 || !ok3) {
            QMessageBox::warning(extPage, "Input Error", "Please enter valid numerical values.");
            return;
        }

        // Ensure immediate amount does not exceed total dose.
        if (immediateAmount > dose) {
            QMessageBox::warning(extPage, "Input Error", "Immediate amount cannot exceed total dose.");
            return;
        }

        double remaining = dose - immediateAmount;
        double splitDose = (splits > 0) ? remaining / splits : 0.0;

        QString summary = QString("Immediate Amount: %1 units\n"
                                  "Remaining Extended Dose: %2 units\n"
                                  "Number of Splits: %3\n"
                                  "Dose per Split: %4 units\n"
                                  "Total Extended Duration: %5 minutes")
                              .arg(immediateAmount)
                              .arg(remaining)
                              .arg(splits)
                              .arg(splitDose)
                              .arg(duration);
        QMessageBox::information(extPage, "Extended Bolus Summary", summary);

        // Deliver the extended bolus via bolusManager.
        bolusManager->deliverBolus(dose, true, immediateAmount, duration, splits);
        // Return to the Home page.
        stackedWidget->setCurrentWidget(homePage);
    });

    // Connect Back button to return to the Bolus Input page.
    connect(backButton, &QPushButton::clicked, [this, extPage]() {
        stackedWidget->setCurrentWidget(bolusInputPage);
    });

    extPage->setLayout(mainLayout);
    stackedWidget->addWidget(extPage);
    stackedWidget->setCurrentWidget(extPage);
}



// ---------------------- Navigation Slots -----------------------

void MergedMainWindow::showHomePage()
{
    stackedWidget->setCurrentWidget(homePage);
}

void MergedMainWindow::showOptionsPage()
{
    stackedWidget->setCurrentWidget(optionsPage);
}

void MergedMainWindow::showPersonalProfilesPage()
{
    refreshProfilesList();
    stackedWidget->setCurrentWidget(profilesPage);
}

void MergedMainWindow::showAddProfilePage()
{
    stackedWidget->setCurrentWidget(addProfilePage);
}

void MergedMainWindow::showViewProfilePage(Profile* profile)
{
    if (!profile) return;
    currentProfile = profile;

    QLineEdit* nameEdit = static_cast<QLineEdit*>(viewProfilePage->property("nameEditPtr").value<void*>());
    QDoubleSpinBox* icrSpin = static_cast<QDoubleSpinBox*>(viewProfilePage->property("icrEditPtr").value<void*>());
    QDoubleSpinBox* corrSpin = static_cast<QDoubleSpinBox*>(viewProfilePage->property("corrEditPtr").value<void*>());
    QDoubleSpinBox* targetBGSpin = static_cast<QDoubleSpinBox*>(viewProfilePage->property("targetBGEditPtr").value<void*>());

    if (nameEdit && icrSpin && corrSpin && targetBGSpin) {
        nameEdit->setText(QString::fromStdString(profile->getName()));
        icrSpin->setValue(profile->getInsulinToCarbRatio());
        corrSpin->setValue(profile->getCorrectionFactor());
        targetBGSpin->setValue(profile->getTargetBG());

        nameEdit->setReadOnly(true);
        icrSpin->setReadOnly(true);
        corrSpin->setReadOnly(true);
        targetBGSpin->setReadOnly(true);

        // Highlight active profile
        Profile* active = profileManager->getActiveProfile();
        if (active && active->getName() == profile->getName()) {
            nameEdit->setStyleSheet("background-color: lightgreen;");
        } else {
            nameEdit->setStyleSheet("");
        }
    }

    stackedWidget->setCurrentWidget(viewProfilePage);
}

// Flow 2
void MergedMainWindow::showBolusPage()
{
    setupBolusInputPage();
    stackedWidget->setCurrentWidget(bolusInputPage);
}


void MergedMainWindow::updateSimulationClock() {
    simulationTime = simulationTime.addSecs(60);
    simTimeLabel->setText(simulationTime.toString("hh:mm"));
}


// ---------------------- End of MergedMainWindow.cpp ----------------------
