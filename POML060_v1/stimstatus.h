#ifndef STIMSTATUS_H
#define STIMSTATUS_H

#include <QWidget>

namespace Ui {
class stimstatus;
}

class stimstatus : public QWidget
{
    Q_OBJECT

public:
    explicit stimstatus(QWidget *parent = nullptr);
    ~stimstatus();

private:
    Ui::stimstatus *ui;
};

#endif // STIMSTATUS_H
