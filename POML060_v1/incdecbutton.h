#ifndef INCDECBUTTON_H
#define INCDECBUTTON_H

#include <QWidget>

namespace Ui {
class IncDecButton;
}

class IncDecButton : public QWidget
{
    Q_OBJECT

public:
    explicit IncDecButton(QWidget *parent = nullptr);
    ~IncDecButton();
     double value =0.0;


private slots:

    void doIncrement();
    void doDecrement();

signals:
    void getValue(double);


private:

    double smallSteps = 0.01; // in 0.5 mA


    bool isPressed = false;

    Ui::IncDecButton *ui;
};

#endif // INCDECBUTTON_H
