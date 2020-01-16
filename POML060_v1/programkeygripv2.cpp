#include "programkeygripv2.h"
#include "ui_programkeygripv2.h"
#include "stageprogram.h"

ProgramKeyGripV2::ProgramKeyGripV2(QString patientLabel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramKeyGripV2)
{
    ui->setupUi(this);
    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");

    pLabel = patientLabel;


}
ProgramKeyGripV2::~ProgramKeyGripV2()
{
    delete ui;
}

// small helper to give us the distance
int ProgramKeyGripV2::distance(QPoint x1, QPoint x2)
{
    return abs(x2.y() - x1.y());
}


void ProgramKeyGripV2::paintEvent(QPaintEvent *e)
{
    QPainter painter1(this);
    //this->setCursor(QCursor(Qt::WaitCursor));

    QPen pointpen(Qt::black, Qt::RoundJoin);
    pointpen.setWidth(5);
    QPen limitpen(Qt::black,20, Qt::DashLine);
    limitpen.setWidth(1);
    QPen linepen1(Qt::darkRed);
    linepen1.setWidth(2);
    QPen linepen2(Qt::darkMagenta);
    linepen2.setWidth(2);
    QPen linepen3(Qt::darkBlue);
    linepen3.setWidth(2);
    QPen linepen4(Qt::darkGreen);
    linepen4.setWidth(2);


    painter1.setPen(linepen1);
    painter1.drawLine(p11,p12);
    painter1.drawLine(p12,p13);
    painter1.drawLine(p13,p14);
    painter1.setPen(pointpen);
    painter1.drawPoint(p11);
    painter1.drawPoint(p12);
    painter1.drawPoint(p13);
    painter1.drawPoint(p14);


    painter1.setPen(linepen2);
    painter1.drawLine(p21,p22);
    painter1.drawLine(p22,p23);
    painter1.drawLine(p23,p24);
    painter1.setPen(pointpen);
    painter1.drawPoint(p21);
    painter1.drawPoint(p22);
    painter1.drawPoint(p23);
    painter1.drawPoint(p24);

    painter1.setPen(limitpen);
    painter1.drawLine(handOpen1, handOpen2);
    painter1.drawLine(fingerFlexion1, fingerFlexion2);
    painter1.drawLine(thumbAdduct1, thumbAdduct2);
    painter1.drawLine(handOpen11, handOpen22);
    painter1.drawLine(handOpen111, handOpen222);

    painter1.setPen(linepen3);
    painter1.drawLine(p31,p32);
    painter1.drawLine(p32,p33);
    painter1.drawLine(p33,p34);
    painter1.setPen(pointpen);
    painter1.drawPoint(p31);
    painter1.drawPoint(p32);
    painter1.drawPoint(p33);
    painter1.drawPoint(p34);

    painter1.setPen(linepen4);
    painter1.drawLine(p41,p42);
    painter1.drawLine(p42,p43);
    painter1.drawLine(p43,p44);
    painter1.drawLine(p44,p45);
    painter1.drawLine(p45,p46);
    painter1.drawLine(p46,p47);
    painter1.drawLine(p47,p48);
    painter1.setPen(pointpen);
    painter1.drawPoint(p41);
    painter1.drawPoint(p42);
    painter1.drawPoint(p43);
    painter1.drawPoint(p44);
    painter1.drawPoint(p45);
    painter1.drawPoint(p46);
    painter1.drawPoint(p47);
    painter1.drawPoint(p48);



    qDebug()<<"Values of p1.y and p2.y:"<<p12.y()<<"and"<<p22.y();
}

// when user clicks
void ProgramKeyGripV2::mousePressEvent(QMouseEvent *event)
{
    checked1 = ui->radioButton_one->isChecked();
    checked2 = ui->radioButton_two->isChecked();
    checked3 = ui->radioButton_three->isChecked();
    checked4 = ui->radioButton_four->isChecked();
    checked5 = ui->radioButton_five->isChecked();


   QPoint mp = event->pos(); // where is mouse


   // test if we hit the line. give user 10 pixels slack as its hard to hit one pixel
   if (distance ( mp, p12) < 20 && ( mp.x() > p12.x() && mp.x() < p13.x() ) && checked1) {
       dragging1 = true; // flag we are dragging
       CurPoint1 = &p12;
       CurPoint2 = &p13;

      // this->setCursor(QCursor(Qt::DragMoveCursor));

     //  ui->radioButton_one->setText("Segment 1   Value:"+QString::number(event->y()));

   }
   else if (distance ( mp, p22) < 20 && ( mp.x() > p22.x() && mp.x() < p23.x() ) && checked2) {
       dragging2 = true;
       CurPoint1 = &p22;
       CurPoint2 = &p23;

   }
   else if (distance ( mp, p32) < 20 && ( mp.x() > p32.x() && mp.x() < p33.x() ) && checked3) {
       dragging3 = true;
       CurPoint1 = &p32;
       CurPoint2 = &p33;

   }

   else if (distance ( mp, p42) < 20 && ( mp.x() > p42.x() && mp.x() < p43.x() ) && checked4) {
       dragging4 = true;
       CurPoint1 = &p42;
       CurPoint2 = &p43;

   }

   else if (distance ( mp, p46) < 20 && ( mp.x() > p46.x() && mp.x() < p47.x() ) && checked5) {
       dragging5 = true;
       CurPoint1 = &p46;
       CurPoint2 = &p47;

   }
}

void ProgramKeyGripV2::mouseReleaseEvent(QMouseEvent *event)
{
    dragging1 = false; // if user release mouse we are not draggign anymore
    dragging2 = false;
    dragging3 = false;
    dragging4 = false;
    dragging5 = false;
}

// then when mouse move
void ProgramKeyGripV2::mouseMoveEvent(QMouseEvent *event)
{
   // If we are dragging, call your normal slider changed function to update your points.


    if (dragging1 && checked1)
    {
       if(event->y() > 150 && event->y() < 250){
         changeP1value(event->y());
       }
        if(checked1){
           ui->radioButton_one->setText("FDS+FDP Val:"+QString::number(CurPoint1->y()+100));
           ui->label_one->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
           ui->label_one->setText(QString::number(CurPoint1->y()));
           //ui->label_dragimg->setGeometry(QString::number(CurPoint1->x()).toInt()+80,QString::number(CurPoint1->y()).toInt()-10,31,21);
          }
    }
    else if(dragging2 && checked2)
    {
      if(event->y() > 160 && event->y() < 260){
        changeP1value(event->y());
       }

      if(checked2){
      ui->radioButton_two->setText("Ulna nerve Val:"+QString::number(CurPoint1->y()));
      ui->label_two->setGeometry(QString::number(CurPoint1->x()).toInt()+20,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_two->setText(QString::number(CurPoint1->y()));
      }
    }
    else if(dragging3 && checked3)
    {
      if(event->y() > 160 && event->y() < 260){
        changeP1value(event->y());
       }

      if(checked3){
      ui->radioButton_three->setText("FPL or ADP Val:"+QString::number(CurPoint1->y()));
      ui->label_three->setGeometry(QString::number(CurPoint1->x()).toInt()+40,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_three->setText(QString::number(CurPoint1->y()));
      }
    }
    else if(dragging4 && checked4)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(checked4){
      ui->radioButton_four->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_four->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_four->setText(QString::number(CurPoint1->y()));
      }
    }
    else if(dragging5 && checked5)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(checked5){
      ui->radioButton_five->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_five->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_five->setText(QString::number(CurPoint1->y()));
      }
    }
    else
       update();



}

void ProgramKeyGripV2::changeP1value(int value)
{

    CurPoint1->setY(value);
    CurPoint2->setY(value);

    //QThread::msleep(200);
    update();

}

void ProgramKeyGripV2::on_pushButton_back_keypro_clicked()
{
    stageProgram *backprogram;
    this->close();
    backprogram = new stageProgram(pLabel,this);
    backprogram -> show();
}
