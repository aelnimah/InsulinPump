#ifndef ADDPROFILEWINDOW_H
#define ADDPROFILEWINDOW_H

#include <QWidget>

class QLineEdit;
class QDoubleSpinBox;
class QPushButton;
class ProfileCRUDController;

class AddProfileWindow : public QWidget {
    Q_OBJECT

public:
    AddProfileWindow(ProfileCRUDController* controller, QWidget* parent = nullptr);

signals:
    void profileAdded();
    void cancelled();

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    ProfileCRUDController* crudController;

    QLineEdit* nameEdit;
    QDoubleSpinBox* icrSpin;
    QDoubleSpinBox* corrSpin;
    QDoubleSpinBox* targetBGSpin;

    QPushButton* saveButton;
    QPushButton* cancelButton;
};

#endif // ADDPROFILEWINDOW_H
