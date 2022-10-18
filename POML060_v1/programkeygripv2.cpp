#include "programkeygripv2.h"
#include "ui_programkeygripv2.h"
#include "stageprogram.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "manageconfigfile.h"

ProgramKeyGripV2::ProgramKeyGripV2(QString patientLabel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramKeyGripV2),
      watch(new Stopwatch())

{
    ui->setupUi(this);
    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");
    ui->btn0->setVisible(false); // to make phase0 button invisible
    ui->pushButton_save->setVisible(false);

   // ui->radioButton ->setChecked(true);
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
   ui->pushButton_keyGrip->setEnabled(true);

   ui->radioButton_one->setEnabled(false); // FPS
   ui->radioButton_two->setEnabled(false); // Ulna
   ui->radioButton_three->setEnabled(false); // ADP
   ui->radioButton_four->setEnabled(false); // EDC seg 1
   ui->radioButton_five->setEnabled(false); // EDC seg 3
   ui->radioButton_six->setEnabled(false); // EDC seg 2




   QComboBox* rampComboBoxes[] { ui->comboBox_1, ui->comboBox_2, ui->comboBox_3, ui->comboBox_4 };
   const double rampValues[] {0.2 , 0.5 , 1 , 1.5 , 2};
   for(auto* cb : rampComboBoxes)
   {
       for(auto val : rampValues)
       {
           cb->addItem(QString::number(val*1000) + "ms", QVariant(val));
           cb->setCurrentIndex(2); // default 1s
       }
   }


   m_currentBtn = 0;

   StyleSheetOn = "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                        "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)";
   StyleSheetOff = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                         "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                 "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                         "color: #0000;";

    pLabel = patientLabel;

    QDomDocument document;

    test_config_file_name = "config_keygrip_test_"+pLabel;
    config_file_name = "config_keygrip_"+pLabel;

    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
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
    QDomNode PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
    QDomElement PWNodeVal = PWNode.toElement();
    QDomNode YCoorNode = root.elementsByTagName("YCoordinates_KeyGrip").at(0).firstChild();
    QDomElement YCoorNodeVal = YCoorNode.toElement();

    if (!CurrentNodeVal.isNull())
    {

        currOneStored = root.elementsByTagName("CH1").at(0).firstChild().nodeValue().toFloat();
        currTwoStored = root.elementsByTagName("CH2").at(0).firstChild().nodeValue().toFloat();
        currThreeStored = root.elementsByTagName("CH3").at(0).firstChild().nodeValue().toFloat();
        currFourStored = root.elementsByTagName("CH4").at(0).firstChild().nodeValue().toFloat();
        currFiveStored = root.elementsByTagName("CH5").at(0).firstChild().nodeValue().toFloat();

        ui->label_currOne->setText(QString("Ch 1 (EDC): %1 mA").arg(currOneStored));
        ui->label_currTwo->setText(QString("Ch 2 (FDS): %1 mA").arg(currTwoStored));
        ui->label_currThree->setText(QString("Ch 3 (Ulna): %1 mA").arg( currThreeStored));
        ui->label_currFour->setText(QString("Ch 4 (ADP): %1 mA").arg(currFourStored));
        ui->label_currFive->setText(QString("Ch 5 (Opp P): %1 mA").arg(currFiveStored));

    }

    if(!YCoorNodeVal.isNull())
    {
        //EDC segments
         Y_EDC1 = findXMLNodeValue(root, "YCoordinates_KeyGrip", "YCoor_EDC1").toInt();
         Y_EDC2 = findXMLNodeValue(root, "YCoordinates_KeyGrip", "YCoor_EDC2").toInt();
         Y_EDC3 = findXMLNodeValue(root, "YCoordinates_KeyGrip", "YCoor_EDC4").toInt();
        // p41 = QPoint(150,350);


         p42 = QPoint(170,Y_EDC1);
         p43 = QPoint(250,Y_EDC1);
         p44 = QPoint(270,Y_EDC2);
         p45 = QPoint(450,Y_EDC2);
         p46 = QPoint(470,Y_EDC3);
         p47 = QPoint(550,Y_EDC3);
         //p48 = QPoint(570,350);
         PW_EDC1 = adjust_PW_range(Y_EDC1);
         PW_phase1_EDC = PW_EDC1;
         Y_coordinates_EDC1 = Y_EDC1;
      //   ramp_stepsize_phase1_EDC = findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p1_EDC").toInt();
         PW_EDC2 = adjust_PW_range(Y_EDC2);
         PW_phase2_EDC = PW_EDC2;
         PW_phase3_EDC = PW_EDC2;
         Y_coordinates_EDC2 = Y_EDC2;
         PW_EDC3 = adjust_PW_range(Y_EDC3);
         PW_phase4_EDC = PW_EDC3;
         Y_coordinates_EDC3 = Y_EDC3;
         ui->label_EDC1->setGeometry(170,Y_EDC1-15,47,13);
         ui->label_EDC1->setText(QString::number(adjust_PW_range(Y_EDC1))+"us");
         ui->label_EDC2->setGeometry(270+50,Y_EDC2-15,47,13);
         ui->label_EDC2->setText(QString::number(adjust_PW_range(Y_EDC2))+"us");
         ui->label_EDC3->setGeometry(470+50,Y_EDC3-15,47,13);
         ui->label_EDC3->setText(QString::number(adjust_PW_range(Y_EDC3))+"us");

        //FDS segments
         Y_FDS = findXMLNodeValue(root, "YCoordinates_KeyGrip", "YCoor_FDS").toInt();
         //p11 = QPoint(230,320);
         p12 = QPoint(280,Y_FDS);
         p13 = QPoint(450,Y_FDS);
         PW_FDS = adjust_PW_range(Y_FDS);
         PW_phase2_FDS = PW_FDS;
         PW_phase3_FDS = PW_FDS;
         Y_coordinates_FDS = Y_FDS;
         //p14 = QPoint(500,320);
         ui->label_FDS->setGeometry(280,Y_FDS-15,47,13);
         ui->label_FDS->setText(QString::number(adjust_PW_range(Y_FDS))+"us");

        //Ulna segments
         Y_Ulna = findXMLNodeValue(root, "YCoordinates_KeyGrip", "YCoor_Ulna").toInt();
         //p31 = QPoint(300,310);
         p32 = QPoint(320,Y_Ulna);
         p33 = QPoint(450,Y_Ulna);
         //p34 = QPoint(470,320);
         PW_Ulna = adjust_PW_range(Y_Ulna);
         PW_phase3_Ulna = PW_Ulna;
         Y_coordinates_Ulna = Y_Ulna;
         ui->label_Ulna->setGeometry(320+20,Y_Ulna-15,47,13);
         ui->label_Ulna->setText(QString::number(adjust_PW_range(Y_Ulna))+"us");

        //ADP segments
         Y_ADP = findXMLNodeValue(root, "YCoordinates_KeyGrip", "YCoor_ADP").toInt();
         //p21 = QPoint(300,310);
         p22 = QPoint(320,Y_ADP);
         p23 = QPoint(450,Y_ADP);
         //p24 = QPoint(470,320);
         PW_ADP = adjust_PW_range(Y_ADP);
         PW_phase3_ADP = PW_ADP;
         Y_coordinates_ADP = Y_ADP;
         ui->label_ADP->setGeometry(320+40,Y_ADP-15,47,13);
         ui->label_ADP->setText(QString::number(adjust_PW_range(Y_ADP))+"us");


         //Ramp index values
         ui->comboBox_1->setCurrentIndex(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P1").toInt());
         ui->comboBox_2->setCurrentIndex(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P2").toInt());
         ui->comboBox_3->setCurrentIndex(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P3").toInt());
         ui->comboBox_4->setCurrentIndex(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P4").toInt());
         ramp_phase1 = ui->comboBox_1->itemData(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P1").toInt()).toFloat();
         ramp_phase2 = ui->comboBox_2->itemData(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P2").toInt()).toFloat();
         ramp_phase3 = ui->comboBox_3->itemData(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P3").toInt()).toFloat();
         ramp_phase4 = ui->comboBox_4->itemData(findXMLNodeValue(root, "RampIndex_KeyGrip", "RmpIdx_P4").toInt()).toFloat();
    }

    if(!PWNode.isNull())
    {
      QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+config_file_name+".txt";
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

    else
    {
      QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+test_config_file_name+".txt";
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



    connect(&api, &tetra_grip_api::tetraGripEvent,this, &ProgramKeyGripV2::keyGripPhaseEventHandler);
    connect(ui->btn_nextPhase, &QPushButton::clicked, this, &ProgramKeyGripV2::nextBtn);
    connect(this, &ProgramKeyGripV2::buttonChanged, this, &ProgramKeyGripV2::paintBtn);
    connect(this, &ProgramKeyGripV2::pulseWidthValue, this, &ProgramKeyGripV2::getPWValue);
    connect(this, &ProgramKeyGripV2::pulseWidthValue, this, &ProgramKeyGripV2::nextBtn);
    connect(this, &ProgramKeyGripV2::pulseWidthValue, this, &ProgramKeyGripV2::prevBtn);
    connect(this, &ProgramKeyGripV2::lastPhase, this, &ProgramKeyGripV2::sendConfigFile);

}
ProgramKeyGripV2::~ProgramKeyGripV2()
{
    tetra_grip_api::stimulation_pause(true);
    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &ProgramKeyGripV2::keyGripPhaseEventHandler);
    delete ui;
}

// small helper to give us the distance
int ProgramKeyGripV2::distance(QPoint x1, QPoint x2)
{
    return abs(x2.y() - x1.y());
}

void ProgramKeyGripV2::eventHandler(STIM_GUI_TOPIC_T topic, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {

        case STIM_REG_NUM_SMART_SENSORS:
           if(value==0){
               QMessageBox::critical(this, "Sensor Not Connected!", "Shoulder sensor got disconnected \n\nReconnect the sensor and press Ok to continue");
           }

            break;
        }
    }
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


    //qDebug()<<"Values of p1.y and p2.y:"<<p12.y()<<"and"<<p22.y();
}

// when user clicks
void ProgramKeyGripV2::mousePressEvent(QMouseEvent *event)
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

//       CurPoint3 = &p46;
//       CurPoint4 = &p47;

       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }

   else if (distance ( mp, p46) < 20 && ( mp.x() > p46.x() && mp.x() < p47.x() ) && EDC_Seg3_checked) {
       EDC_Seg3_dragging = true;

       CurPoint1 = &p46;
       CurPoint2 = &p47;

//       CurPoint3 = &p46;
//       CurPoint4 = &p47;
       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }
   else if (distance ( mp, p44) < 20 && ( mp.x() > p44.x() && mp.x() < p45.x() ) && EDC_Seg2_checked) {
       EDC_Seg2_dragging = true;
       CurPoint1 = &p44;
       CurPoint2 = &p45;
       this->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));

   }
}

void ProgramKeyGripV2::mouseReleaseEvent(QMouseEvent *event)
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
void ProgramKeyGripV2::mouseMoveEvent(QMouseEvent *event)
{
   // If we are dragging, call your normal slider changed function to update your points.


    if (FDS_dragging && FDS_checked)
    {
       if(event->y() > 150 && event->y() < 308){
         changeP1value(event->y());
       }
        if(FDS_checked){
          // ui->radioButton_one->setText("FDS+FDP Val:"+QString::number(CurPoint1->y()));
           ui->label_FDS->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
           ui->label_FDS->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
           PW_FDS = adjust_PW_range(CurPoint1->y());
           PW_phase2_FDS = PW_FDS;
           PW_phase3_FDS = PW_FDS;
           Y_coordinates_FDS = CurPoint1->y();        
          }
    }
    else if(Ulna_dragging && Ulna_checked)
    {
      if(event->y() > 150 && event->y() < 308){
        changeP1value(event->y());
       }

      if(Ulna_checked){
    //  ui->radioButton_two->setText("Ulna nerve Val:"+QString::number(CurPoint1->y()));
      ui->label_Ulna->setGeometry(QString::number(CurPoint1->x()).toInt()+20,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_Ulna->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      PW_Ulna = adjust_PW_range(CurPoint1->y());
      PW_phase3_Ulna = PW_Ulna;
      Y_coordinates_Ulna = CurPoint1->y();
      }
    }
    else if(ADP_dragging && ADP_checked)
    {
      if(event->y() > 150 && event->y() < 308){
        changeP1value(event->y());
       }

      if(ADP_checked){
     // ui->radioButton_three->setText("FPL or ADP Val:"+QString::number(CurPoint1->y()));
      ui->label_ADP->setGeometry(QString::number(CurPoint1->x()).toInt()+40,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_ADP->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      Y_coordinates_ADP = CurPoint1->y();
      PW_ADP = adjust_PW_range(CurPoint1->y());
      PW_phase3_ADP = PW_ADP;
      }
    }
    else if(EDC_Seg1_dragging && EDC_Seg1_checked)
    {
      if(event->y() > 150 && event->y() < 308){ // test
        changeP1value(event->y());
      //  changeP2value(event->y());
       }

      if(EDC_Seg1_checked){
    //  ui->radioButton_four->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_EDC1->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_EDC1->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      PW_EDC1 = adjust_PW_range(CurPoint1->y());
      PW_phase1_EDC = PW_EDC1;
      Y_coordinates_EDC1 = CurPoint1->y();
      }
    }
    else if(EDC_Seg3_dragging && EDC_Seg3_checked)
    {
      if(event->y() > 150 && event->y() < 308){
        changeP1value(event->y());
      //  changeP2value(event->y());
       }

      if(EDC_Seg3_checked){
    //  ui->radioButton_five->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_EDC3->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_EDC3->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
    //  PW_EDC1 = adjust_PW_range(CurPoint3->y());
      PW_EDC3 = adjust_PW_range(CurPoint1->y());
      PW_phase4_EDC = PW_EDC3;

//      ui->label_EDC1->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
//      ui->label_EDC1->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      Y_coordinates_EDC3 = CurPoint1->y();

      }
    }

    else if(EDC_Seg2_dragging && EDC_Seg2_checked)
    {
      if(event->y() > 150 && event->y() < 308){
        changeP1value(event->y());
       }

      if(EDC_Seg2_checked){
     // ui->radioButton_six->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_EDC2->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_EDC2->setText(QString::number(adjust_PW_range(CurPoint1->y()))+"us");
      PW_EDC2 = adjust_PW_range(CurPoint1->y());
      PW_phase2_EDC = PW_EDC2;
      PW_phase3_EDC = PW_EDC2;
      Y_coordinates_EDC2 = CurPoint1->y();
      }
    }
    else
       update();

}

void ProgramKeyGripV2::changeP1value(int value)
{

    CurPoint1->setY(value);
    CurPoint2->setY(value);


    update();
    emit pulseWidthValue(adjust_PW_range(CurPoint1->y()));
    pw_value = adjust_PW_range(CurPoint1->y());


}

void ProgramKeyGripV2::changeP2value(int value)
{
    CurPoint3->setY(value);
    CurPoint4->setY(value);
    update();
}

void ProgramKeyGripV2::on_pushButton_back_keypro_clicked()
{
    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &ProgramKeyGripV2::keyGripPhaseEventHandler);
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}

void ProgramKeyGripV2::nextBtn(int pwvalue)
//void ProgramKeyGripV2::nextBtn()
{
    m_currentBtn++;
    if(m_currentBtn > btnGrp->buttons().size())
    {
        m_currentBtn = 0;
        phaseOver = true;
        getRampStepSize();
        saveToXMLFile();
        ManageConfigFile configFile;
        configFile.keyGripFinal(pLabel);

    }
   emit buttonChanged(m_currentBtn, pwvalue);
    // emit buttonChanged(m_currentBtn, pw_value);
   // qDebug()<<"Button size"<< btnGrp->buttons().size() << "And current Button"<< m_currentBtn;
}

void ProgramKeyGripV2::prevBtn(int pwvalue)
{
    m_currentBtn--;
     if(m_currentBtn < 1)
     {
         m_currentBtn = btnGrp->buttons().size();
     }

  emit buttonChanged(m_currentBtn, pwvalue);
   //  qDebug()<<"Button size"<< btnGrp->buttons().size() << "And current Button"<< m_currentBtn;
}

void ProgramKeyGripV2::paintBtn(int id, int pwvalue)
{


   // ui->label_9->setText("ID size:"+QString::number(btnGrp->buttons().size())+" AND "+"ID no:"+QString::number(id));
   // ui->label_7->setText("");

   // ui->label_10->setText(QString::number(pwvalue));

    if(id==1)
    {
        if(EDC_Seg1_checked)
        {
           // PW_EDC1 = pwvalue;
       //     ui->label_11->setText("EDC: "+QString::number(PW_EDC1));
            tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 1, PW_EDC1);

            tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 1, adjust_Ramp_Step_size(PW_EDC1,ramp_phase1));

        }
    }

    else if(id==2)
    {
        if(EDC_Seg2_checked)
       {
           //PW_EDC2 = pwvalue;
       //    ui->label_12->setText("EDC 2: "+QString::number(PW_EDC2));
           tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 2, PW_EDC2);
           tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 3, PW_EDC2);

           tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 2, adjust_Ramp_Step_size(PW_EDC2,ramp_phase2));
           tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 3, adjust_Ramp_Step_size(PW_EDC2,ramp_phase3));

       }

        else if(FDS_checked)
        {
           //PW_FDS = pwvalue;
        //   ui->label_15->setText("FDS: "+QString::number(PW_FDS));
           tetra_grip_api::set_stimulation_target_pulse_width( m_channelTwo, 2, PW_FDS);
           tetra_grip_api::set_stimulation_target_pulse_width( m_channelTwo, 3, PW_FDS);

           tetra_grip_api::set_stimulation_ramp_rate( m_channelTwo, 2, adjust_Ramp_Step_size(PW_FDS,ramp_phase2));
           tetra_grip_api::set_stimulation_ramp_rate( m_channelTwo, 3, adjust_Ramp_Step_size(PW_FDS,ramp_phase3));

        }
    }

    else if(id==3)
    {
        if(Ulna_checked)
       {
          //PW_Ulna = pwvalue;
      //    ui->label_14->setText("Ulna: "+QString::number(PW_Ulna));
          tetra_grip_api::set_stimulation_target_pulse_width( m_channelThree, 3, PW_Ulna);

          tetra_grip_api::set_stimulation_ramp_rate( m_channelThree, 3, adjust_Ramp_Step_size(PW_Ulna,ramp_phase3));

       }

       else if(ADP_checked)
       {
         // PW_ADP = pwvalue;
      //    ui->label_16->setText("ADP: "+QString::number(PW_ADP));
          tetra_grip_api::set_stimulation_target_pulse_width( m_channelFour, 3, PW_ADP);

          tetra_grip_api::set_stimulation_ramp_rate( m_channelFour, 3, adjust_Ramp_Step_size(PW_ADP,ramp_phase3));
       }
    }


    else if(id==4)
    {
        if(EDC_Seg3_checked)
        {
            //PW_EDC3 = pwvalue;
         //   ui->label_13->setText("EDC: "+QString::number(PW_EDC3));
            tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 4, PW_EDC3);

            tetra_grip_api::set_stimulation_ramp_rate( m_channelOne, 4, adjust_Ramp_Step_size(PW_EDC3,ramp_phase4));
        }

    }

    pw_value = pwvalue;



    qDebug()<<"Button size is"<< btnGrp->buttons().size() << "And current Button is"<< id;


//     if(id==1)
//     {
//         ui->label_pwvalue->setText(QString("From 1, %1").arg(pwvalue));
//        // ui->btn1->setStyleSheet("background-color: green");
//         ui->btn1->setStyleSheet(StyleSheetOn);
//         ui->btn2->setStyleSheet(StyleSheetOff);
//         ui->btn3->setStyleSheet(StyleSheetOff);
//         ui->btn4->setStyleSheet(StyleSheetOff);
//         ui->pushButton_keyGrip->setEnabled(false);

//         if(EDC_Seg1_checked)
//         {
//            PW_phase1_EDC = pwvalue;
//            PW_phase4_EDC = pwvalue;
//            //ramp_stepsize_phase1_EDC = adjust_Ramp_Step_size( PW_phase1_EDC, ramp_phase1);

//         }

//         ui->radioButton_one->setEnabled(false); // FPS
//         ui->radioButton_two->setEnabled(false); // Ulna
//         ui->radioButton_three->setEnabled(false); // ADP
//         ui->radioButton_four->setEnabled(true); // EDC seg 1
//         ui->radioButton_five->setEnabled(false); // EDC seg 3
//         ui->radioButton_six->setEnabled(false); // EDC seg 2

//     }

//     else if(id==2)
//     {
//         ui->label_pwvalue->setText(QString("From 2, %1").arg(pwvalue));
//         ui->btn1->setStyleSheet(StyleSheetOff);
//         ui->btn2->setStyleSheet(StyleSheetOn);
//         ui->btn3->setStyleSheet(StyleSheetOff);
//         ui->btn4->setStyleSheet(StyleSheetOff);
//         ui->pushButton_keyGrip->setEnabled(false);

//         ui->radioButton_one->setEnabled(true); // FPS
//         ui->radioButton_two->setEnabled(false); // Ulna
//         ui->radioButton_three->setEnabled(false); // ADP
//         ui->radioButton_four->setEnabled(false); // EDC seg 1
//         ui->radioButton_five->setEnabled(false); // EDC seg 3
//         ui->radioButton_six->setEnabled(true); // EDC seg 2

//         if(EDC_Seg2_checked)
//         {
//            PW_phase2_EDC = pwvalue;
//            //ramp_stepsize_phase2_EDC = adjust_Ramp_Step_size( PW_phase2_EDC, ramp_phase2);
//         }
//         else if(FDS_checked)
//         {
//            PW_phase2_FDS = pwvalue;
//           // ramp_stepsize_phase2_FDS = adjust_Ramp_Step_size( PW_phase2_FDS, ramp_phase2);
//         }
//     }

//    else if(id==3)
//    {
//         ui->label_pwvalue->setText(QString("From 3, %1").arg(pwvalue));
//         ui->btn1->setStyleSheet(StyleSheetOff);
//         ui->btn2->setStyleSheet(StyleSheetOff);
//         ui->btn3->setStyleSheet(StyleSheetOn);
//         ui->btn4->setStyleSheet(StyleSheetOff);
//         ui->pushButton_keyGrip->setEnabled(false);

//         ui->radioButton_one->setEnabled(true); // FPS
//         ui->radioButton_two->setEnabled(true); // Ulna
//         ui->radioButton_three->setEnabled(true); // ADP
//         ui->radioButton_four->setEnabled(false); // EDC seg 1
//         ui->radioButton_five->setEnabled(false); // EDC seg 3
//         ui->radioButton_six->setEnabled(true); // EDC seg 2

//         if(EDC_Seg2_checked)
//         {
//            //PW_phase3_EDC = pwvalue;
//            PW_phase3_EDC = PW_phase2_EDC;
//            //ramp_stepsize_phase3_EDC = adjust_Ramp_Step_size( PW_phase3_EDC, ramp_phase3);
//         }
//         else if(FDS_checked)
//         {
//           // PW_phase3_FDS = pwvalue;
//            PW_phase3_FDS = PW_phase2_FDS;
//           // ramp_stepsize_phase3_FDS = adjust_Ramp_Step_size( PW_phase3_FDS, ramp_phase3);
//         }
//         else if(Ulna_checked)
//         {
//            PW_phase3_Ulna = pwvalue;
//           // ramp_stepsize_phase3_Ulna = adjust_Ramp_Step_size( PW_phase3_Ulna, ramp_phase3);
//         }
//         else if(ADP_checked)
//         {
//            PW_phase3_ADP = pwvalue;
//           // ramp_stepsize_phase3_ADP = adjust_Ramp_Step_size( PW_phase3_ADP, ramp_phase3);
//         }
//    }

//    else if(id==4)
//    {
//         ui->label_pwvalue->setText(QString("From 4, %1").arg(pwvalue));
//         ui->btn1->setStyleSheet(StyleSheetOff);
//         ui->btn2->setStyleSheet(StyleSheetOff);
//         ui->btn3->setStyleSheet(StyleSheetOff);
//         ui->btn4->setStyleSheet(StyleSheetOn);
//         ui->pushButton_keyGrip->setEnabled(false);

//         ui->radioButton_one->setEnabled(false); // FPS
//         ui->radioButton_two->setEnabled(false); // Ulna
//         ui->radioButton_three->setEnabled(false); // ADP
//         ui->radioButton_four->setEnabled(false); // EDC seg 1
//         ui->radioButton_five->setEnabled(true); // EDC seg 3
//         ui->radioButton_six->setEnabled(false); // EDC seg 2


//         if(EDC_Seg3_checked)
//         {
//             PW_phase1_EDC = pwvalue;
//             PW_phase4_EDC = pwvalue;
//           // ramp_stepsize_phase4_EDC = adjust_Ramp_Step_size( PW_phase4_EDC, ramp_phase4);
//         }
//    }

//    else
//    {
//        // ui->btn0->setStyleSheet(StyleSheetOn);
//         ui->btn1->setStyleSheet(StyleSheetOff);
//         ui->btn2->setStyleSheet(StyleSheetOff);
//         ui->btn3->setStyleSheet(StyleSheetOff);
//         ui->btn4->setStyleSheet(StyleSheetOff);
//         ui->pushButton_keyGrip->setEnabled(true);
//         ui->pushButton_save->setEnabled(true);

//         ui->radioButton_one->setEnabled(false); // FPS
//         ui->radioButton_two->setEnabled(false); // Ulna
//         ui->radioButton_three->setEnabled(false); // ADP
//         ui->radioButton_four->setEnabled(false); // EDC seg 1
//         ui->radioButton_five->setEnabled(false); // EDC seg 3
//         ui->radioButton_six->setEnabled(false); // EDC seg 2

//         emit lastPhase(id);


//    }

}


void ProgramKeyGripV2::getPWValue(int value)
{
    // ui->label_pwvalue->setText(QString("%1").arg(value));
}


void ProgramKeyGripV2::keyGripPhaseEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value)
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
                 stimStopped = false;

            }
            else
            {
                 ui->pushButton_stimStop->setText("Stopped");
                 ui->pushButton_stimStart->setText("Start");
                 ui->btn_nextPhase->setEnabled(false);
                 stimStopped = true;
            }
            break;

        case STIM_SUB_ACT_REG_CURRENT_PHASE:

            QList<QPushButton *> List{ui->btn0,ui->btn1,ui->btn2,ui->btn3,ui->btn4};
             for (QPushButton * button : List) {
               button->setStyleSheet(StyleSheetOff);
             }
             List[value]->setStyleSheet(StyleSheetOn);

           break;

        }
    }


}


void ProgramKeyGripV2::on_pushButton_nextPhase_clicked()
{
    //ui->label_phaseNo->setText("");
}

void ProgramKeyGripV2::on_pushButton_stimStart_clicked()
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

   // update();
}

void ProgramKeyGripV2::on_pushButton_stimStop_clicked()
{
     tetra_grip_api::stimulation_pause(true);

     m_currentBtn = 0;


     ui->btn_nextPhase->setEnabled(false);
     ui->comboBox_1->setEnabled(false);
     ui->comboBox_2->setEnabled(false);
     ui->comboBox_3->setEnabled(false);
     ui->comboBox_4->setEnabled(false);

     ui->radioButton_one->setAutoExclusive(false); //autoExclusive is off by default, except for radio buttons
     ui->radioButton_two->setAutoExclusive(false);
     ui->radioButton_three->setAutoExclusive(false);
     ui->radioButton_four->setAutoExclusive(false);
     ui->radioButton_five->setAutoExclusive(false);
     ui->radioButton_six->setAutoExclusive(false);

     ui->radioButton_one->setChecked(false);
     ui->radioButton_two->setChecked(false);
     ui->radioButton_three->setChecked(false);
     ui->radioButton_four->setChecked(false);
     ui->radioButton_five->setChecked(false);
     ui->radioButton_six->setChecked(false);

     ui->radioButton_one->setEnabled(false);
     ui->radioButton_two->setEnabled(false);
     ui->radioButton_three->setEnabled(false);
     ui->radioButton_four->setEnabled(false);
     ui->radioButton_five->setEnabled(false);
     ui->radioButton_six->setEnabled(false);

     ui->radioButton_one->setAutoExclusive(true);
     ui->radioButton_two->setAutoExclusive(true);
     ui->radioButton_three->setAutoExclusive(true);
     ui->radioButton_four->setAutoExclusive(true);
     ui->radioButton_five->setAutoExclusive(true);
     ui->radioButton_six->setAutoExclusive(true);

//     ui->pushButton_keyGrip->setEnabled(false);
//     ui->pushButton_save->setEnabled(false);

//     QString StyleSheetOff("border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
//                           "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
//                                   "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
//                           "color: #0000;");

     ui->btn1->setStyleSheet(StyleSheetOff);
     ui->btn2->setStyleSheet(StyleSheetOff);
     ui->btn3->setStyleSheet(StyleSheetOff);
     ui->btn4->setStyleSheet(StyleSheetOff);

     update();

}


void ProgramKeyGripV2::on_btn_nextPhase_clicked()
{
    tetra_grip_api::send_event(0, 253);
}

int ProgramKeyGripV2::adjust_PW_range(int value)
{
    //int result = ((280-value)*3)+1 ; // 0-180us
    //int result = (1224-(4.2*value)) ; // 50-300us
    int result = (588-(1.9*value)) ; // 50-300us



    if (result == 0)
         return -1;
    else
         return result;
}

void ProgramKeyGripV2::on_comboBox_1_currentIndexChanged(int index)
{
    float value = ui->comboBox_1->itemData(index).toFloat();
    ramp_phase1 = value;

}

void ProgramKeyGripV2::on_comboBox_2_currentIndexChanged(int index)
{
    float value = ui->comboBox_2->itemData(index).toFloat();
    ramp_phase2 = value;


}

void ProgramKeyGripV2::on_comboBox_3_currentIndexChanged(int index)
{
    float value = ui->comboBox_3->itemData(index).toFloat();
    ramp_phase3 = value;

}

void ProgramKeyGripV2::on_comboBox_4_currentIndexChanged(int index)
{
    float value = ui->comboBox_4->itemData(index).toFloat();
    ramp_phase4 = value;


}

int ProgramKeyGripV2::adjust_Ramp_Step_size(int pwvalue, float rmpvalue)
{
    int rmp_step_size = 1000*(pwvalue/(rmpvalue*40));
    return rmp_step_size;
}

void ProgramKeyGripV2::getRampStepSize()
{
       ramp_stepsize_phase1_EDC = adjust_Ramp_Step_size( PW_phase1_EDC, ramp_phase1);
       ramp_stepsize_phase2_EDC = adjust_Ramp_Step_size( PW_phase2_EDC, ramp_phase2);
       ramp_stepsize_phase2_FDS = adjust_Ramp_Step_size( PW_phase2_FDS, ramp_phase2);
       ramp_stepsize_phase3_EDC = adjust_Ramp_Step_size( PW_phase3_EDC, ramp_phase3);
       ramp_stepsize_phase3_FDS = adjust_Ramp_Step_size( PW_phase3_FDS, ramp_phase3);
       ramp_stepsize_phase3_Ulna = adjust_Ramp_Step_size( PW_phase3_Ulna, ramp_phase3);
       ramp_stepsize_phase3_ADP = adjust_Ramp_Step_size( PW_phase3_ADP, ramp_phase3);
       ramp_stepsize_phase4_EDC = adjust_Ramp_Step_size( PW_phase4_EDC, ramp_phase4);



}

void ProgramKeyGripV2::closeEvent(QCloseEvent *event)
{
    tetra_grip_api::stimulation_pause(true);
}

QString ProgramKeyGripV2::findXMLNodeValue(const QDomElement &root, const QString &parentname, const QString &childname)
{
    QDomElement element = root.firstChildElement(parentname);
    return element.firstChildElement(childname).firstChild().nodeValue();
}

void ProgramKeyGripV2::saveToXMLFile()
{
    QString filename = pLabel;
    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
    QFile file(path);

    /* QT Append wont work!
     * Open the file read-only, read it all in, close it.
     * Make changes in-memory document.
     * Then open the file for overwrite, write all content, close file. */

    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {

        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
        return;
    }

     QDomDocument document;
     document.setContent(&file);
     QDomElement root = document.documentElement();


    file.close();

    QDomElement newPWTag = document.createElement(QString("PW_KeyGrip"));
    QDomElement newRmpTag = document.createElement(QString("Ramp_KeyGrip"));
    QDomElement newYCoorTag = document.createElement(QString("YCoordinates_KeyGrip"));
    QDomElement newRmpIdxTag = document.createElement(QString("RampIndex_KeyGrip"));


    QDomNode PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
    QDomElement PWNodeVal = PWNode.toElement();
    QDomNode RmpNode = root.elementsByTagName("Ramp_KeyGrip").at(0).firstChild();
    QDomElement RmpNodeVal = RmpNode.toElement();
    QDomNode YCoorNode = root.elementsByTagName("YCoordinates_KeyGrip").at(0).firstChild();
    QDomElement YCoorNodeVal = YCoorNode.toElement();
    QDomNode RmpIdxNode = root.elementsByTagName("RampIndex_KeyGrip").at(0).firstChild();
    QDomElement RmpIdxNodeVal = RmpIdxNode.toElement();


    if (PWNodeVal.isNull())
    {
        QDomElement P1_EDCTag = document.createElement(QString("PW_p1_EDC"));
        QDomText P1_EDCVal = document.createTextNode(QString::number(PW_phase1_EDC));
        P1_EDCTag.appendChild(P1_EDCVal);
        newPWTag.appendChild(P1_EDCTag);

        QDomElement P2_EDCTag = document.createElement(QString("PW_p2_EDC"));
        QDomText P2_EDCVal = document.createTextNode(QString::number(PW_phase2_EDC));
        P2_EDCTag.appendChild(P2_EDCVal);
        newPWTag.appendChild(P2_EDCTag);

        QDomElement P2_FDSTag = document.createElement(QString("PW_p2_FDS"));
        QDomText P2_FDSVal = document.createTextNode(QString::number(PW_phase2_FDS));
        P2_FDSTag.appendChild(P2_FDSVal);
        newPWTag.appendChild(P2_FDSTag);

        QDomElement P3_EDCTag = document.createElement(QString("PW_p3_EDC"));
        QDomText P3_EDCVal = document.createTextNode(QString::number(PW_phase3_EDC));
        P3_EDCTag.appendChild(P3_EDCVal);
        newPWTag.appendChild(P3_EDCTag);

        QDomElement P3_FDSTag = document.createElement(QString("PW_p3_FDS"));
        QDomText P3_FDSVal = document.createTextNode(QString::number(PW_phase3_FDS));
        P3_FDSTag.appendChild(P3_FDSVal);
        newPWTag.appendChild(P3_FDSTag);

        QDomElement P3_UlnaTag = document.createElement(QString("PW_p3_Ulna"));
        QDomText P3_UlnaVal = document.createTextNode(QString::number(PW_phase3_Ulna));
        P3_UlnaTag.appendChild(P3_UlnaVal);
        newPWTag.appendChild(P3_UlnaTag);

        QDomElement P3_ADPTag = document.createElement(QString("PW_p3_ADP"));
        QDomText P3_ADPVal = document.createTextNode(QString::number(PW_phase3_ADP));
        P3_ADPTag.appendChild(P3_ADPVal);
        newPWTag.appendChild(P3_ADPTag);

        QDomElement P4_EDCTag = document.createElement(QString("PW_p4_EDC"));
        QDomText P4_EDCVal = document.createTextNode(QString::number(PW_phase4_EDC));
        P4_EDCTag.appendChild(P4_EDCVal);
        newPWTag.appendChild(P4_EDCTag);

        root.appendChild(newPWTag);
    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("PW_KeyGrip");

          QDomNode pw1 = SettingsNode.namedItem("PW_p1_EDC");
          pw1.firstChild().setNodeValue(QString::number(PW_phase1_EDC));
          QDomNode pw2 = SettingsNode.namedItem("PW_p2_EDC");
          pw2.firstChild().setNodeValue(QString::number(PW_phase2_EDC));
          QDomNode pw3 = SettingsNode.namedItem("PW_p2_FDS");
          pw3.firstChild().setNodeValue(QString::number(PW_phase2_FDS));
          QDomNode pw4 = SettingsNode.namedItem("PW_p3_EDC");
          pw4.firstChild().setNodeValue(QString::number(PW_phase3_EDC));
          QDomNode pw5 = SettingsNode.namedItem("PW_p3_FDS");
          pw5.firstChild().setNodeValue(QString::number(PW_phase3_FDS));
          QDomNode pw6 = SettingsNode.namedItem("PW_p3_Ulna");
          pw6.firstChild().setNodeValue(QString::number(PW_phase3_Ulna));
          QDomNode pw7 = SettingsNode.namedItem("PW_p3_ADP");
          pw7.firstChild().setNodeValue(QString::number(PW_phase3_ADP));
          QDomNode pw8 = SettingsNode.namedItem("PW_p4_EDC");
          pw8.firstChild().setNodeValue(QString::number(PW_phase4_EDC));

    }

     if(RmpNodeVal.isNull())
    {
        QDomElement P1_Rmp_EDCTag = document.createElement(QString("Rmp_p1_EDC"));
        QDomText P1_Rmp_EDCVal = document.createTextNode(QString::number(ramp_stepsize_phase1_EDC));
        P1_Rmp_EDCTag.appendChild(P1_Rmp_EDCVal);
        newRmpTag.appendChild(P1_Rmp_EDCTag);

        QDomElement P2_Rmp_EDCTag = document.createElement(QString("Rmp_p2_EDC"));
        QDomText P2_Rmp_EDCVal = document.createTextNode(QString::number(ramp_stepsize_phase2_EDC));
        P2_Rmp_EDCTag.appendChild(P2_Rmp_EDCVal);
        newRmpTag.appendChild(P2_Rmp_EDCTag);

        QDomElement P2_Rmp_FDSTag = document.createElement(QString("Rmp_p2_FDS"));
        QDomText P2_Rmp_FDSVal = document.createTextNode(QString::number(ramp_stepsize_phase2_FDS));
        P2_Rmp_FDSTag.appendChild(P2_Rmp_FDSVal);
        newRmpTag.appendChild(P2_Rmp_FDSTag);

        QDomElement P3_Rmp_FDSTag = document.createElement(QString("Rmp_p3_FDS"));
        QDomText P3_Rmp_FDSVal = document.createTextNode(QString::number(ramp_stepsize_phase3_FDS));
        P3_Rmp_FDSTag.appendChild(P3_Rmp_FDSVal);
        newRmpTag.appendChild(P3_Rmp_FDSTag);

        QDomElement P3_Rmp_EDCTag = document.createElement(QString("Rmp_p3_EDC"));
        QDomText P3_Rmp_EDCVal = document.createTextNode(QString::number(ramp_stepsize_phase3_EDC));
        P3_Rmp_EDCTag.appendChild(P3_Rmp_EDCVal);
        newRmpTag.appendChild(P3_Rmp_EDCTag);

        QDomElement P3_Rmp_UlnaTag = document.createElement(QString("Rmp_p3_Ulna"));
        QDomText P3_Rmp_UlnaVal = document.createTextNode(QString::number(ramp_stepsize_phase3_Ulna));
        P3_Rmp_UlnaTag.appendChild(P3_Rmp_UlnaVal);
        newRmpTag.appendChild(P3_Rmp_UlnaTag);

        QDomElement P3_Rmp_ADPTag = document.createElement(QString("Rmp_p3_ADP"));
        QDomText P3_Rmp_ADPVal = document.createTextNode(QString::number(ramp_stepsize_phase3_ADP));
        P3_Rmp_ADPTag.appendChild(P3_Rmp_ADPVal);
        newRmpTag.appendChild(P3_Rmp_ADPTag);

        QDomElement P4_Rmp_EDCTag = document.createElement(QString("Rmp_p4_EDC"));
        QDomText P4_Rmp_EDCVal = document.createTextNode(QString::number(ramp_stepsize_phase4_EDC));
        P4_Rmp_EDCTag.appendChild(P4_Rmp_EDCVal);
        newRmpTag.appendChild(P4_Rmp_EDCTag);

        root.appendChild(newRmpTag);

    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode RampSettingsNode = root.namedItem("Ramp_KeyGrip");

          QDomNode rmp1 = RampSettingsNode.namedItem("Rmp_p1_EDC");
          rmp1.firstChild().setNodeValue(QString::number(ramp_stepsize_phase1_EDC));
          QDomNode rmp2 = RampSettingsNode.namedItem("Rmp_p2_EDC");
          rmp2.firstChild().setNodeValue(QString::number(ramp_stepsize_phase2_EDC));
          QDomNode rmp3 = RampSettingsNode.namedItem("Rmp_p2_FDS");
          rmp3.firstChild().setNodeValue(QString::number(ramp_stepsize_phase2_FDS));
          QDomNode rmp4 = RampSettingsNode.namedItem("Rmp_p3_EDC");
          rmp4.firstChild().setNodeValue(QString::number(ramp_stepsize_phase3_EDC));
          QDomNode rmp5 = RampSettingsNode.namedItem("Rmp_p3_FDS");
          rmp5.firstChild().setNodeValue(QString::number(ramp_stepsize_phase3_FDS));
          QDomNode rmp6 = RampSettingsNode.namedItem("Rmp_p3_Ulna");
          rmp6.firstChild().setNodeValue(QString::number(ramp_stepsize_phase3_Ulna));
          QDomNode rmp7 = RampSettingsNode.namedItem("Rmp_p3_ADP");
          rmp7.firstChild().setNodeValue(QString::number(ramp_stepsize_phase3_ADP));
          QDomNode rmp8 = RampSettingsNode.namedItem("Rmp_p4_EDC");
          rmp8.firstChild().setNodeValue(QString::number(ramp_stepsize_phase4_EDC));

    }

     if( YCoorNodeVal.isNull())
     {
         QDomElement P1_YCoor_EDCTag = document.createElement(QString("YCoor_EDC1"));
         QDomText P1_YCoor_EDCVal = document.createTextNode(QString::number(Y_coordinates_EDC1));
         P1_YCoor_EDCTag.appendChild(P1_YCoor_EDCVal);
         newYCoorTag.appendChild(P1_YCoor_EDCTag);

         QDomElement P2_YCoor_EDCTag = document.createElement(QString("YCoor_EDC2"));
         QDomText P2_YCoor_EDCVal = document.createTextNode(QString::number(Y_coordinates_EDC2));
         P2_YCoor_EDCTag.appendChild(P2_YCoor_EDCVal);
         newYCoorTag.appendChild(P2_YCoor_EDCTag);

         QDomElement P4_YCoor_EDCTag = document.createElement(QString("YCoor_EDC4"));
         QDomText P4_YCoor_EDCVal = document.createTextNode(QString::number(Y_coordinates_EDC3));
         P4_YCoor_EDCTag.appendChild(P4_YCoor_EDCVal);
         newYCoorTag.appendChild(P4_YCoor_EDCTag);

         QDomElement P2_YCoor_FDSTag = document.createElement(QString("YCoor_FDS"));
         QDomText P2_YCoor_FDSVal = document.createTextNode(QString::number(Y_coordinates_FDS));
         P2_YCoor_FDSTag.appendChild(P2_YCoor_FDSVal);
         newYCoorTag.appendChild(P2_YCoor_FDSTag);

         QDomElement P3_YCoor_UlnaTag = document.createElement(QString("YCoor_Ulna"));
         QDomText P3_YCoor_UlnaVal = document.createTextNode(QString::number(Y_coordinates_Ulna));
         P3_YCoor_UlnaTag.appendChild(P3_YCoor_UlnaVal);
         newYCoorTag.appendChild(P3_YCoor_UlnaTag);

         QDomElement P3_YCoor_ADPTag = document.createElement(QString("YCoor_ADP"));
         QDomText P3_YCoor_ADPVal = document.createTextNode(QString::number(Y_coordinates_ADP));
         P3_YCoor_ADPTag.appendChild(P3_YCoor_ADPVal);
         newYCoorTag.appendChild(P3_YCoor_ADPTag);

         root.appendChild(newYCoorTag);


     }
     else
     {
         QDomElement root = document.documentElement();
         QDomNode YCoorSettingsNode = root.namedItem("YCoordinates_KeyGrip");

         QDomNode EDC1 = YCoorSettingsNode.namedItem("YCoor_EDC1");
         EDC1.firstChild().setNodeValue(QString::number(Y_coordinates_EDC1));
         QDomNode EDC2 = YCoorSettingsNode.namedItem("YCoor_EDC2");
         EDC2.firstChild().setNodeValue(QString::number(Y_coordinates_EDC2));
         QDomNode FDS = YCoorSettingsNode.namedItem("YCoor_FDS");
         FDS.firstChild().setNodeValue(QString::number(Y_coordinates_FDS));
         QDomNode Ulna = YCoorSettingsNode.namedItem("YCoor_Ulna");
         Ulna.firstChild().setNodeValue(QString::number(Y_coordinates_Ulna));
         QDomNode ADP = YCoorSettingsNode.namedItem("YCoor_ADP");
         ADP.firstChild().setNodeValue(QString::number(Y_coordinates_ADP));
         QDomNode EDC4 = YCoorSettingsNode.namedItem("YCoor_EDC4");
         EDC4.firstChild().setNodeValue(QString::number(Y_coordinates_EDC3));
     }

     if( RmpIdxNodeVal.isNull())
     {
         QDomElement P1_RmpIdxTag = document.createElement(QString("RmpIdx_P1"));
         QDomText P1_RmpIdxVal = document.createTextNode(QString::number(ramp_phase1));
         P1_RmpIdxTag.appendChild(P1_RmpIdxVal);
         newRmpIdxTag.appendChild(P1_RmpIdxTag);

         QDomElement P2_RmpIdxTag = document.createElement(QString("RmpIdx_P2"));
         QDomText P2_RmpIdxVal = document.createTextNode(QString::number(ramp_phase2));
         P2_RmpIdxTag.appendChild(P2_RmpIdxVal);
         newRmpIdxTag.appendChild(P2_RmpIdxTag);

         QDomElement P3_RmpIdxTag = document.createElement(QString("RmpIdx_P3"));
         QDomText P3_RmpIdxVal = document.createTextNode(QString::number(ramp_phase3));
         P3_RmpIdxTag.appendChild(P3_RmpIdxVal);
         newRmpIdxTag.appendChild(P3_RmpIdxTag);

         QDomElement P4_RmpIdxTag = document.createElement(QString("RmpIdx_P4"));
         QDomText P4_RmpIdxVal = document.createTextNode(QString::number(ramp_phase4));
         P4_RmpIdxTag.appendChild(P4_RmpIdxVal);
         newRmpIdxTag.appendChild(P4_RmpIdxTag);

         root.appendChild(newRmpIdxTag);
     }
     else
     {
         QDomElement root = document.documentElement();
         QDomNode RmpIdxSettingsNode = root.namedItem("RampIndex_KeyGrip");

         QDomNode P1 = RmpIdxSettingsNode.namedItem("RmpIdx_P1");
         P1.firstChild().setNodeValue(QString::number(ui->comboBox_1->currentIndex()));
         QDomNode P2 = RmpIdxSettingsNode.namedItem("RmpIdx_P2");
         P2.firstChild().setNodeValue(QString::number(ui->comboBox_2->currentIndex()));
         QDomNode P3 = RmpIdxSettingsNode.namedItem("RmpIdx_P3");
         P3.firstChild().setNodeValue(QString::number(ui->comboBox_3->currentIndex()));
         QDomNode P4 = RmpIdxSettingsNode.namedItem("RmpIdx_P4");
         P4.firstChild().setNodeValue(QString::number(ui->comboBox_4->currentIndex()));

     }


    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        qDebug () << "Error saving XML file....";
        QMessageBox::information(this, "Unable to open XML file to write", file.errorString());
        return;
    }

    QTextStream output(&file);
    output << document.toString();
    file.close();

    qDebug()<< "Finished";
}

void ProgramKeyGripV2::loadConfigFile(QString configfilename)
{




    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/" + configfilename + ".txt";

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









//    QString xmlName = pLabel;
//    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
//    QFile xmlfile(xmlReadPath);

//    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

//    }
//    QDomDocument document;
//    document.setContent(&xmlfile);
//    QDomElement root = document.documentElement();
//    xmlfile.close();
//    QDomNode PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
//    QDomElement PWNodeVal = PWNode.toElement();
//    if(!PWNode.isNull())
//    {
//        ManageConfigFile configFile;
//        configFile.keyGripFinal(pLabel);
//        QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+config_file_name+".txt";
//        QFile f(txtWritePath);
//        if(!f.open(QFile::ReadOnly))
//             {
//                 QMessageBox::information(0, "config file error", f.errorString());
//             }
//        else
//             {
//                 QByteArray config = f.readAll();
//                 tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

//             }
//    }

//    else
//    {
//        ManageConfigFile configFile;
//        configFile.keyGripTest(pLabel);
//        QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+test_config_file_name+".txt";
//        QFile f(txtWritePath);
//        if(!f.open(QFile::ReadOnly))
//             {
//                 QMessageBox::information(0, "config file error", f.errorString());
//             }
//        else
//             {
//                 QByteArray config = f.readAll();
//                 tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

//             }
//    }

}

void ProgramKeyGripV2::sendConfigFile(int id)
{
    ManageConfigFile newFile;
    newFile.keyGripFinal(pLabel);
             //resetTimer();
    loadConfigFile("config_keygrip_"+pLabel);
    ui->label_7->setText("c file sent at phase no " + QString::number(id));
}

void ProgramKeyGripV2::on_pushButton_keyGrip_clicked()
{



    QString configfilename = "config_keygrip_"+pLabel;
    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/" + configfilename + ".txt";
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

void ProgramKeyGripV2::on_pushButton_save_clicked()
{
//    saveToXMLFile();
//    ManageConfigFile configFile;
//    configFile.keyGripFinal(pLabel);
}
