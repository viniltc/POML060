#include "programkeygripv2.h"
#include "ui_programkeygripv2.h"
#include "stageprogram.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>

ProgramKeyGripV2::ProgramKeyGripV2(QString patientLabel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProgramKeyGripV2)
{
    ui->setupUi(this);
    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");

    pLabel = patientLabel;

    btnGrp = new QButtonGroup(this);

    btnGrp->addButton(ui->btn1, 1);
    btnGrp->addButton(ui->btn2, 2);
    btnGrp->addButton(ui->btn3, 3);
    btnGrp->addButton(ui->btn4, 4);


   ui->btn1->setStyleSheet(""); // initial style (start with btn1)
   ui->btn_nextPhase->setEnabled(false);

   m_currentBtn = 0;

    connect(&api, &tetra_grip_api::tetraGripEvent,this, &ProgramKeyGripV2::keyGripPhaseEventHandler);

    connect(ui->btn_nextPhase, &QPushButton::clicked, this, &ProgramKeyGripV2::nextBtn);

    connect(this, &ProgramKeyGripV2::buttonChanged, this, &ProgramKeyGripV2::paintBtn);
    connect(this, &ProgramKeyGripV2::pulseWidthValue, this, &ProgramKeyGripV2::getPWValue);
    connect(this, &ProgramKeyGripV2::pulseWidthValue, this, &ProgramKeyGripV2::nextBtn);
    connect(this, &ProgramKeyGripV2::pulseWidthValue, this, &ProgramKeyGripV2::prevBtn);


    QFile f(":/resources/config_keygrip_v2.txt");
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
       if(event->y() > 220 && event->y() < 280){
         changeP1value(event->y());
       }
        if(FDS_checked){
           ui->radioButton_one->setText("FDS+FDP Val:"+QString::number(CurPoint1->y()));
           ui->label_one->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
           ui->label_one->setText(QString::number(adjust_PW_range(CurPoint1->y())));
           //ui->label_dragimg->setGeometry(QString::number(CurPoint1->x()).toInt()+80,QString::number(CurPoint1->y()).toInt()-10,31,21);
          }
    }
    else if(Ulna_dragging && Ulna_checked)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(Ulna_checked){
      ui->radioButton_two->setText("Ulna nerve Val:"+QString::number(CurPoint1->y()));
      ui->label_two->setGeometry(QString::number(CurPoint1->x()).toInt()+20,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_two->setText(QString::number(adjust_PW_range(CurPoint1->y())));
      }
    }
    else if(ADP_dragging && ADP_checked)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(ADP_checked){
      ui->radioButton_three->setText("FPL or ADP Val:"+QString::number(CurPoint1->y()));
      ui->label_three->setGeometry(QString::number(CurPoint1->x()).toInt()+40,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_three->setText(QString::number(adjust_PW_range(CurPoint1->y())));
      }
    }
    else if(EDC_Seg1_dragging && EDC_Seg1_checked)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(EDC_Seg1_checked){
      ui->radioButton_four->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_four->setGeometry(QString::number(CurPoint1->x()).toInt(),QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_four->setText(QString::number(adjust_PW_range(CurPoint1->y())));
      }
    }
    else if(EDC_Seg3_dragging && EDC_Seg3_checked)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(EDC_Seg3_checked){
      ui->radioButton_five->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_five->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_five->setText(QString::number(adjust_PW_range(CurPoint1->y())));
      }
    }

    else if(EDC_Seg2_dragging && EDC_Seg2_checked)
    {
      if(event->y() > 220 && event->y() < 280){
        changeP1value(event->y());
       }

      if(EDC_Seg2_checked){
      ui->radioButton_six->setText("EDC+EPL Val:"+QString::number(CurPoint1->y()));
      ui->label_six->setGeometry(QString::number(CurPoint1->x()).toInt()+50,QString::number(CurPoint1->y()).toInt()-15,47,13);
      ui->label_six->setText(QString::number(adjust_PW_range(CurPoint1->y())));
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
    emit pulseWidthValue(adjust_PW_range(CurPoint1->y()));

}

void ProgramKeyGripV2::on_pushButton_back_keypro_clicked()
{
    stageProgram *backprogram;
    this->close();
    backprogram = new stageProgram(pLabel,this);
    backprogram -> show();
}

void ProgramKeyGripV2::nextBtn(int pwvalue)
{
    m_currentBtn++;
    if(m_currentBtn > btnGrp->buttons().size())
    {
        m_currentBtn = 0;
        saveToXMLFile();
    }

    emit buttonChanged(m_currentBtn, pwvalue);
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


    if(EDC_Seg1_checked)
    {
      tetra_grip_api::stimulation_target_pulse_width( m_channelOne, 1, pwvalue);
    }
    else if(EDC_Seg2_checked)
    {
    tetra_grip_api::stimulation_target_pulse_width( m_channelOne, 2, pwvalue);
    tetra_grip_api::stimulation_target_pulse_width( m_channelOne, 3, pwvalue);
    }
    else if(EDC_Seg3_checked)
    {
    tetra_grip_api::stimulation_target_pulse_width( m_channelOne, 4, pwvalue);
    }
    else if(Ulna_checked)
    {
       tetra_grip_api::stimulation_target_pulse_width( m_channelThree, 3, pwvalue);
        //tetra_grip_api::stimulation_target_pulse_width( m_channelTwo, 3, pwvalue);
    }
    else if(FDS_checked)
    {
       tetra_grip_api::stimulation_target_pulse_width( m_channelTwo, 2, pwvalue);
       tetra_grip_api::stimulation_target_pulse_width( m_channelTwo, 3, pwvalue);
        //tetra_grip_api::stimulation_target_pulse_width( m_channelTwo, 3, pwvalue);
    }
    else if(ADP_checked)
    {
    tetra_grip_api::stimulation_target_pulse_width( m_channelFour, 3, pwvalue);
    }





    qDebug()<<"Button size is"<< btnGrp->buttons().size() << "And current Button is"<< id;
  //tetra_grip_api::stimulation_target_pulse_width()
/*    switch(id)
    {

    case 1 :

         ui->label_pwvalue->setText(QString("From 1, %1").arg(pwvalue));
         ui->btn1->setStyleSheet("background-color: red");
         ui->btn2->setStyleSheet("");
         ui->btn3->setStyleSheet("");
         ui->btn4->setStyleSheet("");

        break;
    case 2 :
         ui->label_pwvalue->setText(QString("From 2, %1").arg(pwvalue));
         ui->btn1->setStyleSheet("");
         ui->btn2->setStyleSheet("background-color: red");
         ui->btn3->setStyleSheet("");
         ui->btn4->setStyleSheet("");
        break;

    case 3 :
        ui->label_pwvalue->setText(QString("From 3, %1").arg(pwvalue));
        ui->btn1->setStyleSheet("");
        ui->btn2->setStyleSheet("");
        ui->btn3->setStyleSheet("background-color: red");
        ui->btn4->setStyleSheet("");
        break;
    case 4 :    
        ui->label_pwvalue->setText(QString("From 4, %1").arg(pwvalue));     
        ui->btn1->setStyleSheet("");
        ui->btn2->setStyleSheet("");
        ui->btn3->setStyleSheet("");
        ui->btn4->setStyleSheet("background-color: red");
        break;

    } */

     if(id==1)
     {
         ui->label_pwvalue->setText(QString("From 1, %1").arg(pwvalue));
         ui->btn1->setStyleSheet("background-color: green");
         ui->btn2->setStyleSheet("");
         ui->btn3->setStyleSheet("");
         ui->btn4->setStyleSheet("");

         if(EDC_Seg1_checked)
         {
            PW_phase1_EDC = pwvalue;
         }

     }

     else if(id==2)
     {
         ui->label_pwvalue->setText(QString("From 2, %1").arg(pwvalue));
         ui->btn1->setStyleSheet("");
         ui->btn2->setStyleSheet("background-color: green");
         ui->btn3->setStyleSheet("");
         ui->btn4->setStyleSheet("");

         if(EDC_Seg2_checked)
         {
            PW_phase2_EDC = pwvalue;
         }
         else if(FDS_checked)
         {
            PW_phase2_FDS = pwvalue;
         }
     }

    else if(id==3)
    {
         ui->label_pwvalue->setText(QString("From 3, %1").arg(pwvalue));
         ui->btn1->setStyleSheet("");
         ui->btn2->setStyleSheet("");
         ui->btn3->setStyleSheet("background-color: green");
         ui->btn4->setStyleSheet("");

         if(EDC_Seg2_checked)
         {
            PW_phase3_EDC = pwvalue;
         }
         else if(FDS_checked)
         {
            PW_phase3_FDS = pwvalue;
         }
         else if(Ulna_checked)
         {
            PW_phase3_Ulna = pwvalue;
         }
         else if(ADP_checked)
         {
            PW_phase3_ADP = pwvalue;
         }
    }

    else if(id==4)
    {
         ui->label_pwvalue->setText(QString("From 4, %1").arg(pwvalue));
         ui->btn1->setStyleSheet("");
         ui->btn2->setStyleSheet("");
         ui->btn3->setStyleSheet("");
         ui->btn4->setStyleSheet("background-color: green");

         if(EDC_Seg3_checked)
         {
            PW_phase4_EDC = pwvalue;
         }
    }

    else
    {
         ui->btn1->setStyleSheet("");
         ui->btn2->setStyleSheet("");
         ui->btn3->setStyleSheet("");
         ui->btn4->setStyleSheet("");
    }


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

void ProgramKeyGripV2::on_pushButton_nextPhase_clicked()
{
    //ui->label_phaseNo->setText("");
}

void ProgramKeyGripV2::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);
    ui->btn_nextPhase->setEnabled(true);
}

void ProgramKeyGripV2::on_pushButton_stimStop_clicked()
{
     tetra_grip_api::stimulation_pause(true);
}

void ProgramKeyGripV2::on_btn_nextPhase_clicked()
{
    tetra_grip_api::send_event(0, 253);
}

int ProgramKeyGripV2::adjust_PW_range(int value)
{
    return ((280-value)*3)+1;
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

    QDomNode PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
    QDomElement PWNodeVal = PWNode.toElement();

    if (PWNodeVal.isNull())
    {
        QDomElement P1_EDCTag = document.createElement(QString("PW_phase1_EDC"));
        QDomText P1_EDCVal = document.createTextNode(QString::number(PW_phase1_EDC));
        P1_EDCTag.appendChild(P1_EDCVal);
        newPWTag.appendChild(P1_EDCTag);

        QDomElement P2_EDCTag = document.createElement(QString("PW_phase2_EDC"));
        QDomText P2_EDCVal = document.createTextNode(QString::number(PW_phase2_EDC));
        P2_EDCTag.appendChild(P2_EDCVal);
        newPWTag.appendChild(P2_EDCTag);

        QDomElement P2_FDSTag = document.createElement(QString("PW_phase2_FDS"));
        QDomText P2_FDSVal = document.createTextNode(QString::number(PW_phase2_FDS));
        P2_FDSTag.appendChild(P2_FDSVal);
        newPWTag.appendChild(P2_FDSTag);

        QDomElement P3_EDCTag = document.createElement(QString("PW_phase3_EDC"));
        QDomText P3_EDCVal = document.createTextNode(QString::number(PW_phase3_EDC));
        P3_EDCTag.appendChild(P3_EDCVal);
        newPWTag.appendChild(P3_EDCTag);

        QDomElement P3_FDSTag = document.createElement(QString("PW_phase3_FDS"));
        QDomText P3_FDSVal = document.createTextNode(QString::number(PW_phase3_FDS));
        P3_FDSTag.appendChild(P3_FDSVal);
        newPWTag.appendChild(P3_FDSTag);

        QDomElement P3_UlnaTag = document.createElement(QString("PW_phase3_Ulna"));
        QDomText P3_UlnaVal = document.createTextNode(QString::number(PW_phase3_Ulna));
        P3_UlnaTag.appendChild(P3_UlnaVal);
        newPWTag.appendChild(P3_UlnaTag);

        QDomElement P3_ADPTag = document.createElement(QString("PW_phase3_ADP"));
        QDomText P3_ADPVal = document.createTextNode(QString::number(PW_phase3_ADP));
        P3_ADPTag.appendChild(P3_ADPVal);
        newPWTag.appendChild(P3_ADPTag);

        QDomElement P4_EDCTag = document.createElement(QString("PW_phase4_EDC"));
        QDomText P4_EDCVal = document.createTextNode(QString::number(PW_phase4_EDC));
        P4_EDCTag.appendChild(P4_EDCVal);
        newPWTag.appendChild(P4_EDCTag);

        root.appendChild(newPWTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("PW_KeyGrip");

          QDomNode pw1 = SettingsNode.namedItem("PW_phase1_EDC");
          pw1.firstChild().setNodeValue(QString::number(PW_phase1_EDC));
          QDomNode pw2 = SettingsNode.namedItem("PW_phase2_EDC");
          pw2.firstChild().setNodeValue(QString::number(PW_phase2_EDC));
          QDomNode pw3 = SettingsNode.namedItem("PW_phase2_FDS");
          pw3.firstChild().setNodeValue(QString::number(PW_phase2_FDS));
          QDomNode pw4 = SettingsNode.namedItem("PW_phase3_EDC");
          pw4.firstChild().setNodeValue(QString::number(PW_phase3_EDC));
          QDomNode pw5 = SettingsNode.namedItem("PW_phase3_FDS");
          pw5.firstChild().setNodeValue(QString::number(PW_phase3_FDS));
          QDomNode pw6 = SettingsNode.namedItem("PW_phase3_Ulna");
          pw6.firstChild().setNodeValue(QString::number(PW_phase3_Ulna));
          QDomNode pw7 = SettingsNode.namedItem("PW_phase3_ADP");
          pw7.firstChild().setNodeValue(QString::number(PW_phase3_ADP));
          QDomNode pw8 = SettingsNode.namedItem("PW_phase4_EDC");
          pw8.firstChild().setNodeValue(QString::number(PW_phase4_EDC));

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
