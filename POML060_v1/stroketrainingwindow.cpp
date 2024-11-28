#include "stroketrainingwindow.h"
#include "ui_stroketrainingwindow.h"
#include "stageprogram.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include "imudialog.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "manageconfigfile.h"
#include "manangeconfigextended.h"
#include <QTimer>

StrokeTrainingWindow::StrokeTrainingWindow(QString patientLabel, QMap<QString, QString> defaultValues, int page, bool configId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StrokeTrainingWindow)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    progPage = page;
    configStatusId = configId;
    m_currentBtn1 = 0;
    ui->btn0->setVisible(false);
    ui->radioButton_p1t2->setChecked(true);
    ui->toolButton_imu1->setEnabled(false);
   // ui->pushButton_triggerApply->setEnabled(false);
    //ui->pushButton_configSet->setEnabled(false);

     // Save the original size of the window
    originalSize = this->size();

    // Save the original geometry of each widget
       for (auto& widget : this->findChildren<QWidget*>()) {
           originalGeometries[widget] = widget->geometry();

           QFont font = widget->font();
                   originalFontSizes[widget] = font.pointSize();
       }

    // Convert the defaultValues QMap to a string
     QString defaultValuesString = defaultValuesToString(defaultValues);

     // Assuming you have a QPlainTextEdit widget in your UI named plainTextEdit
     //ui->plainTextEdit->setPlainText(defaultValuesString);


    if (progPage == 0)
    {
        QPixmap pix(":/resources/envStrokeProg12.jpg"); // this is the code to add image to the form
        originalPixmap = pix; // Save the original pixmap
        int w = ui->label_bgs1->width();
        int h = ui->label_bgs1->height();
        ui->label_bgs1->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

        StyleSheetOn = "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                             "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)";
        StyleSheetOff = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                              "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                      "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                              "color: #0000;";

        test_config_file_pr1 = "config_sProg1_test_"+pLabel;
        config_file_pr1 = "config_sProg1_final_"+pLabel;
          if (configId ==0)
        {

            loadConfigFile(test_config_file_pr1);


        }

        else
        {
            loadConfigFile(config_file_pr1);
        }

    }

    setupUiComponents(defaultValues);
    configureConnections();
    configureUiComponents();

}

StrokeTrainingWindow::~StrokeTrainingWindow()
{
    delete ui;

}

void StrokeTrainingWindow::strokeTrainingPhaseEventHandler(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value)
{
    if (progPage ==0)
    {
        if (topic==TOPIC_STIMULATOR)
        {
            switch(reg)
            {
            case STIM_REG_ACTIVITY_STATUS: // Line no: 714,  stim_gui_protocol_decode.c

                if(value==true)
                {
                    ui->pushButton_stimStop1->setText("Stop");
                    ui->pushButton_stimStart1->setText("Running");
                    stimStopped = false;

                }
                else
                {
                    ui->pushButton_stimStop1->setText("Stopped");
                    ui->pushButton_stimStart1->setText("Start");
                    ui->btn_nextPhase1->setEnabled(false);
                    stimStopped = true;
                }
                break;

            case STIM_SUB_ACT_REG_CURRENT_PHASE:

                //QList<QPushButton *> List{ui->btn0,ui->btn11,ui->btn12,ui->btn13};
                QList<QPushButton *> List{ui->btn11,ui->btn12,ui->btn13};
                for (QPushButton * button : List) {
                    button->setStyleSheet(StyleSheetOff);
                }
                List[value]->setStyleSheet(StyleSheetOn);

                break;

            }
        }
    }
}

void StrokeTrainingWindow::closeEvent(QCloseEvent *event)
{
    tetra_grip_api::stimulation_pause(true);

    if(event->spontaneous()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "TetraGrip", "You are about to close Tetragrip Application, have you saved settings?\n\n Click Yes to save and close or Cancel to remian in this window",
                                      QMessageBox::Yes|QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {

            saveToXMLFile();
            event->accept();
        }
        else if(reply == QMessageBox::Cancel) {
            event->ignore();
        }
    }
}



void StrokeTrainingWindow::P1W1Changer_Prog1(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 1, pwvalue);
    //ui->label_pw1->setText(QString::number(pwvalue));
}

void StrokeTrainingWindow::P1W2Changer_Prog1(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelTwo, 1, pwvalue);
    //ui->label_pw2->setText(QString::number(pwvalue));
}


void StrokeTrainingWindow::nextButton_Prog1(int pwvalue)
{
    m_currentBtn1++;
    if(m_currentBtn1 >= btnGrp1->buttons().size())
    {
        m_currentBtn1 = 0;
        phaseOver = true;
      //  getRampStepSize();
        saveToXMLFile();
        configStatusId =1;
        ManageConfigFile configFile;

       // configFile.keyGripFinal(pLabel);

    }

    emit nxtButtonChanged_prog1( m_currentBtn1, pwvalue);
}

bool StrokeTrainingWindow::setComboBoxFromXml(QComboBox *comboBox, QDomElement &root, const QString &tagName, const QString &suffix)
{
    QDomNode node = root.elementsByTagName(tagName).at(0).firstChild();
       if (!node.isNull()) {
           comboBox->setCurrentIndex(comboBox->findText(node.nodeValue() + suffix));
           return true;
       }
       return false;
}



void StrokeTrainingWindow::on_pushButton_back1_clicked()
{

    ManageConfigFile currentConfigFile;
    currentConfigFile.currentTest(pLabel);
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}


void StrokeTrainingWindow::on_pushButton_back2_clicked()
{
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}


void StrokeTrainingWindow::on_pushButton_back3_clicked()
{
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}


void StrokeTrainingWindow::on_pushButton_back4_clicked()
{
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}


void StrokeTrainingWindow::on_pushButton_back5_clicked()
{
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}

void StrokeTrainingWindow::setupUiComponents(QMap<QString, QString> &defaultValues)
{


    ui->label_p1curr1->setText("Ch1: "+defaultValues["Current_CH1"]+ "mA");
    ui->label_p1curr2->setText("Ch2: "+defaultValues["Current_CH2"]+ "mA");
    ui->label_p1curr1_2->setText(defaultValues["Tooltip_comboBox_m1"]);
    ui->label_p1curr1_3->setText(defaultValues["Tooltip_comboBox_m2"]);

    for (int i = 0; i <= 400; ++i) {
        ui->comboBox_p1pw1->addItem(QString::number(i) + "uS", QVariant(i));
        ui->comboBox_p1pw2->addItem(QString::number(i) + "uS", QVariant(i));

     }



    //ui->label->setText(defaultValues["Current_CH1"]);


    QComboBox* rampComboBoxes[] {ui->comboBox_p1rr1, ui->comboBox_p1rf1,  ui->comboBox_p1rf2, ui->comboBox_p1rr2};
    const double rampValues[] {0.2 , 0.5 , 1 , 1.5 , 2};
    for(auto* cb : rampComboBoxes)
    {
        for(auto val : rampValues)
        {
            cb->addItem(QString::number(val*1000) + "ms", QVariant(val));
           // cb->setCurrentIndex(2); // default 1s
        }
    }

    QComboBox* delayComboBoxes[] { ui->comboBox_p1d2 };
    const double delayValues[] {0, 0.5, 1 ,1.5, 2, 3 , 4 , 5,6};
    for(auto* cb : delayComboBoxes)
    {
        for(auto val : delayValues)
        {
            cb->addItem(QString::number(val*1000) + "ms", QVariant(val));
           // cb->setCurrentIndex(2); // default 1s
        }
    }

    QComboBox* trainingDurComboBoxes[] { ui->comboBox_p1dur };
    const double trainingValues[] {5,10,15,20,30,60};
    for(auto* cb : trainingDurComboBoxes)
    {
        for(auto val : trainingValues)
        {
            cb->addItem(QString::number(val) + "min", QVariant(val));
           // cb->setCurrentIndex(2); // default 1s
        }
    }

    QComboBox* timeoutDurComboBoxes[] { ui->comboBox_onTime1 };
    const double timeoutValues[] {2,3,4,5,6,7,8};
    for(auto* cb : timeoutDurComboBoxes)
    {
        for(auto val : timeoutValues)
        {
            cb->addItem(QString::number(val*1000) + "ms", QVariant(val));
          // cb->setCurrentIndex(1); // default 0 --> no timeout
        }
    }



    if(defaultValues.value("PW_Stroke1_PW_Ch1").isEmpty())
    {
        ui->comboBox_p1pw1->setCurrentIndex(180);
        ui->comboBox_p1pw2->setCurrentIndex(180);

        ui->comboBox_p1rr1->setCurrentIndex(2);
        ui->comboBox_p1rr2->setCurrentIndex(2);

        ui->comboBox_p1rf1->setCurrentIndex(2);
        ui->comboBox_p1rf2->setCurrentIndex(2);

        ui->comboBox_onTime1->setCurrentIndex(1);
        ui->comboBox_p1dur->setCurrentIndex(2);
        ui->comboBox_p1d2->setCurrentIndex(6);

    }
    else
    {
        ui->comboBox_p1pw1->setCurrentText(defaultValues["PW_Stroke1_PW_Ch1"]+ "uS");
        ui->comboBox_p1pw2->setCurrentText(defaultValues["PW_Stroke1_PW_Ch2"]+ "uS");

        ui->comboBox_p1rr1->setCurrentText(QString::number(defaultValues["Ramp_Stroke1_RR_Ch1"].toFloat()*1000)+"ms");
        ui->comboBox_p1rr2->setCurrentText(QString::number(defaultValues["Ramp_Stroke1_RR_Ch2"].toFloat()*1000)+"ms");

        ui->comboBox_p1rf1->setCurrentText(QString::number(defaultValues["Ramp_Stroke1_RF_Ch1"].toFloat()*1000)+"ms");
        ui->comboBox_p1rf2->setCurrentText(QString::number(defaultValues["Ramp_Stroke1_RF_Ch2"].toFloat()*1000)+"ms");

        ui->comboBox_onTime1->setCurrentText(defaultValues["TDur_Stroke1_OnTime1"]+ "ms");
        ui->comboBox_p1dur->setCurrentText(defaultValues["TDur_Stroke1_Duration"]+ "min");
        ui->comboBox_p1d2->setCurrentText(defaultValues["Delay_Stroke1_Delay_Ch2"]+ "ms");
    }


    btnGrp1 = new QButtonGroup(this);

    btnGrp1->addButton(ui->btn11, 1);
    btnGrp1->addButton(ui->btn12, 2);
    btnGrp1->addButton(ui->btn13, 3);
}

void StrokeTrainingWindow::readXmlFile()
{
    QString xmlReadPath = QCoreApplication::applicationDirPath() + "/data/" + pLabel + ".xml";
    QFile xmlfile(xmlReadPath);

    if (!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error opening XML file: " << xmlfile.errorString();
        return;
    }

    QDomDocument document;
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();
    QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke1").at(0).firstChild();
    QDomElement SP1PWNodeVal = SP1PWNode.toElement();
    QDomNode RNode = root.elementsByTagName("Ramp_Stroke1").at(0).firstChild();
    QDomElement RNodeVal = RNode.toElement();
    QDomNode DelayNode = root.elementsByTagName("Delay_Stroke1").at(0).firstChild();
    QDomElement DelayNodeVal = DelayNode.toElement();
    QDomNode TDurNode = root.elementsByTagName("TDur_Stroke1").at(0).firstChild();
    QDomElement TDurNodeVal = TDurNode.toElement();


    if (!CurrentNodeVal.isNull())
    {
        currOneStored = root.elementsByTagName("CH1").at(0).firstChild().nodeValue().toFloat();
        currTwoStored = root.elementsByTagName("CH2").at(0).firstChild().nodeValue().toFloat();
        currThreeStored = root.elementsByTagName("CH3").at(0).firstChild().nodeValue().toFloat();
        currFourStored = root.elementsByTagName("CH4").at(0).firstChild().nodeValue().toFloat();
        currFiveStored = root.elementsByTagName("CH5").at(0).firstChild().nodeValue().toFloat();


        ui->label_p1curr1->setText(QString("Ch1: %1mA").arg(currOneStored));
        ui->label_p1curr2->setText(QString("Ch2: %1mA").arg(currTwoStored));
//        ui->label_p1curr3->setText(QString("Ch3: %1mA").arg(currThreeStored));
//        ui->label_p1curr4->setText(QString("Ch4: %1mA").arg(currFourStored));
//        ui->label_p1curr5->setText(QString("Ch5: %1mA").arg(currFiveStored));
    }

    if (!SP1PWNodeVal.isNull())
    {
        PW_Ch1 = root.elementsByTagName("PW_Ch1").at(0).firstChild().nodeValue().toInt();
        PW_Ch2 = root.elementsByTagName("PW_Ch2").at(0).firstChild().nodeValue().toInt();
//        ui->comboBox_p1pw1->setCurrentText(QString::number(PW_Ch1)+"uS");
//        ui->comboBox_p1pw2->setCurrentText(QString::number(PW_Ch2)+"uS");
        ui->comboBox_p1pw1->setCurrentIndex(ui->comboBox_p1pw1->findText(QString::number(PW_Ch1)+"uS"));
        ui->comboBox_p1pw2->setCurrentIndex(ui->comboBox_p1pw2->findText(QString::number(PW_Ch2)+"uS"));

        //ui->label_page1->setText(QString::number(PW_Ch1));
//        ui->comboBox_p1pw2->setCurrentIndex(ui->comboBox_p1pw2->findText(QString::number(PW_Ch2)+"uS"));
//        ui->comboBox_p1pw1->setCurrentIndex(ui->comboBox_p1pw1->findText(QString::number(PW_Ch1)+"uS"));


    }
    else
    {
//        ui->comboBox_p1pw1->setCurrentIndex(180);
//        ui->comboBox_p1pw2->setCurrentIndex(180);
    }

    if (!RNodeVal.isNull())
    {
        RR_Ch1 = root.elementsByTagName("RR_Ch1").at(0).firstChild().nodeValue().toFloat();
        RR_Ch2 = root.elementsByTagName("RR_Ch2").at(0).firstChild().nodeValue().toFloat();
        RF_Ch1 = root.elementsByTagName("RF_Ch1").at(0).firstChild().nodeValue().toFloat();
        RF_Ch2 = root.elementsByTagName("RF_Ch2").at(0).firstChild().nodeValue().toFloat();
        ui->comboBox_p1rr1->setCurrentIndex(ui->comboBox_p1rr1->findText(QString::number(RR_Ch1)+"ms"));
        ui->comboBox_p1rr2->setCurrentIndex(ui->comboBox_p1rr2->findText(QString::number(RR_Ch2)+"ms"));
        ui->comboBox_p1rf1->setCurrentIndex(ui->comboBox_p1rf1->findText(QString::number(RF_Ch1)+"ms"));
        ui->comboBox_p1rf2->setCurrentIndex(ui->comboBox_p1rf2->findText(QString::number(RF_Ch2)+"ms"));

    }
    else
    {
//        ui->comboBox_p1rr1->setCurrentIndex(2);
//        ui->comboBox_p1rr2->setCurrentIndex(2);
//        ui->comboBox_p1rf1->setCurrentIndex(2);
//        ui->comboBox_p1rf2->setCurrentIndex(2);
    }

    if (!DelayNodeVal.isNull())
    {
        Delay_Ch2 = root.elementsByTagName("Delay_Ch2").at(0).firstChild().nodeValue().toFloat();
        //ui->comboBox_p1d2->setCurrentText(QString::number(Delay_Ch2)+"ms");
        ui->comboBox_p1d2->setCurrentIndex(ui->comboBox_p1d2->findText(QString::number(Delay_Ch2)+"ms"));
    }
    else
    {
       // ui->comboBox_p1d2->setCurrentIndex(2);
    }

    if (!TDurNodeVal.isNull())
    {
        Training_Duration1 = root.elementsByTagName("Duration").at(0).firstChild().nodeValue().toFloat();
        ui->comboBox_p1dur->setCurrentIndex(ui->comboBox_p1dur->findText(QString::number(Training_Duration1)+"ms"));
    }
    else
    {
       //  ui->comboBox_p1dur->setCurrentIndex(2);
    }

//    if(progPage ==0)
//    {

//        test_config_file_pr1 = "config_sProg1_test_"+pLabel;
//        config_file_pr1 = "config_sProg1_final_"+pLabel;
//        if(!SP1PWNode.isNull())
//        {
//           // loadConfigFile(config_file_pr1); // change this later
//            loadConfigFile(config_file_pr1);

//            buildNewConfig1 = 1;
//        }

//        else
//        {
//            loadConfigFile(test_config_file_pr1);
//        }
//    }

}

void StrokeTrainingWindow::configureUiComponents()
{
    ui->btn_nextPhase1->setEnabled(false);
}

void StrokeTrainingWindow::setUiElementsEnabled(bool enabled)
{
    ui->comboBox_p1rr1->setEnabled(enabled);
    ui->comboBox_p1rf1->setEnabled(enabled);
    ui->comboBox_p1rr2->setEnabled(enabled);
    ui->comboBox_p1rf2->setEnabled(enabled);
    ui->comboBox_p1d2->setEnabled(enabled);
    ui->comboBox_p1dur->setEnabled(enabled);
    ui->pushButton_back1->setEnabled(enabled);
    ui->pushButton_download1->setEnabled(enabled);
    ui->groupBox->setEnabled(enabled);
}

void StrokeTrainingWindow::configureConnections()
{
    connect(&api, &tetra_grip_api::tetraGripEvent,this, &StrokeTrainingWindow::strokeTrainingPhaseEventHandler);
    connect(this, &StrokeTrainingWindow::P1W1_Value_Prog1, this, &StrokeTrainingWindow::P1W1Changer_Prog1);
    connect(this, &StrokeTrainingWindow::P1W2_Value_Prog1, this, &StrokeTrainingWindow::P1W2Changer_Prog1);
//connect(this, &StrokeTrainingWindow::settingsLoaded, this, &StrokeTrainingWindow::updateUiWithSettings);

}

void StrokeTrainingWindow::loadConfigFile(QString filename)
{
    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+filename+".txt";
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

void StrokeTrainingWindow::downloadConfigFile(QString filename)
{
    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+filename+".txt";
    //  QString lastWord = getLastWordInFilePath(txtWritePath);

    QFileInfo fileInfo(txtWritePath); // Extract file info
    QString baseName = fileInfo.fileName(); // Get the last part of the file path (file name)

     QFile f(txtWritePath);
    if(!f.open(QFile::ReadOnly))
    {
        QMessageBox::information(0, "config file error", f.errorString());
    }
    else
    {
        QByteArray config = f.readAll();
        QMessageBox::StandardButton reply;
        // tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());
        reply = QMessageBox::question(this, "Test App", "Are you sure want to permanently load "+baseName+" to the stimulator?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            tetra_grip_api::send_long_register(STIM_LONG_REG_NV_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());// send config file as non volatile
        }
        else if(reply == QMessageBox::No) {
            return;
        }
    }

}

void StrokeTrainingWindow::saveToXMLFile()
{


   loadCurrentWidgetValues();

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

    QDomElement newSP1PWTag = document.createElement(QString("PW_Stroke1"));
    QDomElement newRTag = document.createElement(QString("Ramp_Stroke1"));
    QDomElement newDelayTag = document.createElement(QString("Delay_Stroke1"));
    QDomElement newTDur1Tag = document.createElement(QString("TDur_Stroke1"));
    QDomElement newRR1Tag = document.createElement(QString("Ramp_Step_Stroke1"));


    QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke1").at(0).firstChild();
    QDomElement SP1PWNodeVal = SP1PWNode.toElement();
    QDomNode RNode = root.elementsByTagName("Ramp_Stroke1").at(0).firstChild();
    QDomElement RNodeVal = RNode.toElement();
    QDomNode DelayNode = root.elementsByTagName("Delay_Stroke1").at(0).firstChild();
    QDomElement DelayNodeVal = DelayNode.toElement();
    QDomNode TDurNode = root.elementsByTagName("TDur_Stroke1").at(0).firstChild();
    QDomElement TDurNodeVal = TDurNode.toElement();
    QDomNode RR1Node = root.elementsByTagName("Ramp_Step_Stroke1").at(0).firstChild();
    QDomElement RR1NodeVal = RR1Node.toElement();


    if (SP1PWNodeVal.isNull())
    {
        QDomElement SP1_PW1Tag = document.createElement(QString("PW_Ch1"));
        QDomText SP1_PW1Val = document.createTextNode(QString::number(PW_Ch1));
        SP1_PW1Tag.appendChild(SP1_PW1Val);
        newSP1PWTag.appendChild(SP1_PW1Tag);

        QDomElement SP1_PW2Tag = document.createElement(QString("PW_Ch2"));
        QDomText SP1_PW2Val = document.createTextNode(QString::number(PW_Ch2));
        SP1_PW2Tag.appendChild(SP1_PW2Val);
        newSP1PWTag.appendChild(SP1_PW2Tag);

        root.appendChild(newSP1PWTag);
    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("PW_Stroke1");
          QDomNode pw1 = SettingsNode.namedItem("PW_Ch1");
          pw1.firstChild().setNodeValue(QString::number(PW_Ch1));
          QDomNode pw2 = SettingsNode.namedItem("PW_Ch2");
          pw2.firstChild().setNodeValue(QString::number(PW_Ch2));

    }

     if(RNodeVal.isNull())
    {
        QDomElement SP1_Rr1Tag = document.createElement(QString("RR_Ch1"));
        QDomText SP1_Rr1Val = document.createTextNode(QString::number(RR_Ch1));
        SP1_Rr1Tag .appendChild(SP1_Rr1Val);
        newRTag.appendChild(SP1_Rr1Tag);

        QDomElement SP1_Rf1Tag = document.createElement(QString("RF_Ch1"));
        QDomText SP1_Rf1Val = document.createTextNode(QString::number(RF_Ch1));
        SP1_Rf1Tag .appendChild(SP1_Rf1Val);
        newRTag.appendChild(SP1_Rf1Tag);

        QDomElement SP1_Rr2Tag = document.createElement(QString("RR_Ch2"));
        QDomText SP1_Rr2Val = document.createTextNode(QString::number(RR_Ch2));
        SP1_Rr2Tag .appendChild(SP1_Rr2Val);
        newRTag.appendChild(SP1_Rr2Tag);

        QDomElement SP1_Rf2Tag = document.createElement(QString("RF_Ch2"));
        QDomText SP1_Rf2Val = document.createTextNode(QString::number(RF_Ch2));
        SP1_Rf2Tag .appendChild(SP1_Rf2Val);
        newRTag.appendChild(SP1_Rf2Tag);



        root.appendChild(newRTag);

    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode RampSettingsNode = root.namedItem("Ramp_Stroke1");

          QDomNode rmp1 = RampSettingsNode.namedItem("RR_Ch1");
          rmp1.firstChild().setNodeValue(QString::number(RR_Ch1));
          QDomNode rmp2 = RampSettingsNode.namedItem("RF_Ch1");
          rmp2.firstChild().setNodeValue(QString::number(RF_Ch1));
          QDomNode rmp3 = RampSettingsNode.namedItem("RR_Ch2");
          rmp3.firstChild().setNodeValue(QString::number(RR_Ch2));
          QDomNode rmp4 = RampSettingsNode.namedItem("RF_Ch2");
          rmp4.firstChild().setNodeValue(QString::number(RF_Ch2));

    }

     if(DelayNodeVal.isNull())
    {
        QDomElement SP1_Delay2Tag = document.createElement(QString("Delay_Ch2"));
        QDomText SP1_Delay2Val = document.createTextNode(QString::number(Delay_Ch2));
        SP1_Delay2Tag .appendChild(SP1_Delay2Val);
        newDelayTag.appendChild(SP1_Delay2Tag);

        root.appendChild(newDelayTag);

    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode RampSettingsNode = root.namedItem("Delay_Stroke1");

          QDomNode d2 = RampSettingsNode.namedItem("Delay_Ch2");
          d2.firstChild().setNodeValue(QString::number(Delay_Ch2));

    }

     if(TDurNodeVal.isNull())
    {
        QDomElement TDurTag = document.createElement(QString("Duration"));
        QDomText TDurVal = document.createTextNode(QString::number(Training_Duration1));
        TDurTag .appendChild(TDurVal);
        newTDur1Tag.appendChild(TDurTag);

        QDomElement OnTimeTag = document.createElement(QString("OnTime1"));
        QDomText OnTimeVal = document.createTextNode(QString::number(onTime1));
        OnTimeTag .appendChild(OnTimeVal);
        newTDur1Tag.appendChild(OnTimeTag);

        root.appendChild(newTDur1Tag);

    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode TDurNode = root.namedItem("TDur_Stroke1");

          QDomNode d2 = TDurNode.namedItem("Duration");
          d2.firstChild().setNodeValue(QString::number(Training_Duration1));

          QDomNode d3 = TDurNode.namedItem("OnTime1");
          d3.firstChild().setNodeValue(QString::number(onTime1));

    }

     if(RR1NodeVal.isNull())
     {
         QDomElement SP1_Rr1Tag = document.createElement(QString("RR_Step_Ch1"));
         QDomText SP1_Rr1Val = document.createTextNode(QString::number(ramp_step_size_rr1));
         SP1_Rr1Tag .appendChild(SP1_Rr1Val);
         newRR1Tag.appendChild(SP1_Rr1Tag);

         QDomElement SP1_Rf1Tag = document.createElement(QString("RF_Step_Ch1"));
         QDomText SP1_Rf1Val = document.createTextNode(QString::number(ramp_step_size_rf1));
         SP1_Rf1Tag .appendChild(SP1_Rf1Val);
         newRR1Tag.appendChild(SP1_Rf1Tag);

         QDomElement SP1_Rr2Tag = document.createElement(QString("RR_Step_Ch2"));
         QDomText SP1_Rr2Val = document.createTextNode(QString::number(ramp_step_size_rr2));
         SP1_Rr2Tag .appendChild(SP1_Rr2Val);
         newRR1Tag.appendChild(SP1_Rr2Tag);

         QDomElement SP1_Rf2Tag = document.createElement(QString("RF_Step_Ch2"));
         QDomText SP1_Rf2Val = document.createTextNode(QString::number(ramp_step_size_rf2));
         SP1_Rf2Tag .appendChild(SP1_Rf2Val);
         newRR1Tag.appendChild(SP1_Rf2Tag);



         root.appendChild(newRR1Tag);

     }
     else
     {
         QDomElement root = document.documentElement();
         QDomNode RampSettingsNode = root.namedItem("Ramp_Step_Stroke1");

         QDomNode rmp1 = RampSettingsNode.namedItem("RR_Step_Ch1");
         rmp1.firstChild().setNodeValue(QString::number(ramp_step_size_rr1));
         QDomNode rmp2 = RampSettingsNode.namedItem("RF_Step_Ch1");
         rmp2.firstChild().setNodeValue(QString::number(ramp_step_size_rf1));
         QDomNode rmp3 = RampSettingsNode.namedItem("RR_Step_Ch2");
         rmp3.firstChild().setNodeValue(QString::number(ramp_step_size_rr2));
         QDomNode rmp4 = RampSettingsNode.namedItem("RF_Step_Ch2");
         rmp4.firstChild().setNodeValue(QString::number(ramp_step_size_rf2));

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

void StrokeTrainingWindow::loadCurrentWidgetValues()
{
    QVariant PW_Ch1Data =  ui->comboBox_p1pw1->currentData();
    QVariant PW_Ch2Data =  ui->comboBox_p1pw2->currentData();
    QVariant RR_Ch1Data =  ui->comboBox_p1rr1->currentData();
    QVariant  RR_Ch2Data =  ui->comboBox_p1rr2->currentData();
    QVariant   RF_Ch1Data =  ui->comboBox_p1rf1->currentData();
    QVariant  RF_Ch2Data =  ui->comboBox_p1rf2->currentData();
    QVariant  Delay_Ch2Data =  ui->comboBox_p1d2->currentData();
    QVariant  Duration1_Data =  ui->comboBox_p1dur->currentData();
    QVariant  OnTime1_Data =  ui->comboBox_onTime1->currentData();

    PW_Ch1 = PW_Ch1Data.toInt();
    PW_Ch2 = PW_Ch2Data.toInt();
    RR_Ch1 = RR_Ch1Data.toFloat();
    RR_Ch2 = RR_Ch2Data.toFloat();
    RF_Ch1 = RF_Ch1Data.toFloat();
    RF_Ch2 = RF_Ch2Data.toFloat();
    onTime1 = OnTime1_Data.toFloat()*1000;


    Delay_Ch2 = Delay_Ch2Data.toFloat()*1000;
    Training_Duration1 = Duration1_Data.toFloat();

    ramp_step_size_rr1 = adjust_Ramp_Step_size(PW_Ch1,RR_Ch1);
    ramp_step_size_rr2 = adjust_Ramp_Step_size(PW_Ch2,RR_Ch2);
    ramp_step_size_rf1 = adjust_Ramp_Step_size(PW_Ch1,RF_Ch1);
    ramp_step_size_rf2 = adjust_Ramp_Step_size(PW_Ch2,RF_Ch2);
}

int StrokeTrainingWindow::adjust_Ramp_Step_size(int pwvalue, float rmpvalue)
{
    int rmp_step_size = static_cast<int>(1000 * (pwvalue / (rmpvalue * 30.0)));
    return rmp_step_size;
}


void StrokeTrainingWindow::on_btn_nextPhase1_clicked()
{
    tetra_grip_api::send_event(0, 253);

    m_currentBtn1++;
    if(m_currentBtn1 >= btnGrp1->buttons().size())
    {
        m_currentBtn1 = 0;
        phaseOver = true;
      //  getRampStepSize();
        saveToXMLFile();
        ManageConfigFile configFile;
        configFile.sProg1Final(pLabel);
        configStatusId =1;

    }
}


void StrokeTrainingWindow::on_pushButton_stimStart1_clicked()
{




    tetra_grip_api::stimulation_start(true);

    ui->btn_nextPhase1 ->setEnabled(true);

    setUiElementsEnabled(false);
    ui->pushButton_download1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(false);
}


void StrokeTrainingWindow::on_pushButton_stimStop1_clicked()
{
    tetra_grip_api::stimulation_pause(true);
    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
    ui->btn_nextPhase1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);

    m_currentBtn1 = 0;



//    ManageConfigFile configFile;
//    configFile.sProg1Final(pLabel);
//    loadConfigFile(config_file_pr1);


    setUiElementsEnabled(true);
  //  ui->pushButton_triggerApply->setEnabled(false);


    ui->btn11->setStyleSheet(StyleSheetOff);

    ui->btn12->setStyleSheet(StyleSheetOff);

    ui->btn13->setStyleSheet(StyleSheetOff);

    //ui->label_page1->setText("Hello");




    update();

}


void StrokeTrainingWindow::on_comboBox_p1pw1_activated(int index)
{
    qDebug() << "the PW combobox activated at index: "<<index;
}


void StrokeTrainingWindow::on_comboBox_p1pw1_currentIndexChanged(int index)
{
//    int value = ui->comboBox_p1pw1->itemData(index).toInt();
    PW_Ch1 = index;
    emit P1W1_Value_Prog1(index);
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);
}


void StrokeTrainingWindow::on_comboBox_p1rr1_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindow::on_comboBox_p1rf1_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindow::on_comboBox_p1pw2_currentIndexChanged(int index)
{
  //  int value = ui->comboBox_p1pw2->itemData(index).toInt();
    PW_Ch2 = index;

  //  ui->label_pw2->setText(QString::number(index));

    emit P1W2_Value_Prog1(index);
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);
}


void StrokeTrainingWindow::on_comboBox_p1rr2_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }

}


void StrokeTrainingWindow::on_comboBox_p1rf2_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindow::on_comboBox_p1d2_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindow::on_comboBox_p1dur_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg1Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
   }
}


void StrokeTrainingWindow::on_radioButton_p1t1_clicked()
{

    saveToXMLFile();
    triggerIMU = false;
    triggerTime= true;
    triggerButton = false;
    ui->comboBox_onTime1->setEnabled(true);
    //ui->pushButton_triggerApply->setEnabled(true);
    ui->toolButton_imu1->setEnabled(false);
    //ui->label_radio->setText(QString::number(triggerTime)+QString::number(triggerButton)+QString::number(triggerIMU));
//    ManageConfigFile configFile;
//    configFile.sProg1Final2(pLabel);
}


void StrokeTrainingWindow::on_radioButton_p1t2_clicked()
{

      saveToXMLFile();
     triggerIMU = false;
     triggerTime= false;
     triggerButton = true;
     ui->comboBox_onTime1->setEnabled(false);
    // ui->pushButton_triggerApply->setEnabled(true);
     ui->toolButton_imu1->setEnabled(false);
    // ui->label_radio->setText(QString::number(triggerTime)+QString::number(triggerButton)+QString::number(triggerIMU));
}


void StrokeTrainingWindow::on_radioButton_p1t3_clicked()
{

      saveToXMLFile();
     triggerIMU = true;
     triggerTime= false;
     triggerButton = false;
     ui->comboBox_onTime1->setEnabled(false);
     //ui->pushButton_triggerApply->setEnabled(true);
     ui->toolButton_imu1->setEnabled(true);
   //   ui->label_radio->setText(QString::number(triggerTime)+QString::number(triggerButton)+QString::number(triggerIMU));

}


/*void StrokeTrainingWindow::on_pushButton_triggerApply_clicked()
{

//    config_file_pr1 = "config_sProg1_final2_"+pLabel;
//    loadConfigFile(config_file_pr1);



    if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
        ManageConfigFile configFile;
        configFile.sProg1Final(pLabel);
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
        ManageConfigFile configFile;
        configFile.sProg1Final2(pLabel);
    }
    else if (triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)


        {

//        ui->label->setText(QString::number(triggerIMUWrist) + QString::number(triggerIMUShoulder));
//        ui->label_8->setText(QString::number(triggerIMUtimeout));
            if (triggerIMUWrist == 1 && triggerIMUShoulder == 0 && triggerIMUtimeout == 0)
            {
                ManangeConfigExtended configFile;
                configFile.sProg1Final3_1_1(pLabel);

            }
            else if (triggerIMUWrist == 1 && triggerIMUShoulder == 0 && triggerIMUtimeout == 1)
            {
                ManangeConfigExtended configFile;
                configFile.sProg1Final3_1_2(pLabel);
            }
            else if (triggerIMUWrist == 0 && triggerIMUShoulder == 1 && triggerIMUtimeout == 0)
            {
                ManangeConfigExtended configFile;
                configFile.sProg1Final3_2_1(pLabel);
            }
            else if (triggerIMUWrist == 0 && triggerIMUShoulder == 1 && triggerIMUtimeout == 1)
            {
                ManangeConfigExtended configFile;
                configFile.sProg1Final3_2_2(pLabel);
            }
            else
            {
                ManageConfigFile configFile;
                configFile.sProg1Final3(pLabel);
            }
        }

    //ui->label_radio->setText("B:"+QString::number(triggerButton)+"T:"+QString::number(triggerTime));

    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);

}*/


void StrokeTrainingWindow::on_comboBox_onTime1_currentIndexChanged(int index)
{
    saveToXMLFile();
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
}

QString StrokeTrainingWindow::defaultValuesToString(const QMap<QString, QString> &defaultValues)
{

    QStringList list;
        QMapIterator<QString, QString> it(defaultValues);
        while (it.hasNext()) {
            it.next();
            list << it.key() + ": " + it.value();
        }
        return list.join("\n");
}

void StrokeTrainingWindow::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
     qreal scaleX = static_cast<qreal>(newSize.width()) / originalSize.width();
     qreal scaleY = static_cast<qreal>(newSize.height()) / originalSize.height();

     // Example of scaling a widget
     for (auto& widget : this->findChildren<QWidget*>()) {
         QRect originalGeometry = originalGeometries[widget];
         QRect newGeometry(
             static_cast<int>(originalGeometry.x() * scaleX),
             static_cast<int>(originalGeometry.y() * scaleY),
             static_cast<int>(originalGeometry.width() * scaleX),
             static_cast<int>(originalGeometry.height() * scaleY)
         );
         widget->setGeometry(newGeometry);

         // Scale the font size
                 QFont font = widget->font();
                 int originalFontSize = originalFontSizes[widget];
                 int newFontSize = static_cast<int>(originalFontSize * scaleY);
                 font.setPointSize(newFontSize);
                 widget->setFont(font);
     }

     // Scale the pixmap for the label
     int w = ui->label_bgs1->width();
     int h = ui->label_bgs1->height();
     //ui->label_bgs1->setPixmap(originalPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_bgs1->setPixmap(originalPixmap.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    // ui->label_bgs1->setPixmap(originalPixmap.scaled(w, h, Qt::IgnoreAspectRatio));

     QMainWindow::resizeEvent(event);
}


void StrokeTrainingWindow::on_pushButton_configSet_clicked()
{
    ui->pushButton_stimStart1->setEnabled(true);

     if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
         ManageConfigFile configFile;
         configFile.sProg1Final(pLabel);
         loadConfigFile(config_file_pr1);
       //ui->label->setText(t1+QString::number(triggerButton)+t2+QString::number(triggerTime)+t3+QString::number(triggerIMU));
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
         ManageConfigFile configFile;
         configFile.sProg1Final2(pLabel);
         config_file_pr12 = "config_sProg1_final2_"+pLabel;
        loadConfigFile(config_file_pr12);

    }


    else if(triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)
    {
      //  ui->label->setText(t1+QString::number(triggerButton)+t2+QString::number(triggerTime)+t3+QString::number(triggerIMU));
        if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==0)
        {
            ManangeConfigExtended configFile;
            configFile.sProg1Final3_1_1(pLabel);
            config_file_pr13 = "config_sProg1_final311_"+pLabel;
            loadConfigFile(config_file_pr13);
        }
        else if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==1)
        {
            ManangeConfigExtended configFile;
            configFile.sProg1Final3_1_2(pLabel);
            config_file_pr13 = "config_sProg1_final312_"+pLabel;
            loadConfigFile(config_file_pr13);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==0)
        {
            ManangeConfigExtended configFile;
            configFile.sProg1Final3_2_1(pLabel);
            config_file_pr13 = "config_sProg1_final321_"+pLabel;
            loadConfigFile(config_file_pr13);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==1)
        {
            ManangeConfigExtended configFile;
            configFile.sProg1Final3_2_2(pLabel);
            config_file_pr13 = "config_sProg1_final322_"+pLabel;
            loadConfigFile(config_file_pr13);
        }

        else
        {
            ManageConfigFile configFile;
            configFile.sProg1Final3(pLabel);
            config_file_pr13 = "config_sProg1_final3_"+pLabel;
            loadConfigFile(config_file_pr13);
        }
    }

    else
    {
        loadConfigFile(config_file_pr1);
    }

}


void StrokeTrainingWindow::on_comboBox_onTime1_activated(const QString &arg1)
{

}


void StrokeTrainingWindow::on_pushButton_download1_clicked()
{
    if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
        downloadConfigFile(config_file_pr1);
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
        config_file_pr12 = "config_sProg1_final2_"+pLabel;
        downloadConfigFile(config_file_pr12);
    }
    else if(triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)
    {
        if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==0)
        {
            config_file_pr13 = "config_sProg1_final311_"+pLabel;
            downloadConfigFile(config_file_pr13);
        }
        else if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==1)
        {
            config_file_pr13 = "config_sProg1_final312_"+pLabel;
            downloadConfigFile(config_file_pr13);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==0)
        {
            config_file_pr13 = "config_sProg1_final321_"+pLabel;
            downloadConfigFile(config_file_pr13);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==1)
        {
            config_file_pr13 = "config_sProg1_final322_"+pLabel;
            downloadConfigFile(config_file_pr13);
        }

        else
        {
            config_file_pr13 = "config_sProg1_final3_"+pLabel;
            downloadConfigFile(config_file_pr13);
        }
    }
    else
    {
        downloadConfigFile(config_file_pr1);
    }



    if (resetLogStatus==true)
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

}


void StrokeTrainingWindow::on_toolButton_imu1_triggered(QAction *arg1)
{

}


void StrokeTrainingWindow::on_toolButton_imu1_clicked()
{
    //ui->pushButton_triggerApply->setEnabled(true);
    IMUDialog *newdialog;
    //this->hide();
    newdialog = new IMUDialog( pLabel, nullptr);

    connect(newdialog, &IMUDialog::radioButtonChanged, this, &StrokeTrainingWindow::IMUExtSettings);
    connect(newdialog, &IMUDialog::checkBoxChanged, this, &StrokeTrainingWindow::IMUExtSettingsCheckBox);

    newdialog->setAttribute(Qt::WA_DeleteOnClose);
    newdialog -> show();
}

void StrokeTrainingWindow::IMUExtSettings(const QString &buttonText)
{



    if (buttonText ==  "Wrist Extension")
    {
        triggerIMUWrist = true;
        triggerIMUShoulder = false;
    }
    else if (buttonText == "Arm Flexion")
    {
        triggerIMUWrist = false;
        triggerIMUShoulder = true;
    }


}

void StrokeTrainingWindow::IMUExtSettingsCheckBox(bool status)
{


    if (status ==true)
    {
        triggerIMUtimeout = true;
    }
    else if (status == false)
    {
        triggerIMUtimeout = false;
    }


}


void StrokeTrainingWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)  // Qt::Checked is 2
    {
        resetLogStatus = true;
    }
    else if (arg1 == Qt::Unchecked)  // Qt::Unchecked is 0
    {
        resetLogStatus = false;
    }
}

