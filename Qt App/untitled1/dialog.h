#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QComboBox>


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

};
#endif // DIALOG_H
