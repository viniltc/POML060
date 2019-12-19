#ifndef PROGRAMKEYGRIP_H
#define PROGRAMKEYGRIP_H

#include <QMainWindow>
#include <QDebug>
#include<QDialog>
#include <QPainter>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>



namespace Ui {
class ProgramKeyGrip;
}

class ProgramKeyGrip : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProgramKeyGrip(QWidget *parent = nullptr);
    ~ProgramKeyGrip();
    void paintEvent(QPaintEvent *e);


signals:

    void needToRepaint();

public slots:

void changeP1value(int);
void changeP2value(int);
void changeP3value(int);
void changeP4value(int);

private:
    Ui::ProgramKeyGrip *ui;
    QPoint p11, p12, p13, p14, p15, p16;
    QPoint p21, p22, p23, p24, p25, p26;
    QPoint p31, p32, p33, p34, p35, p36;
    QPoint p41, p42, p43, p44, p45, p46, p47,p48,p49,p410,p411,p412,p413,p414,p415;
};

#endif // PROGRAMKEYGRIP_H
