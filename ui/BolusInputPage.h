#ifndef BOLUSINPUTPAGE_H
#define BOLUSINPUTPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QLineEdit;

class BolusInputPage : public QWidget {
    Q_OBJECT
public:
    explicit BolusInputPage(QWidget *parent = nullptr);

    // Set the CGM reading for blood glucose.
    void setCGMReading(double bg);
    // Get the blood glucose value.
    double getBloodGlucose() const;
    // Get the carbohydrate intake value.
    double getCarbIntake() const;

signals:
    // Emitted when the user clicks "Calculate Bolus"
    // Sends the current blood glucose (from the BG field) and carbohydrate intake.
    void calculateBolusRequested(double bloodGlucose, double carbIntake);

private slots:
    void onEditBGClicked();
    void onSaveBGClicked();
    void onCalculateClicked();

private:
    QLabel *bgLabel;
    QLineEdit *bgLineEdit;
    QLabel *carbLabel;
    QLineEdit *carbLineEdit;
    QPushButton *editBGButton;
    QPushButton *saveBGButton;
    QPushButton *calculateButton;
};

#endif // BOLUSINPUTPAGE_H
