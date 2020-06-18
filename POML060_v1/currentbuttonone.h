#ifndef CURRENTBUTTONONE_H
#define CURRENTBUTTONONE_H

#include <QWidget>

class Timer;

namespace Ui {
class CurrentButtonOne;
}

class CurrentButtonOne : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentButtonOne(QWidget *parent = nullptr);
    ~CurrentButtonOne();
      //int value = 0 ;
    int value;

public slots:

    void disableMe();


private slots:

    void buttonPressed_high();
    void buttonReleased_high();
    void buttonPressed_moreHigh();
    void buttonReleased_moreHigh();
    void buttonPressed_low();
    void buttonReleased_low();
    void buttonPressed_moreLow();
    void buttonReleased_moreLow();
    void doIncrement();
    void doDecrement();
    void doMoreIncrement();
    void doMoreDecrement();

signals:
    void getValue(unsigned int);
   // void zeroCurrent(unsigned int);

private:
    Ui::CurrentButtonOne *ui;
    QTimer *timer_high, *timer_low;
    int timerTimeout;

    unsigned int channel_number = 1;
    int smallSteps = 500; // in 0.5 mA
    int bigSteps = 5000; // in 5 mA
    int timeoutValue = 1000;

};

#endif // CURRENTBUTTONONE_H
