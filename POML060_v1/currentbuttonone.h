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

public slots:

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
    void getValue(uint8_t);
    void zeroCurrent(uint8_t);

private:
    Ui::CurrentButtonOne *ui;
    QTimer *timer_high, *timer_low;
    int timerTimeout;
     uint8_t value;
     unsigned int channel_number = 1;
};

#endif // CURRENTBUTTONONE_H
