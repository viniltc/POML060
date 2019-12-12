#ifndef CURRENTBUTTONONE_H
#define CURRENTBUTTONONE_H

#include <QWidget>

namespace Ui {
class CurrentButtonOne;
}

class CurrentButtonOne : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentButtonOne(QWidget *parent = nullptr);
    ~CurrentButtonOne();

private:
    Ui::CurrentButtonOne *ui;
};

#endif // CURRENTBUTTONONE_H
