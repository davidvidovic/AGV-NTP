#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // To be moved into a function called Init - it will be called after every delivery again

    label = new QLabel("Status bar...", this);
    ui->verticalLayout->insertWidget(3, label);

    progress_bar = new QProgressBar(this);
    progress_bar->setValue(0);
    ui->verticalLayout->insertWidget(4, progress_bar);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    label->setText("Delivery under way");
    progress_bar->setValue(50);
}

