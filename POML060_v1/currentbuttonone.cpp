#include "currentbuttonone.h"
#include "ui_currentbuttonone.h"
#include <QTimer>
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"

CurrentButtonOne::CurrentButtonOne(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentButtonOne)
{
    ui->setupUi(this);

    value = 0;

    ui->label_currentValue->setText(QString("%1").arg(value));
    ui->label_currentValue->setAlignment(Qt::AlignCenter);

    timerTimeout = 0;
    timer_high = new QTimer(this);
    timer_low = new QTimer(this);

    connect(timer_high,&QTimer::timeout, this, &CurrentButtonOne::doIncrement);
    connect(timer_low, &QTimer::timeout, this, &CurrentButtonOne::doDecrement);
    connect(timer_high,&QTimer::timeout, this, &CurrentButtonOne::doMoreIncrement);
    connect(timer_low, &QTimer::timeout, this, &CurrentButtonOne::doMoreDecrement);


    connect(ui->pushButton_high, &QPushButton::pressed, this, &CurrentButtonOne::buttonPressed_high);
    connect(ui->pushButton_high, &QPushButton::released, this, &CurrentButtonOne::buttonReleased_high);

    connect(ui->pushButton_moreHigh, &QPushButton::pressed, this, &CurrentButtonOne::buttonPressed_moreHigh);
    connect(ui->pushButton_moreHigh, &QPushButton::released, this, &CurrentButtonOne::buttonReleased_moreHigh);

    connect(ui->pushButton_low, &QPushButton::pressed, this, &CurrentButtonOne::buttonPressed_low);
    connect(ui->pushButton_low, &QPushButton::released, this, &CurrentButtonOne::buttonReleased_low);

    connect(ui->pushButton_moreLow, &QPushButton::pressed, this, &CurrentButtonOne::buttonPressed_moreLow);
    connect(ui->pushButton_moreLow, &QPushButton::released, this, &CurrentButtonOne::buttonReleased_moreLow);


}

CurrentButtonOne::~CurrentButtonOne()
{
    delete ui;
}

void CurrentButtonOne::buttonPressed_high()
{
timerTimeout = 5000;
doIncrement();
}

void CurrentButtonOne::buttonReleased_high()
{
timer_high->stop();
}

void CurrentButtonOne::buttonPressed_moreHigh()
{
timerTimeout = 5000;
doMoreIncrement();
}

void CurrentButtonOne::buttonReleased_moreHigh()
{
timer_high->stop();
}

void CurrentButtonOne::buttonPressed_low()
{
timerTimeout = 5000;
doDecrement();
}

void CurrentButtonOne::buttonReleased_low()
{
timer_low->stop();
}
void CurrentButtonOne::buttonPressed_moreLow()
{
timerTimeout = 5000;
doMoreDecrement();
}

void CurrentButtonOne::buttonReleased_moreLow()
{
timer_low->stop();
}

void CurrentButtonOne::doIncrement()
{
++value;
ui->label_currentValue->setText(QString("%1").arg(value));
emit getValue(QString::number(value));
if(timerTimeout > 50)
timerTimeout = timerTimeout / 2;
timer_high->start(timerTimeout);
}

void CurrentButtonOne::doDecrement()

{
value--;
if(value<=0)
   value=0;
ui->label_currentValue->setText(QString("%1").arg(value));
emit getValue(QString::number(value));
if(timerTimeout > 50)
timerTimeout = timerTimeout / 2;
timer_low->start(timerTimeout);
}

void CurrentButtonOne::doMoreIncrement()
{
value=value+5;
ui->label_currentValue->setText(QString("%1").arg(value));
emit getValue(QString::number(value));
if(timerTimeout > 50)
timerTimeout = timerTimeout / 2;
timer_high->start(timerTimeout);
}

void CurrentButtonOne::doMoreDecrement()

{
value=value-5;
if(value<=0)
   value=0;
ui->label_currentValue->setText(QString("%1").arg(value));
emit getValue(QString::number(value));
if(timerTimeout > 50)
timerTimeout = timerTimeout / 2;
timer_low->start(timerTimeout);
}





