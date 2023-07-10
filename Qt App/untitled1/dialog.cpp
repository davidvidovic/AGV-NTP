#include "dialog.h"
#include "ui_dialog.h"

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
        string = "Delivery underway...";
        label_status_bar->setText(string);

        // Wait until robot is at trip destination
        // START_TRIP();

        string = "Robot is at the door. Please load the item.";
        label_status_bar->setText(string);

        string = "ITEM LOADED | START DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage++;
        break;


    // From first destination to finish destination
    case 1:
        string = "Delivery underway...";
        label_status_bar->setText(string);

        // Wait until robot is at trip destination
        // START_TRIP();

        string = "Item is delivered. Please unload the robot and confirm to finish delivery.";
        label_status_bar->setText(string);

        string = "ITEM DELIVERED | FINISH DELIVERY";
        ui->pushButton->setText(string);

        delivery_stage = 0;
        break;
    }
}

