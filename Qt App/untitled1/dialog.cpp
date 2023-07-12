#include "dialog.h"
#include "ui_dialog.h"

// Defining tresholds for each color
#define RED_TRESHOLD	620
#define GREEN_TRESHOLD	1300
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

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // To be moved into a function called Init - it will be called after every delivery again

    box1 = new QComboBox();
    box2 = new QComboBox();

    for(int i = 0; i < 8; i++)
    {
        box1->addItem(array[i]);
        box2->addItem(array[i]);
    }

    ui->gridLayout->addWidget(box1, 1, 0);
    ui->gridLayout->addWidget(box2, 1, 1);

    label_menu1 = new QLabel("Choose a room to be delivered from:", this);
    ui->gridLayout->addWidget(label_menu1, 0, 0);

    label_menu2 = new QLabel("Choose a room to be delivered into:", this);
    ui->gridLayout->addWidget(label_menu2, 0, 1);

    label_item = new QLabel("Describe an item You wish to be delivered:", this);
    ui->gridLayout->addWidget(label_item, 2, 0);

    label_status_bar = new QLabel("Status bar...", this);
    ui->verticalLayout->insertWidget(3, label_status_bar);

    progress_bar = new QProgressBar(this);
    progress_bar->setValue(0);
    ui->verticalLayout->insertWidget(5, progress_bar);

    delivery_stage = 0;
    trip_finished = 0;
    backward_flag = 0;

    // Initializing wiringPi library
    if(wiringPiSetup() < 0) 	printf("Error during library initialization.");

    // Initializing GPIO pins
    initGPIO();

    // Initializing PWM settings
    initPWM();

    // Initially PWM is 0
    // No need to explicitly call this function again, it will be called whenever the motor_control::stop() is called
    PWM_OFF();

    // Initializing I2C communication and TCS3472 color sensor
    fd = init_color_sensor();

    // Initially stop the motors
    stop();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    switch(delivery_stage)
    {
    // Initial - ROBOT WILL RESTART THE COUNTER BY HIMSELF?
    /*
        string = "Waiting for delivery.";
        label_status_bar->setText(string);

        string = "START DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage++;
        break;
    */

    // From start to first destination
    case 0:
        ui->pushButton->setEnabled(false);
        box1->setEnabled(false);
        box2->setEnabled(false);

        first_destination = box1->currentIndex();
        second_destination = box2->currentIndex();

        // Get room from index
        switch(first_destination)
        {
        case 0:
            first_destination = L106;
            break;
        case 1:
            first_destination = L107;
            break;
        case 2:
            first_destination = L108;
            break;
        case 3:
            first_destination = L109;
            break;
        case 4:
            first_destination = L110;
            break;
        case 5:
            first_destination = L111;
            break;
        case 6:
            first_destination = L112;
            break;
        case 7:
            first_destination = L113;
            break;
        }

        switch(second_destination)
        {
        case 0:
            second_destination = L106;
            break;
        case 1:
            second_destination = L107;
            break;
        case 2:
            second_destination = L108;
            break;
        case 3:
            second_destination = L109;
            break;
        case 4:
            second_destination = L110;
            break;
        case 5:
            second_destination = L111;
            break;
        case 6:
            second_destination = L112;
            break;
        case 7:
            second_destination = L113;
            break;
        }

        string = "Robot is on its way to You!";
        label_status_bar->setText(string);

        // Wait until robot is at trip destination
        //trip_finished = 0;
        FIRST_TRIP(first_destination);
        //while(trip_finished == 0);    ????? da li mi treba ovo
        //trip_finished = 0;

        string = "Robot is at the door. Please load the item.";
        label_status_bar->setText(string);

        string = "ITEM LOADED | START DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage++;

        ui->pushButton->setEnabled(true);
        break;


    // From first destination to finish destination
    case 1:
        ui->pushButton->setEnabled(false);

        string = "Delivery underway...";
        label_status_bar->setText(string);

        // Wait until robot is at trip destination
        DELIVERY_TRIP(first_destination, second_destination);

        string = "Item is delivered. Please unload the robot and confirm to finish delivery.";
        label_status_bar->setText(string);

        string = "ITEM DELIVERED | FINISH DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage++;

        ui->pushButton->setEnabled(true);
        break;

    // From second destination to HOME
    case 2:
        ui->pushButton->setEnabled(false);

        string = "Robot is retrieving to home spot.";
        label_status_bar->setText(string);

        // Wait until robot is at HOME destination
        HOME_TRIP(second_destination);

        string = "Robot is ready for next delivery.";
        label_status_bar->setText(string);

        string = "START DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage = 0;

        box1->setEnabled(true);
        box2->setEnabled(true);
        ui->pushButton->setEnabled(true);
        break;
    }
}

// FIRST PART OF DELIVERY - FROM HOME TO START DESTINATION
void Dialog::FIRST_TRIP(int second_destination)
{
    // DRIVE FORWARD FROM THE HOME SPOT

    driveForward();
    while(lines_counter != second_destination))
    {
        while((measure_distance_front() < 20) && (measure_distance_back() < 20) && (read_GREEN(fd) < GREEN_TRESHOLD));

        stop();
        delay(1000);

        if(read_GREEN(fd) > GREEN_TRESHOLD) // while its over the line
        {
            while(read_GREEN(fd) > GREEN_TRESHOLD);
            lines_counter++;
        }
        else
        {
            while((measure_distance_front() > 20) || (measure_distance_back() > 20)) delay(500);
            driveForward();
        }
    }

    lines_counter = 0;


    // TURN

    if(second_destination < 111) turnLeft();
    else turnRight();

    while(read_GREEN(fd) < GREEN_TRESHOLD); // Keep turning until it hits the line
    stop();
    delay(1000);


    // CHECK IF DOOR IS OPEN

    if(measure_distance_front() < 200)  // DOOR CLOSED - TO BE CALIBRATED
    {
        stop();
        string = "Door seems to be closed. Please open the door.";
        label_status_bar->setText(string);
        while(measure_distance_front() < 200);
    }

    // DRIVE FORWARD UNTIL IT REACHES THE LINE IN FRONT OF THE DOOR
    driveForward();
    while(read_GREEN(fd) < GREEN_TRESHOLD);
    stop();

    //backward_flag = ~backward_flag;
}

// MIDDLE PART OF DELIVERY - FROM FIRST TO SECOND DESTINATION
void Dialog::DELIVERY_TRIP(int first_destination, int second_destination)
{
    driveBackward();
    while(read_GREEN(fd) < GREEN_TRESHOLD);     // POSSIBLY ADD FOR IT TO CROSS THE LINE FIRST AND THEN TURN
    stop();
    delay(1000);

    if(first_destination < 111)  // YOU ARE ON A RIGHT-HAND SIDE OF THE HALLWAY
        backward_turnLeft();
    else
        backward_turnRight();

    while(read_GREEN(fd) < GREEN_TRESHOLD); // Keep turning until it reaches the line
    stop();
    delay(1000);

    if(second_destination - first_destination > 0)
    {
        driveForward();
    }
    else
    {
        driveBackward();
    }

    delay(100); // TO STEP OFF THE LINE IT IS CURRENTLY ON

    while(lines_counter != abs(second_destination - first_destination))
    {
        while((measure_distance_front() < 20) && (measure_distance_back() < 20) && (read_GREEN(fd) < GREEN_TRESHOLD));

        stop();
        delay(1000);

        if(read_GREEN(fd) > GREEN_TRESHOLD) // while its over the line
        {
            while(read_GREEN(fd) > GREEN_TRESHOLD);
            lines_counter++;
        }
        else
        {
            while((measure_distance_front() > 20) || (measure_distance_back() > 20)) delay(500);

            if(second_destination - first_destination > 0)
            {
                driveForward();
            }
            else
            {
                driveBackward();
            }
        }
    }

    lines_counter = 0;

    // TURN

    if(second_destination < 111) turnLeft();
    else turnRight();

    while(read_GREEN(fd) < GREEN_TRESHOLD); // Keep turning until it hits the line
    stop();
    delay(1000);

    // CHECK IF DOOR IS OPEN

    if(measure_distance_front() < 200)  // DOOR CLOSED - TO BE CALIBRATED
    {
        stop();
        string = "Door seems to be closed. Please open the door.";
        label_status_bar->setText(string);
        while(measure_distance_front() < 200);
    }

    // DRIVE FORWARD UNTIL IT REACHES THE LINE IN FRONT OF THE DOOR
    driveForward();
    while(read_GREEN(fd) < GREEN_TRESHOLD);
    stop();
}

// FINAL PART OF DELIVERY - FROM SECONDD DESTINATION GET BACK TO HOME SPOT
void Dialog::HOME_TRIP(int first_destination)
{
    driveBackward();
    while(read_GREEN(fd) < GREEN_TRESHOLD);     // POSSIBLY ADD FOR IT TO CROSS THE LINE FIRST AND THEN TURN
    stop();
    delay(1000);

    if(first_destination < 111)  // YOU ARE ON A RIGHT-HAND SIDE OF THE HALLWAY
        backward_turnLeft();
    else
        backward_turnRight();

    while(read_GREEN(fd) < GREEN_TRESHOLD); // Keep turning until it reaches the line
    stop();
    delay(1000);

    driveBackward();
    while(read_RED(fd) < RED_TRESHOLD)   // Keep turning until it reaches the line - HOME LINE IS MARKED RED
    {
        if(measure_distance_back() < 20)
        {
            stop();
            delay(1000);

            string = "Something seems to be in the way. Wait or clear the path.";
            label_status_bar->setText(string);

            while(measure_distance_back() < 20) delay(1000);

            driveBackward();

            string = "Robot is retrieving to home spot.";
            label_status_bar->setText(string);
        }
    }

    stop();
}
