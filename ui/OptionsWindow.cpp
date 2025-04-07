#include "OptionsWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "ProfileManager.h"

OptionsWindow::OptionsWindow(ProfileManager* mgr, QWidget *parent)
    : QWidget(parent), profileManager(mgr)
{
    setWindowTitle("Options");
    personalProfilesButton = new QPushButton("Personal Profiles", this);
    QPushButton* homeButton = new QPushButton("Home", this);

    // Main vertical layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add your "Personal Profiles" button at the top
    mainLayout->addWidget(personalProfilesButton);

    // Add a vertical spacer to push subsequent widgets to the bottom
    mainLayout->addSpacerItem(new QSpacerItem(
        0, 0, 
        QSizePolicy::Minimum, 
        QSizePolicy::Expanding
    ));

    // Create a horizontal layout for the Home button
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();     // push Home button to the right
    bottomLayout->addWidget(homeButton);

    // Add the bottom layout to the main layout
    mainLayout->addLayout(bottomLayout);

    // Connect signals
    connect(personalProfilesButton, &QPushButton::clicked, this, &OptionsWindow::personalProfilesClicked);
    connect(homeButton, &QPushButton::clicked, this, [this]() {
        emit homeClicked();
    });
}
