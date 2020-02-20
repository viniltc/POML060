#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "manageconfigfile.h"


stageProgram::stageProgram(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram)
{
    ui->setupUi(this);
    ui->pushButton_currOnOne->setCheckable(true);
    ui->pushButton_currOnTwo->setCheckable(true);
    ui->pushButton_currOnThree->setCheckable(true);
    ui->pushButton_currOnFour->setCheckable(true);
    ui->pushButton_currOnFive->setCheckable(true);

    ui->label_curr_one->setText(QString("%1 mA").arg(0));
    ui->label_curr_two->setText(QString("%1 mA").arg(0));
    ui->label_curr_three->setText(QString("%1 mA").arg(0));
    ui->label_curr_four->setText(QString("%1 mA").arg(0));
    ui->label_curr_five->setText(QString("%1 mA").arg(0));


    ui->widget_currentOne->setEnabled(false);
    ui->widget_currentTwo->setEnabled(false);
    ui->widget_currentThree->setEnabled(false);
    ui->widget_currentFour->setEnabled(false);
    ui->widget_currentFive->setEnabled(false);

    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");

    pLabel = patientLabel;

    connect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);

    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);

   // connect(this, &stageProgram::targetCurrentValue, this, &stageProgram::setCurrOnChannelOne);

 // tetra_grip_api::get_target_current_channel(0);

}

stageProgram::~stageProgram()
{
    delete ui;
   // tetra_grip_api::stimulation_pause(true);
}


void stageProgram::setCurrOnChannelOne(unsigned int current_uA)
{

   // currentOneSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelOne, current_uA);

}

void stageProgram::setCurrOnChannelTwo(unsigned int current_uA)
{

    //currentTwoSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelTwo, current_uA);
}

void stageProgram::setCurrOnChannelThree(unsigned int current_uA)
{

    //currentThreeSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelThree, current_uA);
}

void stageProgram::setCurrOnChannelFour(unsigned int current_uA)
{

   // currentFourSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFour, current_uA);
}

void stageProgram::setCurrOnChannelFive(unsigned int current_uA)
{

   // currentFiveSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFive, current_uA);
}

void stageProgram::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);
}

void stageProgram::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);
}

void stageProgram::stimStatusEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value)
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
            }
            break;
        }
    }
   if (topic==TOPIC_CHANNEL && reg==STIM_ENGINE_REG_TARGET_CURRENT) // Line no: 1088,  stim_gui_protocol_decode.c
   {
        switch(index)
        {
        case 0: // channel 1

            ui->label_curr_one->setText(QString("%1 mA").arg(value/m_currentmA));
            currentOneSetVal = value/m_currentmA;
//            if(value != currentOneSetVal/10)
//            {
//            ui->label_curr_one->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 1:

            ui->label_curr_two->setText(QString("%1 mA").arg(value/m_currentmA));
            currentTwoSetVal = value/m_currentmA;
//            if(value != currentTwoSetVal/10)
//            {
//            ui->label_curr_two->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 2:

            ui->label_curr_three->setText(QString("%1 mA").arg(value/m_currentmA));
            currentThreeSetVal = value/m_currentmA;
//            if(value != currentThreeSetVal/10)
//            {
//            ui->label_curr_three->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 3:

            ui->label_curr_four->setText(QString("%1 mA").arg(value/m_currentmA));
            currentFourSetVal = value/m_currentmA;
//            if(value != currentFourSetVal/10)
//            {
//            ui->label_curr_four->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        case 4:

            ui->label_curr_five->setText(QString("%1 mA").arg(value/m_currentmA));
            currentFiveSetVal = value/m_currentmA;
//            if(value != currentFiveSetVal/10)
//            {
//            ui->label_curr_five->setText("Value mismatch!");
//            tetra_grip_api::stimulation_pause(true);
//            }
            break;
        }

    }
}



void stageProgram::on_pushButton_programKeyGrip_clicked()
{
   ManageConfigFile configFile;
   configFile.keyGripTest(pLabel);

   this->hide();
   keygripv2 = new ProgramKeyGripV2(pLabel);
   keygripv2 -> show();
}

void stageProgram::on_pushButton_programPalmerGrasp_clicked()
{
    this->hide();
    palmergrasp = new ProgramPalmerGrasp(pLabel);
    palmergrasp->show();
}

void stageProgram::on_pushButton_programOpenHand_clicked()
{
    this->close();
    openhand = new ProgramOpenHand(pLabel);
    openhand->show();
}

void stageProgram::on_pushButton_programSwitchGrasp_clicked()
{
    this->hide();
    switchgrasp = new ProgramSwitchGrasp(pLabel);
    switchgrasp->show();

}

void stageProgram::on_pushButton_stimSave_clicked()
{

  saveToXMLFile();

}

void stageProgram::saveToXMLFile()
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

    QDomElement newCurrentTag = document.createElement(QString("Current"));

    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();

    if (CurrentNodeVal.isNull())
    {
        QDomElement Ch1Tag = document.createElement(QString("CH1"));
        QDomText Ch1Val = document.createTextNode(QString::number(currentOneSetVal));
        Ch1Tag.appendChild(Ch1Val);
        newCurrentTag.appendChild(Ch1Tag);

        QDomElement Ch2Tag = document.createElement(QString("CH2"));
        QDomText Ch2Val = document.createTextNode(QString::number(currentTwoSetVal));
        Ch2Tag.appendChild(Ch2Val);
        newCurrentTag.appendChild(Ch2Tag);

        QDomElement Ch3Tag = document.createElement(QString("CH3"));
        QDomText Ch3Val = document.createTextNode(QString::number(currentThreeSetVal));
        Ch3Tag.appendChild(Ch3Val);
        newCurrentTag.appendChild(Ch3Tag);

        QDomElement Ch4Tag = document.createElement(QString("CH4"));
        QDomText Ch4Val = document.createTextNode(QString::number(currentFourSetVal));
        Ch4Tag.appendChild(Ch4Val);
        newCurrentTag.appendChild(Ch4Tag);

        QDomElement Ch5Tag = document.createElement(QString("CH5"));
        QDomText Ch5Val = document.createTextNode(QString::number(currentFiveSetVal));
        Ch5Tag.appendChild(Ch5Val);
        newCurrentTag.appendChild(Ch5Tag);

        root.appendChild(newCurrentTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("Current");

          QDomNode cur1 = SettingsNode.namedItem("CH1");
          cur1.firstChild().setNodeValue(QString::number(currentOneSetVal));
          QDomNode cur2 = SettingsNode.namedItem("CH2");
          cur2.firstChild().setNodeValue(QString::number(currentTwoSetVal));
          QDomNode cur3 = SettingsNode.namedItem("CH3");
          cur3.firstChild().setNodeValue(QString::number(currentThreeSetVal));
          QDomNode cur4 = SettingsNode.namedItem("CH4");
          cur4.firstChild().setNodeValue(QString::number(currentFourSetVal));
          QDomNode cur5 = SettingsNode.namedItem("CH5");
          cur5.firstChild().setNodeValue(QString::number(currentFiveSetVal));

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
