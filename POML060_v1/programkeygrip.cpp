#include "programkeygrip.h"
#include "ui_programkeygrip.h"


ProgramKeyGrip::ProgramKeyGrip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProgramKeyGrip)
{
    ui->setupUi(this);
    ui->verticalSlider_red->setRange(150,200);
    ui->verticalSlider_yellow->setRange(200,250);
    ui->verticalSlider_green->setRange(255,295);

    connect(ui->verticalSlider_red,   &QSlider::valueChanged, this, &ProgramKeyGrip::changeP1value);
    connect(ui->verticalSlider_yellow, &QSlider::valueChanged, this, &ProgramKeyGrip::changeP2value);
    connect(ui->verticalSlider_green, &QSlider::valueChanged, this, &ProgramKeyGrip::changeP3value);
    connect(ui->verticalSlider_blue, &QSlider::valueChanged, this, &ProgramKeyGrip::changeP4value);

    ui->verticalSlider_red->valueChanged(150);
    ui->verticalSlider_yellow->valueChanged(200);
    ui->verticalSlider_green->valueChanged(255);
    ui->verticalSlider_red->setInvertedAppearance(true);
    ui->verticalSlider_yellow->setInvertedAppearance(true);
    ui->verticalSlider_green->setInvertedAppearance(true);
}

ProgramKeyGrip::~ProgramKeyGrip()
{
    delete ui;
}


void ProgramKeyGrip::paintEvent(QPaintEvent *e)
{
    QPainter painter1(this);


    QPen pointpen(Qt::black);
    pointpen.setWidth(5);
    QPen linepen1(Qt::red);
    linepen1.setWidth(2);
    QPen linepen2(Qt::yellow);
    linepen2.setWidth(2);
    QPen linepen3(Qt::blue);
    linepen3.setWidth(2);
    QPen linepen4(Qt::green);
    linepen4.setWidth(2);



    p11.setX(400);
    p11.setY(300);
    p12.setX(450);
    //p2.setY(100);
    p13.setX(450);
   // p3.setY(100);
    p14.setX(600);
   // p4.setY(100);
    p15.setX(600);
    //p5.setY(100);
    p16.setX(650);
    p16.setY(300);



    painter1.setPen(linepen1);
    painter1.drawLine(p11,p12);
    painter1.drawLine(p13,p14);
    painter1.drawLine(p15,p16);
    painter1.setPen(pointpen);
    painter1.drawPoint(p11);
    painter1.drawPoint(p12);
    painter1.drawPoint(p13);
    painter1.drawPoint(p14);
    painter1.drawPoint(p15);
    painter1.drawPoint(p16);

    p21.setX(450);
    p21.setY(300);
    p22.setX(470);
    //p2.setY(100);
    p23.setX(470);
   // p3.setY(100);
    p24.setX(600);
   // p4.setY(100);
    p25.setX(600);
    //p5.setY(100);
    p26.setX(650);
    p26.setY(300);

    painter1.setPen(linepen2);
    painter1.drawLine(p21,p22);
    painter1.drawLine(p23,p24);
    painter1.drawLine(p25,p26);
    painter1.setPen(pointpen);
    painter1.drawPoint(p21);
    painter1.drawPoint(p22);
    painter1.drawPoint(p23);
    painter1.drawPoint(p24);
    painter1.drawPoint(p25);
    painter1.drawPoint(p26);

    p31.setX(450);
    p31.setY(300);
    p32.setX(470);
    //p2.setY(100);
    p33.setX(470);
   // p3.setY(100);
    p34.setX(600);
   // p4.setY(100);
    p35.setX(600);
    //p5.setY(100);
    p36.setX(650);
    p36.setY(300);

    painter1.setPen(linepen3);
    painter1.drawLine(p31,p32);
    painter1.drawLine(p33,p34);
    painter1.drawLine(p35,p36);
    painter1.setPen(pointpen);
    painter1.drawPoint(p31);
    painter1.drawPoint(p32);
    painter1.drawPoint(p33);
    painter1.drawPoint(p34);
    painter1.drawPoint(p35);
    painter1.drawPoint(p36);


    p41.setX(300);
    p41.setY(300);
    p42.setX(320);
    p42.setY(200);
    p43.setX(320);
    p43.setY(200);
    p44.setX(400);
    p44.setY(200);
    p45.setX(400);
    p45.setY(200);
    p46.setX(420);
    p46.setY(230);
    p47.setX(420);
    p47.setY(230);
    p48.setX(600);
    p48.setY(230);
    p49.setX(600);
    p49.setY(230);
    p410.setX(620);
    p410.setY(200);
    p411.setX(620);
    p411.setY(200);
    p412.setX(700);
    p412.setY(200);
    p413.setX(700);
    p413.setY(200);
    p414.setX(720);
    p414.setY(300);

    painter1.setPen(linepen4);
    painter1.drawLine(p41,p42);
    painter1.drawLine(p43,p44);
    painter1.drawLine(p45,p46);
    painter1.drawLine(p47,p48);
    painter1.drawLine(p49,p410);
    painter1.drawLine(p411,p412);
     painter1.drawLine(p413,p414);
    painter1.setPen(pointpen);
    painter1.drawPoint(p41);
    painter1.drawPoint(p42);
    painter1.drawPoint(p43);
    painter1.drawPoint(p44);
    painter1.drawPoint(p45);
    painter1.drawPoint(p46);
    painter1.drawPoint(p47);
    painter1.drawPoint(p48);
    painter1.drawPoint(p49);
    painter1.drawPoint(p410);
    painter1.drawPoint(p411);
    painter1.drawPoint(p412);
    painter1.drawPoint(p413);
    painter1.drawPoint(p414);


   qDebug()<< "value is p2y"<<p12.y();


}


void ProgramKeyGrip::changeP1value(int value)
{

    p12.setY(value);
    p13.setY(value);
    p14.setY(value);
    p15.setY(value);
    update();


}

void ProgramKeyGrip::changeP2value(int value)
{
    p22.setY(value);
    p23.setY(value);
    p24.setY(value);
    p25.setY(value);
    update();
}

void ProgramKeyGrip::changeP3value(int value)
{
    p32.setY(value);
    p33.setY(value);
    p34.setY(value);
    p35.setY(value);
    update();
}

void ProgramKeyGrip::changeP4value(int value)
{
    p42.setY(value);
    p43.setY(value);
    p44.setY(value);
    p45.setY(value);
    update();
}
