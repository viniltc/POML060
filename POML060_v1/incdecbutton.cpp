#include "incdecbutton.h"
#include "ui_incdecbutton.h"
#include <QTimer>

IncDecButton::IncDecButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IncDecButton)
{
    ui->setupUi(this);


//    connect(timer_high,&QTimer::timeout, this, &IncDecButton::doIncrement);
//    connect(timer_low, &QTimer::timeout, this, &IncDecButton::doDecrement);



    connect(ui->pushButton, &QPushButton::pressed, this, &IncDecButton::doIncrement);
   // connect(ui->pushButton_2, &QPushButton::released, this, &IncDecButton::buttonReleased_high);
    connect(ui->pushButton, &QPushButton::pressed, this, &IncDecButton::doDecrement);
   // connect(ui->pushButton_2, &QPushButton::released, this, &IncDecButton::buttonReleased_low);
}

IncDecButton::~IncDecButton()
{
    delete ui;
}

void IncDecButton::doIncrement()
{
    value=value+smallSteps;
    emit getValue(value);
//    if(timerTimeout > timeoutValue)
//    timerTimeout = timerTimeout / 2;
//    timer_high->start(timerTimeout);


}

void IncDecButton::doDecrement()
{
    value=value-smallSteps;
    if(value<=0)
       value=0;
    emit getValue(value);
//    if(timerTimeout > timeoutValue)
//    timerTimeout = timerTimeout / 2;
//    timer_low->start(timerTimeout);


}

