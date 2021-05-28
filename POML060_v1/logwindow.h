#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>

namespace Ui {
class logwindow;
}

class logwindow : public QWidget
{
    Q_OBJECT

public:
    explicit logwindow(QString, QWidget *parent = nullptr);
    ~logwindow();
    QString pLabel;

private:
    Ui::logwindow *ui;
};

#endif // LOGWINDOW_H
