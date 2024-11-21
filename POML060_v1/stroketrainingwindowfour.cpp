#include "stroketrainingwindowfour.h"
#include "ui_stroketrainingwindowfour.h"
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

StrokeTrainingWindowFour::StrokeTrainingWindowFour(QString patientLabel,QMap<QString, QString> defaultValues, int page, bool configId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StrokeTrainingWindowFour)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    progPage = page;
    configStatusId = configId;
    m_currentBtn1 = 0;
    ui->btn0->setVisible(false);
    ui->radioButton_p1t2->setChecked(true);
    //ui->pushButton_triggerApply->setEnabled(false);
   // ui->pushButton_configSet->setEnabled(false);

    // Save the original size of the window
   originalSize = this->size();

   // Save the original geometry of each widget
      for (auto& widget : this->findChildren<QWidget*>()) {
          originalGeometries[widget] = widget->geometry();
          QFont font = widget->font();
          originalFontSizes[widget] = font.pointSize();
      }

    if (progPage == 0)
    {
        QPixmap pix(":/resources/envStrokeProg43.jpg"); // this is the code to add image to the form
        originalPixmap = pix; // Save the original pixmap
        int w = ui->label_bgs2->width();
        int h = ui->label_bgs2->height();

        ui->label_bgs2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        StyleSheetOn = "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                             "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)";
        StyleSheetOff = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                              "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                                      "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                              "color: #0000;";

        test_config_file_pr4 = "config_sProg4_test_"+pLabel;
        config_file_pr4 = "config_sProg4_final_"+pLabel;
          if (configId ==0)
        {
            loadConfigFile(test_config_file_pr4);
        }

        else
        {
            loadConfigFile(config_file_pr4);
        }
    }


    setupUiComponents(defaultValues);
    configureConnections();
    configureUiComponents();
}

StrokeTrainingWindowFour::~StrokeTrainingWindowFour()
{
    delete ui;
}

void StrokeTrainingWindowFour::strokeTrainingPhaseEventHandler(STIM_GUI_TOPIC_T topic, uint8_t index, uint8_t reg, uint32_t value)
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
                QList<QPushButton *> List{ui->btn11,ui->btn12,ui->btn13,ui->btn14,ui->btn15};
                for (QPushButton * button : List) {
                    button->setStyleSheet(StyleSheetOff);
                }
                List[value]->setStyleSheet(StyleSheetOn);

                break;

            }
        }
    }
}

void StrokeTrainingWindowFour::closeEvent(QCloseEvent *event)
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

void StrokeTrainingWindowFour::resizeEvent(QResizeEvent *event)
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
     int w = ui->label_bgs2->width();
     int h = ui->label_bgs2->height();
     //ui->label_bgs1->setPixmap(originalPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_bgs2->setPixmap(originalPixmap.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    // ui->label_bgs1->setPixmap(originalPixmap.scaled(w, h, Qt::IgnoreAspectRatio));

     QMainWindow::resizeEvent(event);
}

int StrokeTrainingWindowFour::adjust_Ramp_Step_size(int pwvalue, float rmpvalue)
{
    int rmp_step_size = static_cast<int>(1000 * (pwvalue / (rmpvalue * 30.0)));
    return rmp_step_size;
}

void StrokeTrainingWindowFour::P1W1Changer_Prog4(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelOne, 1, pwvalue);

}

void StrokeTrainingWindowFour::P1W2Changer_Prog4(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelTwo, 1, pwvalue);

}

void StrokeTrainingWindowFour::P1W3Changer_Prog4(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelThree, 1, pwvalue);

}

void StrokeTrainingWindowFour::P1W4Changer_Prog4(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelFour, 1, pwvalue);
}

void StrokeTrainingWindowFour::P1W5Changer_Prog4(int pwvalue)
{
    tetra_grip_api::set_stimulation_target_pulse_width( m_channelFive, 1, pwvalue);
}

void StrokeTrainingWindowFour::nextButton_Prog4(int pwvalue)
{
    m_currentBtn1++;
    if(m_currentBtn1 >= btnGrp1->buttons().size())
    {
        m_currentBtn1 = 0;
        phaseOver = true;
      //  getRampStepSize();
        saveToXMLFile();
        ManageConfigFile configFile;
       // configFile.keyGripFinal(pLabel);

    }
}

void StrokeTrainingWindowFour::on_pushButton_clicked()
{
    ManageConfigFile currentConfigFile;
    currentConfigFile.currentTest(pLabel);
    this->close();
    stageProgram *backprogram;
    backprogram = new stageProgram(pLabel,nullptr);
    backprogram->setAttribute(Qt::WA_DeleteOnClose);
    backprogram -> show();
}

QString StrokeTrainingWindowFour::defaultValuesToString(const QMap<QString, QString> &defaultValues)
{
    QStringList list;
        QMapIterator<QString, QString> it(defaultValues);
        while (it.hasNext()) {
            it.next();
            list << it.key() + ": " + it.value();
        }
        return list.join("\n");
}

void StrokeTrainingWindowFour::setupUiComponents(QMap<QString, QString> &defaultValues)
{
    ui->label_p4curr1->setText("Ch1: "+defaultValues["Current_CH1"]+ "mA");
    ui->label_p4curr2->setText("Ch2: "+defaultValues["Current_CH2"]+ "mA");
    ui->label_p4curr3->setText("Ch3: "+defaultValues["Current_CH3"]+ "mA");
    ui->label_p4curr4->setText("Ch4: "+defaultValues["Current_CH4"]+ "mA");
    ui->label_p4curr5->setText("Ch5: "+defaultValues["Current_CH5"]+ "mA");

    ui->label_p2curr1_2->setText(defaultValues["Tooltip_comboBox_m1"]);
    ui->label_p2curr1_3->setText(defaultValues["Tooltip_comboBox_m2"]);
    ui->label_p2curr1_4->setText(defaultValues["Tooltip_comboBox_m3"]);
    ui->label_p2curr1_5->setText(defaultValues["Tooltip_comboBox_m4"]);
    ui->label_p2curr1_6->setText(defaultValues["Tooltip_comboBox_m5"]);

    for (int i = 0; i <= 400; ++i) {
        ui->comboBox_p1pw1->addItem(QString::number(i) + "uS", QVariant(i));
        ui->comboBox_p1pw2->addItem(QString::number(i) + "uS", QVariant(i));
        ui->comboBox_p1pw3->addItem(QString::number(i) + "uS", QVariant(i));
        ui->comboBox_p1pw4->addItem(QString::number(i) + "uS", QVariant(i));
        ui->comboBox_p1pw5->addItem(QString::number(i) + "uS", QVariant(i));
     }



    //ui->label->setText(defaultValues["Current_CH1"]);


    QComboBox* rampComboBoxes[] { ui->comboBox_p1rr1, ui->comboBox_p1rf1,  ui->comboBox_p1rf2, ui->comboBox_p1rr2,ui->comboBox_p1rf3, ui->comboBox_p1rr3,ui->comboBox_p1rf4, ui->comboBox_p1rr4,ui->comboBox_p1rf5, ui->comboBox_p1rr5 };
    const double rampValues[] {0.2 , 0.5 , 1 , 1.5 , 2};
    for(auto* cb : rampComboBoxes)
    {
        for(auto val : rampValues)
        {
            cb->addItem(QString::number(val*1000) + "ms", QVariant(val));
           // cb->setCurrentIndex(2); // default 1s
        }
    }

//    QComboBox* delayComboBoxes[] { ui->comboBox_p1d2 };
//    const double delayValues[] {1 , 2 , 3 , 4 , 5,6};
//    for(auto* cb : delayComboBoxes)
//    {
//        for(auto val : delayValues)
//        {
//            cb->addItem(QString::number(val*1000) + "ms", QVariant(val));
//           // cb->setCurrentIndex(2); // default 1s
//        }
//    }

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



    if(defaultValues.value("PW_Stroke4_PW_Ch1").isEmpty())
    {
        ui->comboBox_p1pw1->setCurrentIndex(180);
        ui->comboBox_p1pw2->setCurrentIndex(180);
        ui->comboBox_p1pw3->setCurrentIndex(180);
        ui->comboBox_p1pw4->setCurrentIndex(180);
        ui->comboBox_p1pw5->setCurrentIndex(180);
        ui->comboBox_p1rr1->setCurrentIndex(2);
        ui->comboBox_p1rr2->setCurrentIndex(2);
        ui->comboBox_p1rr3->setCurrentIndex(2);
        ui->comboBox_p1rr4->setCurrentIndex(2);
        ui->comboBox_p1rr5->setCurrentIndex(2);
        ui->comboBox_p1rf1->setCurrentIndex(2);
        ui->comboBox_p1rf2->setCurrentIndex(2);
        ui->comboBox_p1rf3->setCurrentIndex(2);
        ui->comboBox_p1rf4->setCurrentIndex(2);
        ui->comboBox_p1rf5->setCurrentIndex(2);
        ui->comboBox_onTime1->setCurrentIndex(1);
        ui->comboBox_p1dur->setCurrentIndex(2);
      //  ui->comboBox_p1d2->setCurrentIndex(2);

    }
    else
    {
        ui->comboBox_p1pw1->setCurrentText(defaultValues["PW_Stroke4_PW_Ch1"]+ "uS");
        ui->comboBox_p1pw2->setCurrentText(defaultValues["PW_Stroke4_PW_Ch2"]+ "uS");
        ui->comboBox_p1pw3->setCurrentText(defaultValues["PW_Stroke4_PW_Ch3"]+ "uS");
        ui->comboBox_p1pw4->setCurrentText(defaultValues["PW_Stroke4_PW_Ch4"]+ "uS");
        ui->comboBox_p1pw5->setCurrentText(defaultValues["PW_Stroke4_PW_Ch5"]+ "uS");
        ui->comboBox_p1rr1->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RR_Ch1"].toFloat()*1000)+"ms");
        ui->comboBox_p1rr2->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RR_Ch2"].toFloat()*1000)+"ms");
        ui->comboBox_p1rr3->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RR_Ch3"].toFloat()*1000)+"ms");
        ui->comboBox_p1rr4->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RR_Ch4"].toFloat()*1000)+"ms");
        ui->comboBox_p1rr5->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RR_Ch5"].toFloat()*1000)+"ms");
        ui->comboBox_p1rf1->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RF_Ch1"].toFloat()*1000)+"ms");
        ui->comboBox_p1rf2->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RF_Ch2"].toFloat()*1000)+"ms");
        ui->comboBox_p1rf3->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RF_Ch3"].toFloat()*1000)+"ms");
        ui->comboBox_p1rf4->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RF_Ch4"].toFloat()*1000)+"ms");
        ui->comboBox_p1rf5->setCurrentText(QString::number(defaultValues["Ramp_Stroke4_RF_Ch5"].toFloat()*1000)+"ms");
        ui->comboBox_onTime1->setCurrentText(defaultValues["TDur_Stroke2_OnTime1"]+ "ms");
        ui->comboBox_p1dur->setCurrentText(defaultValues["TDur_Stroke2_Duration"]+ "min");
       // ui->comboBox_p1d2->setCurrentText(defaultValues["Delay_Stroke2_Delay_Ch2"]+ "ms");
    }


    btnGrp1 = new QButtonGroup(this);

    btnGrp1->addButton(ui->btn11, 1);
    btnGrp1->addButton(ui->btn12, 2);
    btnGrp1->addButton(ui->btn13, 3);
    btnGrp1->addButton(ui->btn14, 4);
    btnGrp1->addButton(ui->btn15, 5);
   // btnGrp1->addButton(ui->btn14, 4);
}

void StrokeTrainingWindowFour::configureUiComponents()
{
     ui->btn_nextPhase1->setEnabled(false);
}

void StrokeTrainingWindowFour::setUiElementsEnabled(bool enabled)
{
    ui->comboBox_p1rr1->setEnabled(enabled);
    ui->comboBox_p1rf1->setEnabled(enabled);
    ui->comboBox_p1rr2->setEnabled(enabled);
    ui->comboBox_p1rf2->setEnabled(enabled);
    ui->comboBox_p1rr3->setEnabled(enabled);
    ui->comboBox_p1rf3->setEnabled(enabled);
    ui->comboBox_p1rr4->setEnabled(enabled);
    ui->comboBox_p1rf4->setEnabled(enabled);
    ui->comboBox_p1rr5->setEnabled(enabled);
    ui->comboBox_p1rf5->setEnabled(enabled);
   // ui->comboBox_p1d2->setEnabled(enabled);
    ui->comboBox_p1dur->setEnabled(enabled);
    ui->pushButton->setEnabled(enabled);
    ui->pushButton_download1->setEnabled(enabled);
    ui->groupBox->setEnabled(enabled);
}

void StrokeTrainingWindowFour::configureConnections()
{
    connect(&api, &tetra_grip_api::tetraGripEvent,this, &StrokeTrainingWindowFour::strokeTrainingPhaseEventHandler);
    connect(this, &StrokeTrainingWindowFour::P1W1_Value_Prog4, this, &StrokeTrainingWindowFour::P1W1Changer_Prog4);
    connect(this, &StrokeTrainingWindowFour::P1W2_Value_Prog4, this, &StrokeTrainingWindowFour::P1W2Changer_Prog4);
    connect(this, &StrokeTrainingWindowFour::P1W3_Value_Prog4, this, &StrokeTrainingWindowFour::P1W3Changer_Prog4);
    connect(this, &StrokeTrainingWindowFour::P1W4_Value_Prog4, this, &StrokeTrainingWindowFour::P1W4Changer_Prog4);
    connect(this, &StrokeTrainingWindowFour::P1W5_Value_Prog4, this, &StrokeTrainingWindowFour::P1W5Changer_Prog4);
    //connect(this, &StrokeTrainingWindow::settingsLoaded, this, &StrokeTrainingWindow::updateUiWithSettings);
}

void StrokeTrainingWindowFour::loadConfigFile(QString filename)
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

void StrokeTrainingWindowFour::downloadConfigFile(QString filename)
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

void StrokeTrainingWindowFour::saveToXMLFile()
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

    QDomElement newSP3PWTag = document.createElement(QString("PW_Stroke4"));
    QDomElement newRTag = document.createElement(QString("Ramp_Stroke4"));
    QDomElement newDelayTag = document.createElement(QString("Delay_Stroke4"));
    QDomElement newTDur3Tag = document.createElement(QString("TDur_Stroke4"));
    QDomElement newRR3Tag = document.createElement(QString("Ramp_Step_Stroke4"));


    QDomNode SP3PWNode = root.elementsByTagName("PW_Stroke4").at(0).firstChild();
    QDomElement SP3PWNodeVal = SP3PWNode.toElement();
    QDomNode RNode = root.elementsByTagName("Ramp_Stroke4").at(0).firstChild();
    QDomElement RNodeVal = RNode.toElement();
    QDomNode DelayNode = root.elementsByTagName("Delay_Stroke4").at(0).firstChild();
    QDomElement DelayNodeVal = DelayNode.toElement();
    QDomNode TDurNode = root.elementsByTagName("TDur_Stroke4").at(0).firstChild();
    QDomElement TDurNodeVal = TDurNode.toElement();
    QDomNode RR3Node = root.elementsByTagName("Ramp_Step_Stroke4").at(0).firstChild();
    QDomElement RR3NodeVal = RR3Node.toElement();

    if (SP3PWNodeVal.isNull())
    {
        QDomElement SP1_PW1Tag = document.createElement(QString("PW_Ch1"));
        QDomText SP1_PW1Val = document.createTextNode(QString::number(PW_Ch1));
        SP1_PW1Tag.appendChild(SP1_PW1Val);
        newSP3PWTag.appendChild(SP1_PW1Tag);

        QDomElement SP1_PW2Tag = document.createElement(QString("PW_Ch2"));
        QDomText SP1_PW2Val = document.createTextNode(QString::number(PW_Ch2));
        SP1_PW2Tag.appendChild(SP1_PW2Val);
        newSP3PWTag.appendChild(SP1_PW2Tag);

        QDomElement SP1_PW3Tag = document.createElement(QString("PW_Ch3"));
        QDomText SP1_PW3Val = document.createTextNode(QString::number(PW_Ch2));
        SP1_PW3Tag.appendChild(SP1_PW3Val);
        newSP3PWTag.appendChild(SP1_PW3Tag);

        QDomElement SP1_PW4Tag = document.createElement(QString("PW_Ch4"));
        QDomText SP1_PW4Val = document.createTextNode(QString::number(PW_Ch4));
        SP1_PW4Tag.appendChild(SP1_PW4Val);
        newSP3PWTag.appendChild(SP1_PW4Tag);

        QDomElement SP1_PW5Tag = document.createElement(QString("PW_Ch5"));
        QDomText SP1_PW5Val = document.createTextNode(QString::number(PW_Ch5));
        SP1_PW5Tag.appendChild(SP1_PW5Val);
        newSP3PWTag.appendChild(SP1_PW5Tag);

        root.appendChild(newSP3PWTag);
    }


    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("PW_Stroke4");
          QDomNode pw1 = SettingsNode.namedItem("PW_Ch1");
          pw1.firstChild().setNodeValue(QString::number(PW_Ch1));
          QDomNode pw2 = SettingsNode.namedItem("PW_Ch2");
          pw2.firstChild().setNodeValue(QString::number(PW_Ch2));
          QDomNode pw3 = SettingsNode.namedItem("PW_Ch3");
          pw3.firstChild().setNodeValue(QString::number(PW_Ch3));
          QDomNode pw4 = SettingsNode.namedItem("PW_Ch4");
          pw4.firstChild().setNodeValue(QString::number(PW_Ch4));
          QDomNode pw5 = SettingsNode.namedItem("PW_Ch5");
          pw5.firstChild().setNodeValue(QString::number(PW_Ch5));

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

       QDomElement SP1_Rr3Tag = document.createElement(QString("RR_Ch3"));
       QDomText SP1_Rr3Val = document.createTextNode(QString::number(RR_Ch3));
       SP1_Rr3Tag .appendChild(SP1_Rr3Val);
       newRTag.appendChild(SP1_Rr3Tag);

       QDomElement SP1_Rf3Tag = document.createElement(QString("RF_Ch3"));
       QDomText SP1_Rf3Val = document.createTextNode(QString::number(RF_Ch3));
       SP1_Rf3Tag .appendChild(SP1_Rf3Val);
       newRTag.appendChild(SP1_Rf3Tag);

       QDomElement SP1_Rr4Tag = document.createElement(QString("RR_Ch4"));
       QDomText SP1_Rr4Val = document.createTextNode(QString::number(RR_Ch4));
       SP1_Rr4Tag .appendChild(SP1_Rr4Val);
       newRTag.appendChild(SP1_Rr4Tag);

       QDomElement SP1_Rf4Tag = document.createElement(QString("RF_Ch4"));
       QDomText SP1_Rf4Val = document.createTextNode(QString::number(RF_Ch4));
       SP1_Rf4Tag .appendChild(SP1_Rf4Val);
       newRTag.appendChild(SP1_Rf4Tag);

       QDomElement SP1_Rr5Tag = document.createElement(QString("RR_Ch5"));
       QDomText SP1_Rr5Val = document.createTextNode(QString::number(RR_Ch5));
       SP1_Rr5Tag .appendChild(SP1_Rr5Val);
       newRTag.appendChild(SP1_Rr5Tag);

       QDomElement SP1_Rf5Tag = document.createElement(QString("RF_Ch5"));
       QDomText SP1_Rf5Val = document.createTextNode(QString::number(RF_Ch5));
       SP1_Rf5Tag .appendChild(SP1_Rf5Val);
       newRTag.appendChild(SP1_Rf5Tag);


       root.appendChild(newRTag);

   }


   else
   {
         QDomElement root = document.documentElement();
         QDomNode RampSettingsNode = root.namedItem("Ramp_Stroke4");

         QDomNode rmp1 = RampSettingsNode.namedItem("RR_Ch1");
         rmp1.firstChild().setNodeValue(QString::number(RR_Ch1));
         QDomNode rmp2 = RampSettingsNode.namedItem("RF_Ch1");
         rmp2.firstChild().setNodeValue(QString::number(RF_Ch1));
         QDomNode rmp3 = RampSettingsNode.namedItem("RR_Ch2");
         rmp3.firstChild().setNodeValue(QString::number(RR_Ch2));
         QDomNode rmp4 = RampSettingsNode.namedItem("RF_Ch2");
         rmp4.firstChild().setNodeValue(QString::number(RF_Ch2));
         QDomNode rmp5 = RampSettingsNode.namedItem("RR_Ch3");
         rmp5.firstChild().setNodeValue(QString::number(RR_Ch3));
         QDomNode rmp6 = RampSettingsNode.namedItem("RF_Ch3");
         rmp6.firstChild().setNodeValue(QString::number(RF_Ch3));
         QDomNode rmp7 = RampSettingsNode.namedItem("RR_Ch4");
         rmp7.firstChild().setNodeValue(QString::number(RR_Ch4));
         QDomNode rmp8 = RampSettingsNode.namedItem("RF_Ch4");
         rmp8.firstChild().setNodeValue(QString::number(RF_Ch4));
         QDomNode rmp9 = RampSettingsNode.namedItem("RR_Ch5");
         rmp9.firstChild().setNodeValue(QString::number(RR_Ch5));
         QDomNode rmp10 = RampSettingsNode.namedItem("RF_Ch5");
         rmp10.firstChild().setNodeValue(QString::number(RF_Ch5));


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
         QDomNode RampSettingsNode = root.namedItem("Delay_Stroke4");

         QDomNode d2 = RampSettingsNode.namedItem("Delay_Ch2");
         d2.firstChild().setNodeValue(QString::number(Delay_Ch2));

   }

    if(TDurNodeVal.isNull())
   {
       QDomElement TDurTag = document.createElement(QString("Duration"));
       QDomText TDurVal = document.createTextNode(QString::number(Training_Duration1));
       TDurTag .appendChild(TDurVal);
       newTDur3Tag.appendChild(TDurTag);

       QDomElement OnTimeTag = document.createElement(QString("OnTime1"));
       QDomText OnTimeVal = document.createTextNode(QString::number(onTime1));
       OnTimeTag .appendChild(OnTimeVal);
       newTDur3Tag.appendChild(OnTimeTag);

       root.appendChild(newTDur3Tag);

   }


   else
   {
         QDomElement root = document.documentElement();
         QDomNode TDurNode = root.namedItem("TDur_Stroke4");

         QDomNode d2 = TDurNode.namedItem("Duration");
         d2.firstChild().setNodeValue(QString::number(Training_Duration1));

         QDomNode d3 = TDurNode.namedItem("OnTime1");
         d3.firstChild().setNodeValue(QString::number(onTime1));

   }

    if(RR3NodeVal.isNull())
    {
        QDomElement SP1_Rr1Tag = document.createElement(QString("RR_Step_Ch1"));
        QDomText SP1_Rr1Val = document.createTextNode(QString::number(ramp_step_size_rr1));
        SP1_Rr1Tag .appendChild(SP1_Rr1Val);
        newRR3Tag.appendChild(SP1_Rr1Tag);

        QDomElement SP1_Rf1Tag = document.createElement(QString("RF_Step_Ch1"));
        QDomText SP1_Rf1Val = document.createTextNode(QString::number(ramp_step_size_rf1));
        SP1_Rf1Tag .appendChild(SP1_Rf1Val);
        newRR3Tag.appendChild(SP1_Rf1Tag);

        QDomElement SP1_Rr2Tag = document.createElement(QString("RR_Step_Ch2"));
        QDomText SP1_Rr2Val = document.createTextNode(QString::number(ramp_step_size_rr2));
        SP1_Rr2Tag .appendChild(SP1_Rr2Val);
        newRR3Tag.appendChild(SP1_Rr2Tag);

        QDomElement SP1_Rf2Tag = document.createElement(QString("RF_Step_Ch2"));
        QDomText SP1_Rf2Val = document.createTextNode(QString::number(ramp_step_size_rf2));
        SP1_Rf2Tag .appendChild(SP1_Rf2Val);
        newRR3Tag.appendChild(SP1_Rf2Tag);

        QDomElement SP1_Rr3Tag = document.createElement(QString("RR_Step_Ch3"));
        QDomText SP1_Rr3Val = document.createTextNode(QString::number(ramp_step_size_rr3));
        SP1_Rr3Tag .appendChild(SP1_Rr3Val);
        newRR3Tag.appendChild(SP1_Rr3Tag);

        QDomElement SP1_Rf3Tag = document.createElement(QString("RF_Step_Ch3"));
        QDomText SP1_Rf3Val = document.createTextNode(QString::number(ramp_step_size_rf3));
        SP1_Rf3Tag .appendChild(SP1_Rf3Val);
        newRR3Tag.appendChild(SP1_Rf3Tag);

        QDomElement SP1_Rr4Tag = document.createElement(QString("RR_Step_Ch4"));
        QDomText SP1_Rr4Val = document.createTextNode(QString::number(ramp_step_size_rr4));
        SP1_Rr4Tag .appendChild(SP1_Rr4Val);
        newRR3Tag.appendChild(SP1_Rr4Tag);

        QDomElement SP1_Rf4Tag = document.createElement(QString("RF_Step_Ch4"));
        QDomText SP1_Rf4Val = document.createTextNode(QString::number(ramp_step_size_rf4));
        SP1_Rf4Tag .appendChild(SP1_Rf4Val);
        newRR3Tag.appendChild(SP1_Rf4Tag);

        QDomElement SP1_Rr5Tag = document.createElement(QString("RR_Step_Ch5"));
        QDomText SP1_Rr5Val = document.createTextNode(QString::number(ramp_step_size_rr5));
        SP1_Rr5Tag .appendChild(SP1_Rr5Val);
        newRR3Tag.appendChild(SP1_Rr5Tag);

        QDomElement SP1_Rf5Tag = document.createElement(QString("RF_Step_Ch5"));
        QDomText SP1_Rf5Val = document.createTextNode(QString::number(ramp_step_size_rf5));
        SP1_Rf5Tag .appendChild(SP1_Rf5Val);
        newRR3Tag.appendChild(SP1_Rf5Tag);



        root.appendChild(newRR3Tag);

    }
    else
    {
        QDomElement root = document.documentElement();
        QDomNode RampSettingsNode = root.namedItem("Ramp_Step_Stroke4");

        QDomNode rmp1 = RampSettingsNode.namedItem("RR_Step_Ch1");
        rmp1.firstChild().setNodeValue(QString::number(ramp_step_size_rr1));
        QDomNode rmp2 = RampSettingsNode.namedItem("RF_Step_Ch1");
        rmp2.firstChild().setNodeValue(QString::number(ramp_step_size_rf1));
        QDomNode rmp3 = RampSettingsNode.namedItem("RR_Step_Ch2");
        rmp3.firstChild().setNodeValue(QString::number(ramp_step_size_rr2));
        QDomNode rmp4 = RampSettingsNode.namedItem("RF_Step_Ch2");
        rmp4.firstChild().setNodeValue(QString::number(ramp_step_size_rf2));
        QDomNode rmp5 = RampSettingsNode.namedItem("RR_Step_Ch3");
        rmp5.firstChild().setNodeValue(QString::number(ramp_step_size_rr3));
        QDomNode rmp6 = RampSettingsNode.namedItem("RF_Step_Ch3");
        rmp6.firstChild().setNodeValue(QString::number(ramp_step_size_rf3));
        QDomNode rmp7 = RampSettingsNode.namedItem("RR_Step_Ch4");
        rmp7.firstChild().setNodeValue(QString::number(ramp_step_size_rr4));
        QDomNode rmp8 = RampSettingsNode.namedItem("RF_Step_Ch4");
        rmp8.firstChild().setNodeValue(QString::number(ramp_step_size_rf4));
        QDomNode rmp9 = RampSettingsNode.namedItem("RR_Step_Ch5");
        rmp9.firstChild().setNodeValue(QString::number(ramp_step_size_rr5));
        QDomNode rmp10 = RampSettingsNode.namedItem("RF_Step_Ch5");
        rmp10.firstChild().setNodeValue(QString::number(ramp_step_size_rf5));

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

void StrokeTrainingWindowFour::loadCurrentWidgetValues()
{
    QVariant PW_Ch1Data =  ui->comboBox_p1pw1->currentData();
    QVariant PW_Ch2Data =  ui->comboBox_p1pw2->currentData();
    QVariant PW_Ch3Data =  ui->comboBox_p1pw3->currentData();
    QVariant PW_Ch4Data =  ui->comboBox_p1pw4->currentData();
    QVariant PW_Ch5Data =  ui->comboBox_p1pw5->currentData();
    QVariant RR_Ch1Data =  ui->comboBox_p1rr1->currentData();
    QVariant  RR_Ch2Data =  ui->comboBox_p1rr2->currentData();
    QVariant  RR_Ch3Data =  ui->comboBox_p1rr3->currentData();
    QVariant  RR_Ch4Data =  ui->comboBox_p1rr4->currentData();
    QVariant  RR_Ch5Data =  ui->comboBox_p1rr5->currentData();
    QVariant   RF_Ch1Data =  ui->comboBox_p1rf1->currentData();
    QVariant  RF_Ch2Data =  ui->comboBox_p1rf2->currentData();
    QVariant  RF_Ch3Data =  ui->comboBox_p1rf3->currentData();
    QVariant  RF_Ch4Data =  ui->comboBox_p1rf4->currentData();
    QVariant  RF_Ch5Data =  ui->comboBox_p1rf5->currentData();
    //QVariant  Delay_Ch2Data =  ui->comboBox_p1d2->currentData();
    QVariant  Duration1_Data =  ui->comboBox_p1dur->currentData();
    QVariant  OnTime1_Data =  ui->comboBox_onTime1->currentData();

    PW_Ch1 = PW_Ch1Data.toInt();
    PW_Ch2 = PW_Ch2Data.toInt();
    PW_Ch3 = PW_Ch3Data.toInt();
    PW_Ch4 = PW_Ch4Data.toInt();
    PW_Ch5 = PW_Ch5Data.toInt();
    RR_Ch1 = RR_Ch1Data.toFloat();
    RR_Ch2 = RR_Ch2Data.toFloat();
    RR_Ch3 = RR_Ch3Data.toFloat();
    RR_Ch4 = RR_Ch4Data.toFloat();
    RR_Ch5 = RR_Ch5Data.toFloat();
    RF_Ch1 = RF_Ch1Data.toFloat();
    RF_Ch2 = RF_Ch2Data.toFloat();
    RF_Ch3 = RF_Ch3Data.toFloat();
    RF_Ch4 = RF_Ch4Data.toFloat();
    RF_Ch5 = RF_Ch5Data.toFloat();
    onTime1 = OnTime1_Data.toFloat()*1000;


    //Delay_Ch2 = Delay_Ch2Data.toFloat()*1000;
    Training_Duration1 = Duration1_Data.toFloat();

    ramp_step_size_rr1 = adjust_Ramp_Step_size(PW_Ch1,RR_Ch1);
    ramp_step_size_rr2 = adjust_Ramp_Step_size(PW_Ch2,RR_Ch2);
    ramp_step_size_rr3 = adjust_Ramp_Step_size(PW_Ch3,RR_Ch3);
    ramp_step_size_rr4 = adjust_Ramp_Step_size(PW_Ch4,RR_Ch4);
    ramp_step_size_rr5 = adjust_Ramp_Step_size(PW_Ch5,RR_Ch5);
    ramp_step_size_rf1 = adjust_Ramp_Step_size(PW_Ch1,RF_Ch1);
    ramp_step_size_rf2 = adjust_Ramp_Step_size(PW_Ch2,RF_Ch2);
    ramp_step_size_rf3 = adjust_Ramp_Step_size(PW_Ch3,RF_Ch3);
    ramp_step_size_rf4 = adjust_Ramp_Step_size(PW_Ch4,RF_Ch4);
    ramp_step_size_rf5 = adjust_Ramp_Step_size(PW_Ch5,RF_Ch5);
}


void StrokeTrainingWindowFour::on_pushButton_stimStart1_clicked()
{
    tetra_grip_api::stimulation_start(true);

    ui->btn_nextPhase1 ->setEnabled(true);

    setUiElementsEnabled(false);
    ui->pushButton_download1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(false);
}


void StrokeTrainingWindowFour::on_pushButton_stimStop1_clicked()
{
    tetra_grip_api::stimulation_pause(true);
    ui->btn_nextPhase1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);

    m_currentBtn1 = 0;



//    ManageConfigFile configFile;
//    configFile.sProg4Final(pLabel);
//    loadConfigFile(config_file_pr4);


    setUiElementsEnabled(true);
   // ui->pushButton_triggerApply->setEnabled(false);


    ui->btn11->setStyleSheet(StyleSheetOff);

    ui->btn12->setStyleSheet(StyleSheetOff);

    ui->btn13->setStyleSheet(StyleSheetOff);


    update();
}


void StrokeTrainingWindowFour::on_pushButton_configSet_clicked()
{
    ui->pushButton_stimStart1->setEnabled(true);


//    if (configStatusId == 0)
//    {
//        loadConfigFile(test_config_file_pr4);
//    }
     if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
         ManageConfigFile configFile;
         configFile.sProg4Final(pLabel);
         loadConfigFile(config_file_pr4);
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
         ManageConfigFile configFile;
         configFile.sProg4Final2(pLabel);
         config_file_pr42 = "config_sProg4_final2_"+pLabel;
         loadConfigFile(config_file_pr42);
    }


    else if(triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)
    {
        if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==0)
        {
            ManangeConfigExtended configFile;
            configFile.sProg4Final3_1_1(pLabel);
            config_file_pr43 = "config_sProg4_final311_"+pLabel;
            loadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==1)
        {
            ManangeConfigExtended configFile;
            configFile.sProg4Final3_1_2(pLabel);
            config_file_pr43 = "config_sProg4_final312_"+pLabel;
            loadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==0)
        {
            ManangeConfigExtended configFile;
            configFile.sProg4Final3_2_1(pLabel);
            config_file_pr43 = "config_sProg4_final321_"+pLabel;
            loadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==1)
        {

            ManangeConfigExtended configFile;
            configFile.sProg4Final3_2_2(pLabel);
            config_file_pr43 = "config_sProg4_final322_"+pLabel;
            loadConfigFile(config_file_pr43);
        }

        else
        {
            ManageConfigFile configFile;
            configFile.sProg4Final3(pLabel);
            config_file_pr43 = "config_sProg4_final3_"+pLabel;
            loadConfigFile(config_file_pr43);
        }



    }


    else
    {
        loadConfigFile(config_file_pr4);
    }
}


void StrokeTrainingWindowFour::on_btn_nextPhase1_clicked()
{
    tetra_grip_api::send_event(0, 253);

    m_currentBtn1++;
    //ui->label->setText(QString::number(m_currentBtn1)+"btngrp size: "+QString::number(btnGrp1->buttons().size()));
    if(m_currentBtn1 >= btnGrp1->buttons().size())
    {
        m_currentBtn1 = 0;
        phaseOver = true;
        saveToXMLFile();
        ManageConfigFile configFile;
        configFile.sProg4Final(pLabel);
        configStatusId =1;


    }
}


void StrokeTrainingWindowFour::on_comboBox_p1pw1_currentIndexChanged(int index)
{
    PW_Ch1 = index;
    emit P1W1_Value_Prog4(index);
    saveToXMLFile();
//    ManageConfigFile configFile;
//    configFile.sProg1Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);
}


void StrokeTrainingWindowFour::on_comboBox_p1pw2_currentIndexChanged(int index)
{
    PW_Ch2 = index;
    emit P1W2_Value_Prog4(index);
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);
}


void StrokeTrainingWindowFour::on_comboBox_p1pw3_currentIndexChanged(int index)
{
    PW_Ch3 = index;
    emit P1W3_Value_Prog4(index);
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);
}


void StrokeTrainingWindowFour::on_comboBox_p1pw4_currentIndexChanged(int index)
{
    PW_Ch4 = index;
    emit P1W4_Value_Prog4(index);
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);

}


void StrokeTrainingWindowFour::on_comboBox_p1pw5_currentIndexChanged(int index)
{
    PW_Ch5 = index;
    emit P1W5_Value_Prog4(index);
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    ui->pushButton_configSet->setEnabled(true);

}


void StrokeTrainingWindowFour::on_comboBox_p1rr1_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rf1_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rr2_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rf2_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rr3_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rf4_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rr4_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rf5_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_comboBox_p1rr5_currentIndexChanged(int index)
{
    saveToXMLFile();
    ManageConfigFile configFile;
    configFile.sProg4Final(pLabel);
    if (configStatusId==1){
    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
    }
}


void StrokeTrainingWindowFour::on_pushButton_download1_2_clicked()
{
    if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
        downloadConfigFile(config_file_pr4);
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
        config_file_pr42 = "config_sProg4_final2_"+pLabel;
        downloadConfigFile(config_file_pr42);
    }
    else if(triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)
    {
        if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==0)
        {
            config_file_pr43 = "config_sProg4_final311_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==1)
        {
            config_file_pr43 = "config_sProg4_final312_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==0)
        {
            config_file_pr43 = "config_sProg4_final321_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==1)
        {
            config_file_pr43 = "config_sProg4_final322_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }

        else
        {
            config_file_pr43 = "config_sProg4_final3_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
    }
    else
    {
        downloadConfigFile(config_file_pr4);
    }
}

void StrokeTrainingWindowFour::IMUExtSettings(const QString &buttonText)
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

void StrokeTrainingWindowFour::IMUExtSettingsCheckBox(bool status)
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


/*void StrokeTrainingWindowFour::on_pushButton_triggerApply_clicked()
{

    if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
        ManageConfigFile configFile;
        configFile.sProg4Final(pLabel);
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
        ManageConfigFile configFile;
        configFile.sProg4Final2(pLabel);
    }
    else if (triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)


        {

//        ui->label->setText(QString::number(triggerIMUWrist) + QString::number(triggerIMUShoulder));
//        ui->label_8->setText(QString::number(triggerIMUtimeout));
            if (triggerIMUWrist == 1 && triggerIMUShoulder == 0 && triggerIMUtimeout == 0)
            {
                ManangeConfigExtended configFile;
                configFile.sProg4Final3_1_1(pLabel);

            }
            else if (triggerIMUWrist == 1 && triggerIMUShoulder == 0 && triggerIMUtimeout == 1)
            {
                ManangeConfigExtended configFile;
                configFile.sProg4Final3_1_2(pLabel);
            }
            else if (triggerIMUWrist == 0 && triggerIMUShoulder == 1 && triggerIMUtimeout == 0)
            {
                ManangeConfigExtended configFile;
                configFile.sProg4Final3_2_1(pLabel);
            }
            else if (triggerIMUWrist == 0 && triggerIMUShoulder == 1 && triggerIMUtimeout == 1)
            {
                ManangeConfigExtended configFile;
                configFile.sProg4Final3_2_2(pLabel);
            }
            else
            {
                ManageConfigFile configFile;
                configFile.sProg4Final3(pLabel);
            }
        }

    //ui->label_radio->setText("B:"+QString::number(triggerButton)+"T:"+QString::number(triggerTime));

    ui->pushButton_stimStart1->setEnabled(false);
    ui->pushButton_configSet->setEnabled(true);
} */


void StrokeTrainingWindowFour::on_radioButton_p1t1_clicked()
{
    saveToXMLFile();
    triggerIMU = false;
    triggerTime= true;
    triggerButton = false;
    ui->comboBox_onTime1->setEnabled(true);
    //ui->pushButton_triggerApply->setEnabled(true);
    ui->toolButton_imu1->setEnabled(false);
}


void StrokeTrainingWindowFour::on_radioButton_p1t2_clicked()
{
    saveToXMLFile();
   triggerIMU = false;
   triggerTime= false;
   triggerButton = true;
   ui->comboBox_onTime1->setEnabled(false);
  // ui->pushButton_triggerApply->setEnabled(true);
   ui->toolButton_imu1->setEnabled(false);
}


void StrokeTrainingWindowFour::on_radioButton_p1t3_clicked()
{
    saveToXMLFile();
   triggerIMU = true;
   triggerTime= false;
   triggerButton = false;
   ui->comboBox_onTime1->setEnabled(false);
   //ui->pushButton_triggerApply->setEnabled(true);
   ui->toolButton_imu1->setEnabled(true);
}


void StrokeTrainingWindowFour::on_toolButton_imu1_clicked()
{
    IMUDialog *newdialog;
    //this->hide();
    newdialog = new IMUDialog( pLabel, nullptr);

    connect(newdialog, &IMUDialog::radioButtonChanged, this, &StrokeTrainingWindowFour::IMUExtSettings);
    connect(newdialog, &IMUDialog::checkBoxChanged, this, &StrokeTrainingWindowFour::IMUExtSettingsCheckBox);

    newdialog->setAttribute(Qt::WA_DeleteOnClose);
    newdialog -> show();
}


void StrokeTrainingWindowFour::on_pushButton_download1_clicked()
{
    if(triggerButton == 1 && triggerTime == 0 && triggerIMU == 0)
    {
        downloadConfigFile(config_file_pr4);
    }
    else if(triggerTime == 1 && triggerIMU == 0 && triggerButton == 0)
    {
        config_file_pr42 = "config_sProg4_final2_"+pLabel;
        downloadConfigFile(config_file_pr42);
    }
    else if(triggerIMU == 1 && triggerTime == 0 && triggerButton == 0)
    {
        if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==0)
        {
            config_file_pr43 = "config_sProg4_final311_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==1&&triggerIMUShoulder==0&&triggerIMUtimeout==1)
        {
            config_file_pr43 = "config_sProg4_final312_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==0)
        {
            config_file_pr43 = "config_sProg4_final321_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
        else if (triggerIMUWrist==0&&triggerIMUShoulder==1&&triggerIMUtimeout==1)
        {
            config_file_pr43 = "config_sProg4_final322_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }

        else
        {
            config_file_pr43 = "config_sProg4_final3_"+pLabel;
            downloadConfigFile(config_file_pr43);
        }
    }
    else
    {
        downloadConfigFile(config_file_pr4);
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


void StrokeTrainingWindowFour::on_checkBox_stateChanged(int arg1)
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

