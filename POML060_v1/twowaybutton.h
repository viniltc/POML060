#ifndef TWOWAYBUTTON_H
#define TWOWAYBUTTON_H

#include <QWidget>

namespace Ui {
class TwoWayButton;
}

class TwoWayButton : public QWidget
{
    Q_OBJECT

public:
    explicit TwoWayButton(QWidget *parent = nullptr);
    ~TwoWayButton();

private:
    Ui::TwoWayButton *ui;
};

#endif // TWOWAYBUTTON_H
