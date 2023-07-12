#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QComboBox>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "../../Testing phase/color_sensor.h"
#include "../../Testing phase/motor_control.h"
#include "../../Testing phase/pins.h"
#include "../../Testing phase/ultrasonic_sensor.h"


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
    QString array[8] = {"L106", "L107", "L108", "L109", "L110", "L111", "L112", "L113"};
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
    unsigned char backward_flag = 0;
    int fd;
    int lines_counter = 0;

    void FIRST_TRIP(int second_destination);
    void DELIVERY_TRIP(int first_destination, int second_destination);
    void HOME_TRIP(int first_destination);

};
#endif // DIALOG_H
