#ifndef STAGEPROGRAM_H
#define STAGEPROGRAM_H

#include <QMainWindow>

namespace Ui {
class stageProgram;
}

class stageProgram : public QMainWindow
{
    Q_OBJECT

public:
    explicit stageProgram(QWidget *parent = nullptr);
    ~stageProgram();


public slots:

    void setCurrOnChannelOne(QString);
    void setCurrOnChannelTwo(QString);
    void setCurrOnChannelThree(QString);
    void setCurrOnChannelFour(QString);
    void setCurrOnChannelFive(QString);




private slots:
    void on_pushButton_stimStart_clicked();

    void on_pushButton_stimStop_clicked();

private:
    Ui::stageProgram *ui;
};

#endif // STAGEPROGRAM_H
