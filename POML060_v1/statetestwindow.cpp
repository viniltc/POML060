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

    ui->btn00->setVisible(false); // to make phase0 button invisible

    QPixmap pix(":/resources/GraspStateDiagram_nobox_v24.png"); // this is the code to add image to the form
    int w = ui->label_bg->width();
    int h = ui->label_bg->height();
    ui->label_bg->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QList<QPushButton *> List = findChildren<QPushButton *>();
     for (QPushButton * button : List) {
       button->setStyleSheet(StyleSheetOff);
     }

    // List[0] - > setStyleSheet(StyleSheetOn);
     ui->btn0->setStyleSheet(StyleSheetOn);

   // QString txtfilename = "config_tetraGrip_v7";
    //  QString txtfilename = "config_tetraGrip_TEST1_v1";
     QString txtfilename = "config_tetraGrip_"+pLabel;
     QString configFileName = QCoreApplication::applicationDirPath()+"/data/config_file/"+txtfilename+".txt";
     QFile f(configFileName);
     if(!f.open(QFile::ReadOnly))
          {
              QMessageBox::information(0, "config file error", f.errorString());
          }
     else
          {
              QByteArray config = f.readAll();
              tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

          }


     connect(&api, &tetra_grip_api::tetraGripEvent,this, &StateTestWindow::phaseEventHandler);


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
            //ui -> label_20 -> setText("Battery remaining: "+QString::number(value)+"%");
            break;

        case STIM_SUB_ACT_REG_CURRENT_PHASE:
            QList<QPushButton *> List{ui->btn0,ui->btn1,ui->btn2,ui->btn3,ui->btn4,ui->btn5,ui->btn6,ui->btn7,ui->btn8,ui->btn9,ui->btn10,ui->btn11,ui->btn12,ui->btn13,ui->btn14};
             for (QPushButton * button : List) {
               button->setStyleSheet(StyleSheetOff);
             }
            // ui->label_20->setText("Phase no: " + QString::number(value));
             List[value]->setStyleSheet(StyleSheetOn);
             //List.removeAt(2);
             //qDebug()<<"The list values are" << List;
            break;
        }
    }
}

void StateTestWindow::closeEvent(QCloseEvent *event)
{
    tetra_grip_api::stimulation_pause(true);
}

void StateTestWindow::on_pushButton_back_clicked()
{

    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,this);
    backprogram -> show();
}

void StateTestWindow::on_pushButton_loadConfig_clicked()
{

    // to load config file as non volatile
    QString txtfilename = "config_tetraGrip_"+pLabel;
    QString configFileName = QCoreApplication::applicationDirPath()+"/data/config_file/"+txtfilename+".txt";
    QFile f(configFileName);
    if(!f.open(QFile::ReadOnly))
         {
             QMessageBox::information(0, "config file error", f.errorString());
         }
    else
         {
             QByteArray config = f.readAll();
             QMessageBox::StandardButton reply;
               reply = QMessageBox::question(this, "TetraGrip", "Are you sure want to load settings to the stimulator?",
                                             QMessageBox::Yes|QMessageBox::No);
               if (reply == QMessageBox::Yes) {
                    tetra_grip_api::send_long_register(STIM_LONG_REG_NV_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());// send config file as non volatile
                                                       //STIM_LONG_REG_STIM_CONFIG_FILE or STIM_LONG_REG_NV_STIM_CONFIG_FILE
               }

               else if(reply == QMessageBox::No) {
                   return;
               }
         }

}

void StateTestWindow::on_pushButton_logReset_clicked()
{
    // to reset stimulator log
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "TetraGrip", "Are you sure want to reset the stimulator log?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          tetra_grip_api::do_stimulator_command(STIM_COMMAND_RESET_LOG_HEADER);
          tetra_grip_api::do_stimulator_command(STIM_COMMAND_RESET_LOG_PHASE_DATA);
      }

      else if(reply == QMessageBox::No) {
          return;
      }
}
