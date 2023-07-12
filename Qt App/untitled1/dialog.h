#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QComboBox>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "../../Testing_phase/color_sensor.h"
#include "../../Testing_phase/motor_control.h"
#include "../../Testing_phase/pins.h"
#include "../../Testing_phase/ultrasonic_sensor.h"

// Defining tresholds for each color
#define RED_TRESHOLD	620
#define GREEN_TRESHOLD  1300
#define BLUE_TRESHOLD	500

// Defining rooms - number of lines before and including that door - number of line on which robot should be triggered to turn
#define HOME    0
#define L106    1
#define L107    4
#define L108    5
#define L109    6
#define L110    9
#define L111    2
#define L112    3
#define L113    7
#define L114    8

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog(); 

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QString string;
    QString array[9] = {"L106", "L107", "L108", "L109", "L110", "L111", "L112", "L113", "L114"};
    QComboBox *box1;
    QComboBox *box2;
    QLabel *label_menu1;
    QLabel *label_menu2;
    QLabel *label_status_bar;
    QLabel *label_item;
    QProgressBar *progress_bar;

    int delivery_stage = 0;
    int first_destination;
    int second_destination;
    int trip_finished;
    int fd;
    int lines_counter = 0;
    int progress_sum = 0;
    int progress_count = 0;

    void FIRST_TRIP(int second_destination);
    void DELIVERY_TRIP(int first_destination, int second_destination);
    void HOME_TRIP(int first_destination);

};
#endif // DIALOG_H
