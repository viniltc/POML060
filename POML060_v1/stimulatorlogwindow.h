#ifndef STIMULATORLOGWINDOW_H
#define STIMULATORLOGWINDOW_H

#include <QWidget>

namespace Ui {
class StimulatorLogWindow;
}

class StimulatorLogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StimulatorLogWindow(QString ,QWidget *parent = nullptr);
    ~StimulatorLogWindow();
    QString pLabel;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::StimulatorLogWindow *ui;
};

#endif // STIMULATORLOGWINDOW_H
