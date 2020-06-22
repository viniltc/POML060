#include "programpalmergrasp.h"
#include "ui_programpalmergrasp.h"
#include "stageprogram.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "manageconfigfile.h"

ProgramPalmerGrasp::ProgramPalmerGrasp(QString patientLabel,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramPalmerGrasp)
{
    ui->setupUi(this);
    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");

    pLabel = patientLabel;


    QDomDocument document;

    QString configfilename = "config_keygrip_test_"+pLabel;
    QString xmlName = pLabel;

    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
   // QString txtWritePath = ":/resources/"+configfilename+".txt";
    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configfilename+".txt";

    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

    }
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();

    if (!CurrentNodeVal.isNull())
    {

        currOneStored = root.elementsByTagName("CH1").at(0).firstChild().nodeValue().toFloat()*1000;
        currTwoStored = root.elementsByTagName("CH2").at(0).firstChild().nodeValue().toFloat()*1000;
        currThreeStored = root.elementsByTagName("CH3").at(0).firstChild().nodeValue().toFloat()*1000;
        currFourStored = root.elementsByTagName("CH4").at(0).firstChild().nodeValue().toFloat()*1000;
        currFiveStored = root.elementsByTagName("CH5").at(0).firstChild().nodeValue().toFloat()*1000;


        ui->label_currOne->setText(QString("Ch 1 (EDC): %1 mA").arg(currOneStored/m_currentDiv));
        ui->label_currTwo->setText(QString("Ch 2 (FDS): %1 mA").arg(currOneStored/m_currentDiv));
        ui->label_currThree->setText(QString("Ch 3 (Ulna): %1 mA").arg( currThreeStored/m_currentDiv));
        ui->label_currFour->setText(QString("Ch 4 (ADP): %1 mA").arg(currFourStored/m_currentDiv));
        ui->label_currFive->setText(QString("Ch 5 (---): %1 mA").arg(currFiveStored/m_currentDiv));


    }




    btnGrp = new QButtonGroup(this);

    btnGrp->addButton(ui->btn1, 1);
    btnGrp->addButton(ui->btn2, 2);
    btnGrp->addButton(ui->btn3, 3);
    btnGrp->addButton(ui->btn4, 4);

   ui->btn_nextPhase->setEnabled(false);
   ui->comboBox_1->setEnabled(false);
   ui->comboBox_2->setEnabled(false);
   ui->comboBox_3->setEnabled(false);
   ui->comboBox_4->setEnabled(false);
   ui->pushButton_keyGrip->setEnabled(false);

   ui->radioButton_one->setEnabled(false); // FPS
   ui->radioButton_two->setEnabled(false); // Ulna
   ui->radioButton_three->setEnabled(false); // ADP
   ui->radioButton_four->setEnabled(false); // EDC seg 1
   ui->radioButton_five->setEnabled(false); // EDC seg 3
   ui->radioButton_six->setEnabled(false); // EDC seg 2


   ui->comboBox_1->addItem("200ms", QVariant(0.2));
   ui->comboBox_1->addItem("500ms", QVariant(0.5));
   ui->comboBox_1->addItem("1000ms", QVariant(1));
   ui->comboBox_1->addItem("1500ms", QVariant(1.5));
   ui->comboBox_1->addItem("2000ms", QVariant(2));

   ui->comboBox_2->addItem("200ms", QVariant(0.2));
   ui->comboBox_2->addItem("500ms", QVariant(0.5));
   ui->comboBox_2->addItem("1000ms", QVariant(1));
   ui->comboBox_2->addItem("1500ms", QVariant(1.5));
   ui->comboBox_2->addItem("2000ms", QVariant(2));

   ui->comboBox_3->addItem("200ms", QVariant(0.2));
   ui->comboBox_3->addItem("500ms", QVariant(0.5));
   ui->comboBox_3->addItem("1000ms", QVariant(1));
   ui->comboBox_3->addItem("1500ms", QVariant(1.5));
   ui->comboBox_3->addItem("2000ms", QVariant(2));

   ui->comboBox_4->addItem("200ms", QVariant(0.2));
   ui->comboBox_4->addItem("500ms", QVariant(0.5));
   ui->comboBox_4->addItem("1000ms", QVariant(1));
   ui->comboBox_4->addItem("1500ms", QVariant(1.5));
   ui->comboBox_4->addItem("2000ms", QVariant(2));

   m_currentBtn = 0;

    connect(&api, &tetra_grip_api::tetraGripEvent,this, &ProgramPalmerGrasp::keyGripPhaseEventHandler);

    connect(ui->btn_nextPhase, &QPushButton::clicked, this, &ProgramPalmerGrasp::nextBtn);

    connect(this, &ProgramPalmerGrasp::buttonChanged, this, &ProgramPalmerGrasp::paintBtn);
    connect(this, &ProgramPalmerGrasp::pulseWidthValue, this, &ProgramPalmerGrasp::getPWValue);
    connect(this, &ProgramPalmerGrasp::pulseWidthValue, this, &ProgramPalmerGrasp::nextBtn);
    connect(this, &ProgramPalmerGrasp::pulseWidthValue, this, &ProgramPalmerGrasp::prevBtn);

    //slot
//    connect(ui->pushButton_stimStart, &QPushButton::clicked, this, &ProgramKeyGripV2::startStopTimer);
//    connect(ui->pushButton_stimStop, &QPushButton::clicked, this, &ProgramKeyGripV2::resetTimer);

//    QTimer *timer2 = new QTimer(this);
//    connect(timer2, SIGNAL(timeout()), this, SLOT(onTimeout()));
//    timer2->start(10);


    //QString configfilename1 = "config_keygrip_test_"+pLabel;
  //  QString txtWritePath1 = QCoreApplication::applicationDirPath()+"/data/config_file/" + configfilename1 + ".txt";
    //QFile f(":/resources/config_keygrip_v2.txt");
    QFile f(txtWritePath);
    if(!f.open(QFile::ReadOnly))
         {
             QMessageBox::information(0, "config file error", f.errorString());
         }
    else
         {
             QByteArray config = f.readAll();
             tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

         }




}
ProgramPalmerGrasp::~ProgramPalmerGrasp()
{
    delete ui;
}

// small helper to give us the distance
int ProgramPalmerGrasp::distance(QPoint x1, QPoint x2)
{
    return abs(x2.y() - x1.y());
}


void ProgramPalmerGrasp::paintEvent(QPaintEvent *e)
{
    QPainter painter1(this);

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
    QPen linepen5(Qt::darkGray);
    linepen5.setWidth(2);


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

    painter1.setPen(linepen5);
    painter1.drawLine(p51,p52);
    painter1.drawLine(p52,p53);
    painter1.drawLine(p53,p54);
    painter1.setPen(pointpen);
    painter1.drawPoint(p51);
    painter1.drawPoint(p52);
    painter1.drawPoint(p53);
    painter1.drawPoint(p54);



    qDebug()<<"Values of p1.y and p2.y:"<<p12.y()<<"and"<<p22.y();
}

// when user clicks
void ProgramPalmerGrasp::mousePressEvent(QMouseEvent *event)
{
    FDS_checked = ui->radioButton_one->isChecked();
    Ulna_checked = ui->radioButton_two->isChecked();
    ADP_checked = ui->radioButton_three->isChecked();
    EDC_Seg1_checked = ui->radioButton_four->isChecked();
    EDC_Seg2_checked = ui->radioButton_six->isChecked();
    EDC_Seg3_checked = ui->radioButton_five->isChecked();


   QPoint mp = event->pos(); // where is mouse


   // test if we hit the line. give user 10 pixels slack as its hard to hit one pixel
   if (distance ( mp, p12) < 20 && ( mp.x() > p12.x() && mp.x() < p13.x() ) && FDS_checked) {
       FDS_dragging = true; // flag we are dragging
       CurPoint1 = &p12;
       CurPoint2 = &p13;

      this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

     //  ui->radioButton_one->setText("Segment 1   Value:"+QString::number(event->y()));

   }
   else if (distance ( mp, p22) < 20 && ( mp.x() > p22.x() && mp.x() < p23.x() ) && Ulna_checked) {
       Ulna_dragging = true;
       CurPoint1 = &p22;
       CurPoint2 = &p23;

       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }
   else if (distance ( mp, p32) < 20 && ( mp.x() > p32.x() && mp.x() < p33.x() ) && ADP_checked) {
       ADP_dragging = true;
       CurPoint1 = &p32;
       CurPoint2 = &p33;
       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }

   else if (distance ( mp, p42) < 20 && ( mp.x() > p42.x() && mp.x() < p43.x() ) && EDC_Seg1_checked) {
       EDC_Seg1_dragging = true;
       CurPoint1 = &p42;
       CurPoint2 = &p43;
       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }

   else if (distance ( mp, p46) < 20 && ( mp.x() > p46.x() && mp.x() < p47.x() ) && EDC_Seg3_checked) {
       EDC_Seg3_dragging = true;
       CurPoint1 = &p46;
       CurPoint2 = &p47;
       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }
   else if (distance ( mp, p44) < 20 && ( mp.x() > p44.x() && mp.x() < p45.x() ) && EDC_Seg2_checked) {
       EDC_Seg2_dragging = true;
       CurPoint1 = &p44;
       CurPoint2 = &p45;
       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }
}

void ProgramPalmerGrasp::mouseReleaseEvent(QMouseEvent *event)
{
    FDS_dragging = false; // if user release mouse we are not draggign anymore
    Ulna_dragging = false;
    ADP_dragging = false;
    EDC_Seg1_dragging = false;
    EDC_Seg3_dragging = false;
    EDC_Seg2_dragging = false;
    this->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
}

// then when mouse move
void ProgramPalmerGrasp::mouseMoveEvent(QMouseEvent *event)
{
   // If we are dragging, call your normal slider changed function to update your points.


    if (FDS_dragging && FDS_checked)
    {
       if(event->y() > 150 && event->y() < 280){
         changeP1value(event->y());
       }
        if(FDS_checked){
          // ui->radioButton_one->setText("FDS+FDP Val:"+QString::number(CurPoint1->y()));
           ui->label_FDS->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
           ui->label_FDS->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
           //ui->label_dragimg->setGeometry(QString::number(CurPoint1->x()).toInt()+80,QString::number(CurPoint1->y()).toInt()-10,31,21);
          }
    }
    else if(Ulna_dragging && Ulna_checked)
    {
      if(event->y() > 150 && event->y() < 280){
        changeP1value(event->y());
       }

      if(Ulna_checked){
    //  ui->radioButton_two->setText("Ulna nerve Val:"+QString::number(CurPoint1->y()));
      ui->label_Ulna->setGeometry(QString::number(CurPoint1->x()).toInt()+20,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_Ulna->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      }
    }
    else if(ADP_dragging && ADP_checked)
    {
      if(event->y() > 150 && event->y() < 280){
        changeP1value(event->y());
       }

      if(ADP_checked){
     // ui->radioButton_three->setText("FPL or ADP Val:"+QString::number(CurPoint1->y()));
      ui->label_ADP->setGeometry(QString::number(CurPoint1->x()).toInt()+40,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_ADP->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      }
    }
    else if(EDC_Seg1_dragging && EDC_Seg1_checked)
    {
      if(event->y() > 150 && event->y() < 280){
        changeP1value(event->y());
       }

      if(EDC_Seg1_checked){
    //  ui->radioButton_four->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_EDC1->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_EDC1->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
     // ui->label_7->setText(QString::number(adjust_Ramp_Step_size(CurPoint1->y(),ramp_phase1)));
      }
    }
    else if(EDC_Seg3_dragging && EDC_Seg3_checked)
    {
      if(event->y() > 150 && event->y() < 280){
        changeP1value(event->y());
       }

      if(EDC_Seg3_checked){
    //  ui->radioButton_five->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_EDC3->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_EDC3->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");

      }
    }

    else if(EDC_Seg2_dragging && EDC_Seg2_checked)
    {
      if(event->y() > 150 && event->y() < 280){
        changeP1value(event->y());
       }

      if(EDC_Seg2_checked){
     // ui->radioButton_six->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_EDC2->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_EDC2->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      }
    }
    else
       update();

}

void ProgramPalmerGrasp::changeP1value(int value)
{

    CurPoint1->setY(value);
    CurPoint2->setY(value);

    //QThread::msleep(200);
    update();
    emit pulseWidthValue(adjust_PW_range(CurPoint1->y()));
    pw_value = adjust_PW_range(CurPoint1->y());

}

void ProgramPalmerGrasp::keyGripPhaseEventHandler(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_ACTIVITY_STATUS: // Line no: 714,  stim_gui_protocol_decode.c

            if(value==true)
            {
                 ui->pushButton_stimStop->setText("Stop");
                 ui->pushButton_stimStart->setText("Running");
            }
            else
            {
                 ui->pushButton_stimStop->setText("Stopped");
                 ui->pushButton_stimStart->setText("Start");
                 ui->btn_nextPhase->setEnabled(false);
            }
            break;
        }
    }
}

void ProgramPalmerGrasp::nextBtn(int pwvalue)
{
    m_currentBtn++;
    if(m_currentBtn > btnGrp->buttons().size())
    {
        m_currentBtn = 0;
        phaseOver = true;
        getRampStepSize();
       // saveToXMLFile();
        //resetTimer();
    }

    emit buttonChanged(m_currentBtn, pwvalue);
}

void ProgramPalmerGrasp::prevBtn(int pwvalue)
{
    m_currentBtn--;
     if(m_currentBtn < 1)
     {
         m_currentBtn = btnGrp->buttons().size();
     }

  emit buttonChanged(m_currentBtn, pwvalue);

}

void ProgramPalmerGrasp::paintBtn(int id, int pwvalue)
{
    if(EDC_Seg1_checked)
    {
        PW_EDC1 = pwvalue;
        tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 1, PW_EDC1);

        tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 1, adjust_Ramp_Step_size(pwvalue,ramp_phase1));

    }
    else if(EDC_Seg2_checked)
    {
        PW_EDC2 = pwvalue;
        tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 2, pwvalue);
        tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 3, pwvalue);

        tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 2, adjust_Ramp_Step_size(pwvalue,ramp_phase2));
        tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 3, adjust_Ramp_Step_size(pwvalue,ramp_phase3));

    }
    else if(EDC_Seg3_checked)
    {
        tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 4, pwvalue);

        tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 4, adjust_Ramp_Step_size(pwvalue,ramp_phase4));
    }
    else if(Ulna_checked)
    {
       tetra_grip_api::set_stimulation_target_pulse_width( m_channelThree, 3, pwvalue);

       tetra_grip_api::set_stimulation_ramp_rate( m_channelThree, 3, adjust_Ramp_Step_size(pwvalue,ramp_phase3));

    }
    else if(FDS_checked)
    {
       tetra_grip_api::set_stimulation_target_pulse_width( m_channelTwo, 2, pwvalue);
       tetra_grip_api::set_stimulation_target_pulse_width( m_channelTwo, 3, pwvalue);

       tetra_grip_api::set_stimulation_ramp_rate( m_channelTwo, 2, adjust_Ramp_Step_size(pwvalue,ramp_phase2));
       tetra_grip_api::set_stimulation_ramp_rate( m_channelTwo, 3, adjust_Ramp_Step_size(pwvalue,ramp_phase3));

    }
    else if(ADP_checked)
    {
       tetra_grip_api::set_stimulation_target_pulse_width( m_channelFour, 3, pwvalue);

       tetra_grip_api::set_stimulation_ramp_rate( m_channelFour, 3, adjust_Ramp_Step_size(pwvalue,ramp_phase3));
    }

    pw_value = pwvalue;



    qDebug()<<"Button size is"<< btnGrp->buttons().size() << "And current Button is"<< id;

    QString StyleSheetOn("background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                         "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)");
    QString StyleSheetOff("border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                          "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                  "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                          "color: #0000;");

     if(id==1)
     {
        // ui->label_pwvalue->setText(QString("From 1, %1").arg(pwvalue));
        // ui->btn1->setStyleSheet("background-color: green");
         ui->btn1->setStyleSheet(StyleSheetOn);
         ui->btn2->setStyleSheet(StyleSheetOff);
         ui->btn3->setStyleSheet(StyleSheetOff);
         ui->btn4->setStyleSheet(StyleSheetOff);
         ui->pushButton_keyGrip->setEnabled(false);

         if(EDC_Seg1_checked)
         {
            PW_phase1_EDC = pwvalue;
            //ramp_stepsize_phase1_EDC = adjust_Ramp_Step_size( PW_phase1_EDC, ramp_phase1);

         }

         ui->radioButton_one->setEnabled(false); // FPS
         ui->radioButton_two->setEnabled(false); // Ulna
         ui->radioButton_three->setEnabled(false); // ADP
         ui->radioButton_four->setEnabled(true); // EDC seg 1
         ui->radioButton_five->setEnabled(false); // EDC seg 3
         ui->radioButton_six->setEnabled(false); // EDC seg 2

     }

     else if(id==2)
     {
        // ui->label_pwvalue->setText(QString("From 2, %1").arg(pwvalue));
         ui->btn1->setStyleSheet(StyleSheetOff);
         ui->btn2->setStyleSheet(StyleSheetOn);
         ui->btn3->setStyleSheet(StyleSheetOff);
         ui->btn4->setStyleSheet(StyleSheetOff);
         ui->pushButton_keyGrip->setEnabled(false);

         ui->radioButton_one->setEnabled(true); // FPS
         ui->radioButton_two->setEnabled(false); // Ulna
         ui->radioButton_three->setEnabled(false); // ADP
         ui->radioButton_four->setEnabled(false); // EDC seg 1
         ui->radioButton_five->setEnabled(false); // EDC seg 3
         ui->radioButton_six->setEnabled(true); // EDC seg 2

         if(EDC_Seg2_checked)
         {
            PW_phase2_EDC = pwvalue;
            //ramp_stepsize_phase2_EDC = adjust_Ramp_Step_size( PW_phase2_EDC, ramp_phase2);
         }
         else if(FDS_checked)
         {
            PW_phase2_FDS = pwvalue;
           // ramp_stepsize_phase2_FDS = adjust_Ramp_Step_size( PW_phase2_FDS, ramp_phase2);
         }
     }

    else if(id==3)
    {
         //ui->label_pwvalue->setText(QString("From 3, %1").arg(pwvalue));
         ui->btn1->setStyleSheet(StyleSheetOff);
         ui->btn2->setStyleSheet(StyleSheetOff);
         ui->btn3->setStyleSheet(StyleSheetOn);
         ui->btn4->setStyleSheet(StyleSheetOff);
         ui->pushButton_keyGrip->setEnabled(false);

         ui->radioButton_one->setEnabled(true); // FPS
         ui->radioButton_two->setEnabled(true); // Ulna
         ui->radioButton_three->setEnabled(true); // ADP
         ui->radioButton_four->setEnabled(false); // EDC seg 1
         ui->radioButton_five->setEnabled(false); // EDC seg 3
         ui->radioButton_six->setEnabled(true); // EDC seg 2

         if(EDC_Seg2_checked)
         {
            //PW_phase3_EDC = pwvalue;
            PW_phase3_EDC = PW_phase2_EDC;
            //ramp_stepsize_phase3_EDC = adjust_Ramp_Step_size( PW_phase3_EDC, ramp_phase3);
         }
         else if(FDS_checked)
         {
           // PW_phase3_FDS = pwvalue;
            PW_phase3_FDS = PW_phase2_FDS;
           // ramp_stepsize_phase3_FDS = adjust_Ramp_Step_size( PW_phase3_FDS, ramp_phase3);
         }
         else if(Ulna_checked)
         {
            PW_phase3_Ulna = pwvalue;
           // ramp_stepsize_phase3_Ulna = adjust_Ramp_Step_size( PW_phase3_Ulna, ramp_phase3);
         }
         else if(ADP_checked)
         {
            PW_phase3_ADP = pwvalue;
           // ramp_stepsize_phase3_ADP = adjust_Ramp_Step_size( PW_phase3_ADP, ramp_phase3);
         }
    }

    else if(id==4)
    {
        // ui->label_pwvalue->setText(QString("From 4, %1").arg(pwvalue));
         ui->btn1->setStyleSheet(StyleSheetOff);
         ui->btn2->setStyleSheet(StyleSheetOff);
         ui->btn3->setStyleSheet(StyleSheetOff);
         ui->btn4->setStyleSheet(StyleSheetOn);
         ui->pushButton_keyGrip->setEnabled(false);

         ui->radioButton_one->setEnabled(false); // FPS
         ui->radioButton_two->setEnabled(false); // Ulna
         ui->radioButton_three->setEnabled(false); // ADP
         ui->radioButton_four->setEnabled(false); // EDC seg 1
         ui->radioButton_five->setEnabled(true); // EDC seg 3
         ui->radioButton_six->setEnabled(false); // EDC seg 2


         if(EDC_Seg3_checked)
         {
            PW_phase4_EDC = pwvalue;
           // ramp_stepsize_phase4_EDC = adjust_Ramp_Step_size( PW_phase4_EDC, ramp_phase4);
         }
    }

    else
    {
         ui->btn1->setStyleSheet(StyleSheetOff);
         ui->btn2->setStyleSheet(StyleSheetOff);
         ui->btn3->setStyleSheet(StyleSheetOff);
         ui->btn4->setStyleSheet(StyleSheetOff);
         ui->pushButton_keyGrip->setEnabled(true);
         ui->pushButton_save->setEnabled(true);

         ui->radioButton_one->setEnabled(false); // FPS
         ui->radioButton_two->setEnabled(false); // Ulna
         ui->radioButton_three->setEnabled(false); // ADP
         ui->radioButton_four->setEnabled(false); // EDC seg 1
         ui->radioButton_five->setEnabled(false); // EDC seg 3
         ui->radioButton_six->setEnabled(false); // EDC seg 2


    }

}

void ProgramPalmerGrasp::getPWValue(int)
{

}

int ProgramPalmerGrasp::adjust_PW_range(int value)
{
    //int result = ((280-value)*3)+1 ; // 0-180us
    //int result = (1224-(4.2*value)) ; // 50-300us
    int result = (588-(1.9*value)) ; // 50-300us



    if (result == 0)
         return -1;
    else
         return result;
}

int ProgramPalmerGrasp::adjust_Ramp_Step_size(int pwvalue, float rmpvalue)
{
    int rmp_step_size = 1000*(pwvalue/(rmpvalue*40));
    return rmp_step_size;
}

void ProgramPalmerGrasp::getRampStepSize()
{

}

void ProgramPalmerGrasp::on_pushButton_clicked()
{
    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &ProgramPalmerGrasp::keyGripPhaseEventHandler);
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,this);
    backprogram -> show();
}

void ProgramPalmerGrasp::on_btn_nextPhase_clicked()
{
  tetra_grip_api::send_event(0, 253);
}

void ProgramPalmerGrasp::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);
    ui->btn_nextPhase->setEnabled(true);
    ui->comboBox_1->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->comboBox_4->setEnabled(true);

    ui->radioButton_one->setEnabled(true);
    ui->radioButton_two->setEnabled(true);
    ui->radioButton_three->setEnabled(true);
    ui->radioButton_four->setEnabled(true);
    ui->radioButton_five->setEnabled(true);
    ui->radioButton_six->setEnabled(true);
}

void ProgramPalmerGrasp::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);

    m_currentBtn = 0;

    ui->btn_nextPhase->setEnabled(false);
    ui->comboBox_1->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);

    ui->radioButton_one->setEnabled(false);
    ui->radioButton_two->setEnabled(false);
    ui->radioButton_three->setEnabled(false);
    ui->radioButton_four->setEnabled(false);
    ui->radioButton_five->setEnabled(false);
    ui->radioButton_six->setEnabled(false);
    ui->pushButton_keyGrip->setEnabled(false);
    ui->pushButton_save->setEnabled(false);

    QString StyleSheetOff("border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                          "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                  "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                          "color: #0000;");

    ui->btn1->setStyleSheet(StyleSheetOff);
    ui->btn2->setStyleSheet(StyleSheetOff);
    ui->btn3->setStyleSheet(StyleSheetOff);
    ui->btn4->setStyleSheet(StyleSheetOff);
}

void ProgramPalmerGrasp::on_pushButton_save_clicked()
{

}

void ProgramPalmerGrasp::on_pushButton_keyGrip_clicked()
{

}

void ProgramPalmerGrasp::on_comboBox_1_currentIndexChanged(int index)
{
    float value = ui->comboBox_1->itemData(index).toFloat();
    ramp_phase1 = value;
}

void ProgramPalmerGrasp::on_comboBox_2_currentIndexChanged(int index)
{
    float value = ui->comboBox_2->itemData(index).toFloat();
    ramp_phase2 = value;
}

void ProgramPalmerGrasp::on_comboBox_3_currentIndexChanged(int index)
{
    float value = ui->comboBox_3->itemData(index).toFloat();
    ramp_phase3 = value;
}

void ProgramPalmerGrasp::on_comboBox_4_currentIndexChanged(int index)
{
    float value = ui->comboBox_4->itemData(index).toFloat();
    ramp_phase4 = value;
}
