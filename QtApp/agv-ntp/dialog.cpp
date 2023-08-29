#include "dialog.h"
#include "ui_dialog.h"

// Funtions declaration
int init_color_sensor();
int read_RED(int fd);
int read_GREEN(int fd);
int read_BLUE(int fd);
int read_CLEAR(int fd);

// Function to init GPIO pins as inputs, outputs or PWM outputs
// To be called in setup part of the program
void initGPIO();

// To be called in setup part of the program
void initPWM();

// Turn the PWM ON
void PWM_ON(int, int);

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


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);


    // To be moved into a function called Init - it will be called after every delivery again
    //gridLayout = new QGridLayout();


    box1 = new QComboBox();
    box2 = new QComboBox();

    for(int i = 0; i < 9; i++)
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
    //ui->gridLayout->addWidget(label_item, 2, 0);
    ui->verticalLayout->insertWidget(1, label_item);

    plain_text = new QPlainTextEdit();
    //ui->gridLayout->addWidget(plain_text, 3, 0);
    ui->verticalLayout->insertWidget(2, plain_text);

    label_status_bar = new QLabel();
    ui->verticalLayout->insertWidget(3, label_status_bar);
    string = "Status bar...";
    label_status_bar->setText(string);

    progress_bar = new QProgressBar(this);
    progress_bar->setValue(0);
    ui->verticalLayout->insertWidget(5, progress_bar);

    string = "START DELIVERY";
    ui->pushButton->setText(string);

    delivery_stage = 0;
    trip_finished = 0;
    progress_sum = 0;

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
        case 8:
            first_destination = L114;
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
        case 8:
            second_destination = L114;
            break;
        }


        lines_counter = 0;
        progress_count = 0;
        progress_sum = first_destination + (abs(second_destination - first_destination)) + 2 + 5;  // NUM OF LINES ROBOT NEEDS TO GO OVER WHILE DELIVERING + 2 FOR 2 WAITING SPOTS

        //string = "Robot is on its way to You!";
        //label_status_bar->setText(string);

        // Wait until robot is at trip destination
        //trip_finished = 0;
        FIRST_TRIP(first_destination);
        //while(trip_finished == 0);    ????? da li mi treba ovo
        //trip_finished = 0;

        //string = "Robot is at the door. Please load the item.";
        //label_status_bar->setText(string);

        string = "ITEM LOADED | START DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage++;

        ui->pushButton->setEnabled(true);
        break;


    // From first destination to finish destination
    case 1:
        ui->pushButton->setEnabled(false);

        string = "Delivery underway...";
        //label_status_bar->setText(string);

        // Wait until robot is at trip destination
        DELIVERY_TRIP(first_destination, second_destination);

        string = "Item is delivered. Please unload the robot and confirm to finish delivery.";
        //label_status_bar->setText(string);

        string = "ITEM DELIVERED | FINISH DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage++;

        ui->pushButton->setEnabled(true);
        break;


    // From second destination to HOME
    case 2:
        ui->pushButton->setEnabled(false);

        string = "Robot is retrieving to home spot.";
        //label_status_bar->setText(string);

        // Wait until robot is at HOME destination
        HOME_TRIP(second_destination);

        string = "Robot is ready for next delivery.";
        //label_status_bar->setText(string);

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
    int R, G, B, C;
    
    // DRIVE FORWARD FROM THE HOME SPOT
    driveForward();
    lines_counter = 0;
    
    while(lines_counter != second_destination)
    {
        G = read_GREEN(fd);
        R = read_RED(fd);
        B = read_BLUE(fd);
        C = read_CLEAR(fd);

        //printf("%d\n", read_GREEN(fd));
        while(G > GREEN_TRESHOLD ||
              R < RED_TRESHOLD ||
              B > BLUE_TRESHOLD
              )
        {
            //printf("%d\n", read_GREEN(fd));
            delay(10);
            G = read_GREEN(fd);
            R = read_RED(fd);
            B = read_BLUE(fd);
            //C = read_CLEAR(fd);
        }

        printf("Presao liniju\n");
        lines_counter++;
        progress_count++;
        progress_bar->setValue((int)(progress_count / progress_sum * 100));
    }
    
    //printf("R %d  G %d  B %d  C %d\n", R, G, B, C);
    lines_counter = 0;
    
    stop();
    delay(2000);


    // TURN

    if(second_destination < 111) turnRight();    // ZAMENI!!!!!!
    else turnLeft();


    delay(1400);


    stop();
    delay(1000);
    
    // printf("Idem napred\n");
    
    // DRIVE FORWARD UNTIL IT REACHES THE LINE IN FRONT OF THE DOOR
    driveForward();
    delay(2000);
    
    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    while(read_GREEN(fd) > GREEN_TRESHOLD ||
          read_RED(fd) < RED_TRESHOLD ||
          read_BLUE(fd) > BLUE_TRESHOLD ||
          read_CLEAR(fd) > CLEAR_TRESHOLD
          ) delay(20);
          
    //printf("R %d  G %d  B %d  C %d\n", read_RED(fd), read_GREEN(fd), read_BLUE(fd), read_CLEAR(fd));
    stop();

    // printf("Vrata\n");

    //backward_flag = ~backward_flag;
}


// MIDDLE PART OF DELIVERY - FROM FIRST TO SECOND DESTINATION

void Dialog::DELIVERY_TRIP(int first_destination, int second_destination)
{
    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    driveBackward();
    delay(3000);
    
    while(read_GREEN(fd) > GREEN_TRESHOLD ||
          read_RED(fd) < RED_TRESHOLD ||
          read_BLUE(fd) > BLUE_TRESHOLD ||
          read_CLEAR(fd) > CLEAR_TRESHOLD
          ) delay(20);
    
    // POSSIBLY ADD FOR IT TO CROSS THE LINE FIRST AND THEN TURN
    stop();
    delay(1000);

    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    if(first_destination < 111)  // YOU ARE ON A RIGHT-HAND SIDE OF THE HALLWAY
        backward_turnLeft();
    else
        backward_turnRight();

    delay(1400);

    stop();
    delay(1000);

    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    if(second_destination - first_destination > 0)
    {
        driveForward();
    }
    else
    {
        driveBackward();
    }

    delay(2000); // TO STEP OFF THE LINE IT IS CURRENTLY ON

    while(lines_counter != abs(second_destination - first_destination))
    {
        while(read_GREEN(fd) > GREEN_TRESHOLD ||
              read_RED(fd) < RED_TRESHOLD ||
              read_BLUE(fd) > BLUE_TRESHOLD ||
              read_CLEAR(fd) > CLEAR_TRESHOLD
              ) delay(20);

        delay(1000);
        //while(read_GREEN(fd) > GREEN_TRESHOLD);
        lines_counter++;

        progress_count++;
        progress_bar->setValue((int)(progress_count / progress_sum * 100));

    }

    lines_counter = 0;
    stop();
    delay(1000);

    // TURN

    if(second_destination < 111) turnRight();
    else turnLeft();

    delay(1400);

    stop();
    delay(1000);

    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    // DRIVE FORWARD UNTIL IT REACHES THE LINE IN FRONT OF THE DOOR
    driveForward();
    delay(2000);
    
    while(read_GREEN(fd) > GREEN_TRESHOLD ||
          read_RED(fd) < RED_TRESHOLD ||
          read_BLUE(fd) > BLUE_TRESHOLD ||
          read_CLEAR(fd) > CLEAR_TRESHOLD
          ) delay(20);
    
    stop();

    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));
}


// FINAL PART OF DELIVERY - FROM SECONDD DESTINATION GET BACK TO HOME SPOT

void Dialog::HOME_TRIP(int first_destination)
{
    driveBackward();
    
    while(read_GREEN(fd) > GREEN_TRESHOLD ||
          read_RED(fd) < RED_TRESHOLD ||
          read_BLUE(fd) > BLUE_TRESHOLD ||
          read_CLEAR(fd) > CLEAR_TRESHOLD
          ) delay(20);
   
    stop();
    delay(1000);

    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    if(first_destination < 111)  // YOU ARE ON A RIGHT-HAND SIDE OF THE HALLWAY
        backward_turnLeft();
    else
        backward_turnRight();

    delay(1400);

    stop();
    delay(1000);
    
    progress_count++;
    progress_bar->setValue((int)(progress_count / progress_sum * 100));

    driveBackward();
    
    lines_counter = 0;
    
   	while(lines_counter != second_destination)
    {
        while(read_GREEN(fd) > GREEN_TRESHOLD ||
              read_RED(fd) < RED_TRESHOLD ||
              read_BLUE(fd) > BLUE_TRESHOLD ||
              read_CLEAR(fd) > CLEAR_TRESHOLD
              ) delay(20);

        delay(1000);
        
        lines_counter++;

        progress_count++;
        progress_bar->setValue((int)(progress_count / progress_sum * 100));
    }

    stop();

    progress_bar->setValue(100);  // DELIVERY OVER
}

void driveBackward()
{
    PWM_ON(50, 50);

    // Motor1
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);

    // Motor 2
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
}

void turnLeft()
{
    PWM_ON(40, 0);

    // Motor1
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);

    // Motor 2
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
}

void turnRight()
{
    PWM_ON(0, 60);

    // Motor1
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);

    // Motor 2
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
}

void backward_turnLeft()
{
    PWM_ON(0, 60);

    // Motor1
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);

    // Motor 2
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
}

void backward_turnRight()
{
    PWM_ON(40, 0);

    // Motor1
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);

    // Motor 2
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
}


// Function implementation
void initGPIO()
{
    // Motor 1
    pinMode(IN1, 		OUTPUT);
    pinMode(IN2, 		OUTPUT);
    pinMode(ENABLE_A, 	PWM_OUTPUT);

    // Motor 2
    pinMode(IN3, 		OUTPUT);
    pinMode(IN4, 		OUTPUT);
    pinMode(ENABLE_B, 	PWM_OUTPUT);

    // Front HC-SR04
    pinMode(TRIGGER_FRONT, 	OUTPUT);
    pinMode(ECHO_FRONT, 	INPUT);

    // Back HC-SR04
    pinMode(TRIGGER_BACK, 	OUTPUT);
    pinMode(ECHO_BACK, 	INPUT);
}

void initPWM()
{
    pwmSetClock(PWM_CLOCK);
    pwmSetRange(PWM_RANGE);
    pwmSetMode(PWM_MODE_MS);
}

void PWM_ON(int dutyA, int dutyB)
{
    pwmWrite(ENABLE_A, dutyA);
    pwmWrite(ENABLE_B, dutyB);
}

void PWM_OFF()
{
    pwmWrite(ENABLE_A, 0);
    pwmWrite(ENABLE_B, 0);
}

int init_color_sensor()
{
    // I2C initialization
    int fd;
    fd = wiringPiI2CSetup(SENSOR_ADDRESS);
    if(fd == -1){
        printf("Error during I2C initialization.");
        return -1;
    }

    // Check if sensor is available and communicates by checking ID register
    int ID;
    wiringPiI2CWrite(fd, SENSOR_ID_REG);
    ID = wiringPiI2CRead(fd);

    // 77 = 0x4D
    // 68 = 0x44
    if(ID != 77 && ID != 68){
        printf("Error! Sensor not found.");
        return -1;
    }
    else	printf("Sensor found! Sensor ID is %d.\n", ID);

    // Set RGBC time to a near maximum sample rate
    wiringPiI2CWriteReg8(fd, SENSOR_RGBC_REG, 0xC0);

    // Set minimum gain in control register - gain is x1
    // If needed better resolution, change to x4 or x16
    wiringPiI2CWriteReg8(fd, SENSOR_CONTROL_REG, 0x16);

    // Enable register - set bits to start ADC and clock
    wiringPiI2CWriteReg8(fd, SENSOR_ENABLE_REG, 0x03);

    return fd;
}

int read_CLEAR(int fd)
{
    int clear_low_byte;
    int clear_high_byte;

    // Reading low byte
    wiringPiI2CWrite(fd, SENSOR_CLEAR_CH_LOW);
    clear_low_byte = wiringPiI2CRead(fd);

    // Reading high byte
    wiringPiI2CWrite(fd, SENSOR_CLEAR_CH_HIGH);
    clear_high_byte = wiringPiI2CRead(fd);

    return (((int)clear_high_byte << 8) | clear_low_byte);
}

int read_RED(int fd)
{
    int red_low_byte;
    int red_high_byte;

    // Reading low byte
    wiringPiI2CWrite(fd, SENSOR_RED_CH_LOW);
    red_low_byte = wiringPiI2CRead(fd);

    // Reading high byte
    wiringPiI2CWrite(fd, SENSOR_RED_CH_HIGH);
    red_high_byte = wiringPiI2CRead(fd);

    return (((int)red_high_byte << 8) | red_low_byte);
}

int read_GREEN(int fd)
{
    int green_low_byte;
    int green_high_byte;

    // Reading low byte
    wiringPiI2CWrite(fd, SENSOR_GREEN_CH_LOW);
    green_low_byte = wiringPiI2CRead(fd);

    // Reading high byte
    wiringPiI2CWrite(fd, SENSOR_GREEN_CH_HIGH);
    green_high_byte = wiringPiI2CRead(fd);

    return (((int)green_high_byte << 8) | green_low_byte);
}

int read_BLUE(int fd)
{
    int blue_low_byte;
    int blue_high_byte;

    // Reading low byte
    wiringPiI2CWrite(fd, SENSOR_BLUE_CH_LOW);
    blue_low_byte = wiringPiI2CRead(fd);

    // Reading high byte
    wiringPiI2CWrite(fd, SENSOR_BLUE_CH_HIGH);
    blue_high_byte = wiringPiI2CRead(fd);

    return (((int)blue_high_byte << 8) | blue_low_byte);
}

float measure_distance_front()
{
    int start_time;
    int finish_time;

    // Making sure TRIGGER pin is LOW
    digitalWrite(TRIGGER_FRONT, 0);
    delayMicroseconds(10);

    // Sending 10us HIGH pulse
    digitalWrite(TRIGGER_FRONT, 1);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_FRONT, 0);

    // Measuring distance starts as the ECHO pin becomes HIGH
    while(digitalRead(ECHO_FRONT == 0)) start_time = micros();

    // While ECHO pin is HIGH, ultrasonic waves have not returned to the reciever
    // As the waves return, ECHO pin becames LOW
    while(digitalRead(ECHO_FRONT == 1)) finish_time = micros();

    // Calculating distance based on time elapsed and known speed of sound in the air
    return (float)(finish_time - start_time) * 34300.0 / 2;
}

float measure_distance_back()
{
    int start_time;
    int finish_time;

    digitalWrite(TRIGGER_BACK, 0);
    delayMicroseconds(10);

    digitalWrite(TRIGGER_BACK, 1);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_BACK, 0);

    while(digitalRead(ECHO_BACK == 0)) start_time = micros();

    while(digitalRead(ECHO_BACK == 1)) finish_time = micros();

    return (float)(finish_time - start_time) * 34300.0 / 2;
}

void stop()
{
    PWM_OFF();

    // Motor1
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);

    // Motor 2
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 0);
}

void driveForward()
{
    PWM_ON(EN_A_VALUE, EN_B_VALUE);

    // Motor1
    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);

    // Motor 2
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
}


