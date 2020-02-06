#ifndef PROGRAMKEYGRIPV2_H
#define PROGRAMKEYGRIPV2_H

#include <QMainWindow>
#include<QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QPushButton>
#include "tetra_grip_api.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProgramKeyGripV2; }
QT_END_NAMESPACE

class ProgramKeyGripV2 : public QMainWindow
{
    Q_OBJECT

public:
    ProgramKeyGripV2(QString, QWidget *parent = nullptr);
    ~ProgramKeyGripV2();
    void paintEvent(QPaintEvent *e);

    int distance(QPoint, QPoint);
        QString pLabel;



public slots:

        void changeP1value(int);
        void keyGripPhaseEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
        void nextBtn(int pwvalue);
        void prevBtn(int pwvalue);
        void paintBtn(int id, int pwvalue);
        void getPWValue (int);

signals:

        void buttonOne(bool);
        void buttonTwo(bool);
        void buttonChanged(int id, int pwvalue);
        void pulseWidthValue(int);


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



     QPoint handOpen1 =    QPoint(150,300);
     QPoint handOpen2 =    QPoint(150,100);
     QPoint fingerFlexion1 =  QPoint(230,300);
     QPoint fingerFlexion2 =  QPoint(230,100);
     QPoint thumbAdduct1 = QPoint(300,300);
     QPoint thumbAdduct2 = QPoint(300,100);
     QPoint handOpen11 = QPoint(450,300);
     QPoint handOpen22 = QPoint(450,100);
     QPoint handOpen111 = QPoint(550,300);
     QPoint handOpen222 = QPoint(550,100);

         QPoint *CurPoint1=nullptr;
         QPoint *CurPoint2=nullptr;
         QPoint *CurPoint3=nullptr;
         QPoint *CurPoint4=nullptr;

         bool checked1;
         bool checked2;
         bool checked3;
         bool checked4;
         bool checked5;

         int m_currentBtn = 0;
         QButtonGroup *btnGrp = nullptr;


protected:
   // we then override / make our own of these function to track mouse movement and clicks
         void mousePressEvent(QMouseEvent *event) ;
         void mouseReleaseEvent(QMouseEvent *event) ;
         void mouseMoveEvent(QMouseEvent *event) ;
private slots:

         void on_pushButton_back_keypro_clicked();
         void on_pushButton_nextPhase_clicked();
         void on_pushButton_stimStart_clicked();
         void on_pushButton_stimStop_clicked();
};

#endif // PROGRAMKEYGRIPV2_H
