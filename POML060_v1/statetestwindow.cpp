#include "statetestwindow.h"
#include "ui_statetestwindow.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include "stageprogram.h"

StateTestWindow::StateTestWindow(QString patientLabel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StateTestWindow)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    StyleSheetOn = "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                         "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)";
    StyleSheetOff = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                          "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                  "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                          "color: #0000;";

    QPixmap pix(":/resources/GraspStateDiagram_nobox.png"); // this is the code to add image to the form
    int w = ui->label_bg->width();
    int h = ui->label_bg->height();
    ui->label_bg->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QList<QPushButton *> List = findChildren<QPushButton *>();
     for (QPushButton * button : List) {
       button->setStyleSheet(StyleSheetOff);
     }

    // List[0] - > setStyleSheet(StyleSheetOn);
     ui->btn5->setStyleSheet(StyleSheetOn);

     QString configfilename = "config_full_sample_v1";
     //QString xmlName = pLabel;

    // QString txtWritePath = ":/resources/"+configfilename+".txt";
     QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configfilename+".txt";

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

StateTestWindow::~StateTestWindow()
{
    delete ui;
}

void StateTestWindow::phaseEventHandler(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_BATTERY_CAPACITY_REMAINING:
            //ui -> label_bat -> setText("Battery remaining: "+QString::number(value)+"%");
            break;

        case STIM_SUB_ACT_REG_CURRENT_PHASE:
            //ui->btn1->setText("Phase No " + QString::number(value));
//            if(value == 0)
//            {
//                ui->btn1->setStyleSheet(StyleSheetOn);
//                ui->btn2->setStyleSheet(StyleSheetOff);
//                ui->btn3->setStyleSheet(StyleSheetOff);
//            }
//            else if(value == 1)
//            {
//                ui->btn2->setStyleSheet(StyleSheetOn);
//                ui->btn1->setStyleSheet(StyleSheetOff);
//                ui->btn3->setStyleSheet(StyleSheetOff);
//            }
//            else if(value == 2)
//            {
//                ui->btn3->setStyleSheet(StyleSheetOn);
//                ui->btn2->setStyleSheet(StyleSheetOff);
//                ui->btn1->setStyleSheet(StyleSheetOff);
//            }


            QList<QPushButton *> List{ui->btn0,ui->btn1,ui->btn2,ui->btn3,ui->btn4,ui->btn5,ui->btn6,ui->btn7,ui->btn8 };
             for (QPushButton * button : List) {
               button->setStyleSheet(StyleSheetOff);
             }
             List[value]->setStyleSheet(StyleSheetOn);
             //List.removeAt(2);
             qDebug()<<"The list values are" << List;

            break;
        }
    }
}

void StateTestWindow::on_pushButton_back_clicked()
{

    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,this);
    backprogram -> show();
}
