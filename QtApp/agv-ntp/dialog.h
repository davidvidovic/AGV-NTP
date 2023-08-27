#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QComboBox>
#include <QGridLayout>
#include <QPlainTextEdit>

#include <wiringPi.h>
#include <wiringPiI2C.h>

// Macros used to indicate which GPIO pins are connected to L298 pins
// Motor 1 macros
#define IN1		6	// 5
#define IN2		16	// 4
#define ENABLE_A	23

// Motor 2 macros
#define IN3		11
#define IN4		29	// 31
#define ENABLE_B	1

// Macros used to indicate which GPIO pins are connected to HC-SR04 pins
// Front HC-SR04
#define ECHO_FRONT	28
#define TRIGGER_FRONT	27

// Back HC-SR04
#define ECHO_BACK	7
#define TRIGGER_BACK	25

// PWM settings macros

// PWM freq = 19.2MHz / (PWM_CLOCK * PWM_RANGE)
#define PWM_RANGE	100
#define PWM_CLOCK	5

#define EN_A_VALUE	19
#define EN_B_VALUE	53



// Defining tresholds for each color
#define RED_TRESHOLD    7000
#define GREEN_TRESHOLD  7000
#define BLUE_TRESHOLD	6000
#define CLEAR_TRESHOLD 20000

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

#define SENSOR_ADDRESS		0x29
#define SENSOR_ID_REG		0x92
#define SENSOR_RGBC_REG		0x81
#define SENSOR_CONTROL_REG	0x8F
#define SENSOR_ENABLE_REG	0x80
#define SENSOR_RED_CH_LOW	0x96
#define SENSOR_RED_CH_HIGH	0x97
#define SENSOR_GREEN_CH_LOW	0x98
#define SENSOR_GREEN_CH_HIGH	0x99
#define SENSOR_BLUE_CH_LOW	0x9A
#define SENSOR_BLUE_CH_HIGH	0x9B
#define SENSOR_CLEAR_CH_LOW	0x94
#define SENSOR_CLEAR_CH_HIGH	0x95

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
    QPlainTextEdit *plain_text;

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
/*
    // Funtions declaration
    int init_color_sensor();
    int read_RED(int fd);
    int read_GREEN(int fd);
    int read_BLUE(int fd);

    // Function to init GPIO pins as inputs, outputs or PWM outputs
    // To be called in setup part of the program
    void initGPIO();

    // To be called in setup part of the program
    void initPWM();

    // Turn the PWM ON
    void PWM_ON();

    // Turn the PWM OFF
    void PWM_OFF();

    float measure_distance_front();
    float measure_distance_back();

    void stop();
    void driveForward();
    void driveBackward();
    void turnLeft();
    void turnRight();
    void backward_turnLeft();
    void backward_turnRight();
*/
};
#endif // DIALOG_H

