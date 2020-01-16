#ifndef PROGRAMOPENHAND_H
#define PROGRAMOPENHAND_H

#include <QMainWindow>

namespace Ui {
class ProgramOpenHand;
}

class ProgramOpenHand : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProgramOpenHand(QString, QWidget *parent = nullptr);
    ~ProgramOpenHand();
        QString pLabel;

private slots:
    void on_pushButton_back_keypro_clicked();

private:
    Ui::ProgramOpenHand *ui;
};

#endif // PROGRAMOPENHAND_H
