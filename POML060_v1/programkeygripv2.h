#ifndef PROGRAMKEYGRIPV2_H
#define PROGRAMKEYGRIPV2_H

#include <QMainWindow>
#include<QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class ProgramKeyGripV2; }
QT_END_NAMESPACE

class ProgramKeyGripV2 : public QMainWindow
{
    Q_OBJECT

public:
    ProgramKeyGripV2(QWidget *parent = nullptr);
    ~ProgramKeyGripV2();
    void paintEvent(QPaintEvent *e);



public slots:

        void changeP1value(int);

signals:

        void buttonOne(bool);
        void buttonTwo(bool);


private:
    Ui::ProgramKeyGripV2 *ui;
     bool dragging1 = false; // status var to see if we are dragging
     bool dragging2 = false;
     bool dragging3 = false;
     bool dragging4 = false;
     bool dragging5 = false;

         QPoint p41 = QPoint(150,300);
         QPoint p42 = QPoint(170,250);
         QPoint p43 = QPoint(250,250);
         QPoint p44 = QPoint(270,280);
         QPoint p45 = QPoint(450,280);
         QPoint p46 = QPoint(470,250);
         QPoint p47 = QPoint(550,250);
         QPoint p48 = QPoint(570,300);


         QPoint p11 = QPoint(230,300);
         QPoint p12 = QPoint(280,170);
         QPoint p13 = QPoint(450,170);
         QPoint p14 = QPoint(500,300);


         QPoint p31 = QPoint(300,300);
         QPoint p32 = QPoint(320,200);
         QPoint p33 = QPoint(450,200);
         QPoint p34 = QPoint(470,300);

         QPoint p21 = QPoint(300,300);
         QPoint p22 = QPoint(320,240);
         QPoint p23 = QPoint(450,240);
         QPoint p24 = QPoint(470,300);



         QPoint limitUp1 = QPoint(100,50);
         QPoint limitUp2 = QPoint(200,50);
         QPoint limitDown1 = QPoint(100,150);
         QPoint limitDown2 = QPoint(200,150);

         QPoint *CurPoint1=nullptr;
         QPoint *CurPoint2=nullptr;
         QPoint *CurPoint3=nullptr;
         QPoint *CurPoint4=nullptr;

         bool checked1;
         bool checked2;
         bool checked3;
         bool checked4;
         bool checked5;

protected:
   // we then override / make our own of these function to track mouse movement and clicks
         void mousePressEvent(QMouseEvent *event) ;
         void mouseReleaseEvent(QMouseEvent *event) ;
         void mouseMoveEvent(QMouseEvent *event) ;
private slots:

         void on_pushButton_back_keypro_clicked();
};

#endif // PROGRAMKEYGRIPV2_H
