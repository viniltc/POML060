#include "stageprogram.h"
#include "ui_stageprogram.h"
#include "currentbuttonone.h"
#include "stageonemain.h"
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QCloseEvent>
#include "manageconfigfile.h"


stageProgram::stageProgram(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stageProgram),
    watch(new Stopwatch())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(), this->height());
    ui->pushButton_currOnOne->setCheckable(true);
    ui->pushButton_currOnTwo->setCheckable(true);
    ui->pushButton_currOnThree->setCheckable(true);
    ui->pushButton_currOnFour->setCheckable(true);
    ui->pushButton_currOnFive->setCheckable(true);
    ui->pushButton_stimSave->setEnabled(false);

    ui->pushButton_programStroke2->setToolTip(
        "<img src=':/resources/envStrokeProg12.jpg' style='max-width: 100px; max-height: 100px;'>"
    );
    ui->pushButton_programStroke1->setToolTip(
        "<img src=':/resources/envStrokeProg21.jpg' style='max-width: 100px; max-height: 100px;'>"
    );
    ui->pushButton_programStroke_3->setToolTip(
        "<img src=':/resources/envStrokeProg31.jpg' style='max-width: 100px; max-height: 100px;'>"
    );
    ui->pushButton_programStroke4->setToolTip(
        "<img src=':/resources/envStrokeProg43.jpg' style='max-width: 100px; max-height: 100px;'>"
    );
    ui->pushButton_programStroke5->setToolTip(
        "<img src=':/resources/envStrokeProg53.jpg' style='max-width: 100px; max-height: 100px;'>"
    );
    ui->pushButton_programStroke6->setToolTip(
        "<img src=':/resources/envStrokeProg6.jpg' style='max-width: 100px; max-height: 100px;'>"
    );

originalSize = this->size();

    ui->tabWidget->setCurrentWidget(ui->tab);

    ui->label_curr_one->setText(QString("%1 mA").arg(0));
    ui->label_curr_two->setText(QString("%1 mA").arg(0));
    ui->label_curr_three->setText(QString("%1 mA").arg(0));

    ui->label_curr_four->setText(QString("%1 mA").arg(0));
    ui->label_curr_five->setText(QString("%1 mA").arg(0));


    QComboBox* freqComboBoxes[] { ui->comboBox_frequency_1, ui->comboBox_frequency_2, ui->comboBox_frequency_3, ui->comboBox_frequency_4, ui->comboBox_frequency_5 };
    const int freqValues[] { 20, 25, 30, 35, 40, 45, 50, 55, 60 };
    for(auto* cb : freqComboBoxes)
    {
        for(auto val : freqValues)
        {
            cb->addItem(QString::number(val) + "Hz", QVariant(val*10));
            cb->setCurrentIndex(2); // default 30 Hz
        }
    }



    QComboBox* waveformComboBoxes[] { ui->comboBox_waveform_1, ui->comboBox_waveform_2, ui->comboBox_waveform_3, ui->comboBox_waveform_4, ui->comboBox_waveform_5 };
    for(auto* cb : waveformComboBoxes)
    {
        cb->addItem("ASYM", STIM_ENGINE_WAVEFORM_ENUM::STIM_ENGINE_WAVEFORM_ASYM);
        cb->addItem("SYM", STIM_ENGINE_WAVEFORM_ENUM::STIM_ENGINE_WAVEFORM_SYM);
        cb->setCurrentIndex(0);
    }

    ui->comboBox_frequency_1->hide();
    ui->comboBox_frequency_2->hide();
    ui->comboBox_frequency_3->hide();
    ui->comboBox_frequency_4->hide();
    ui->comboBox_frequency_5->hide();
    ui->comboBox_waveform_1->hide();
    ui->comboBox_waveform_2->hide();
    ui->comboBox_waveform_3->hide();
    ui->comboBox_waveform_4->hide();
    ui->comboBox_waveform_5->hide();
    originalSize = ui->groupBox_ch1->size();
    ui->groupBox_ch1->setFixedHeight(30);
    ui->groupBox_ch2->setFixedHeight(30);
    ui->groupBox_ch3->setFixedHeight(30);
    ui->groupBox_ch4->setFixedHeight(30);
    ui->groupBox_ch5->setFixedHeight(30);


    // Function to create and populate a combo box model
    auto createComboBoxModel = [](QComboBox* comboBox) {
        QStandardItemModel *model = new QStandardItemModel(comboBox);
        auto createItemWithTooltip = [&](const QString &text, const QString &tooltip) {
            QStandardItem *item = new QStandardItem(text);
            item->setData(tooltip, Qt::ToolTipRole);
            model->appendRow(item);
        };

        // Create and append items
        createItemWithTooltip("PIN", "Posterior Interosseous Nerve - Supplies finger, thumb and wrist extensors.");
        createItemWithTooltip("UN", "Ulna nerve - Supplies lumbricals and AdP - straightens fingers, flexes the MCP joints and abducts.");
        createItemWithTooltip("ECRB/L", "Extensor Carpi Radialis Brevis / Longus - Extends and abducts the wrist.");
        createItemWithTooltip("EDC", "Extensor Digitorum Communis - Finger extensors.");
        createItemWithTooltip("EPL", "Extensor Pollicis Longus - Thumb extensor. Straightens distal joint.");
        createItemWithTooltip("AbPL", "Abductor Pollicis Longus - Long thumb abductor, lies next to EPL.");
        createItemWithTooltip("AbPB", "Abductor Pollicis Brevis - Thumb abductor part of thenar eminence.");
         createItemWithTooltip("AdP", "Adductor Pollicis - Brings thumb in usually stimulated via UN.");
         createItemWithTooltip("FPL", "Flexor Pollicis Longus - Flexes thumb bringing end over.");
         createItemWithTooltip("FDS", "Flexor Digitorum Superficialis - Finger flexor – bends two most proximal joints.");
         createItemWithTooltip("Tr", "Triceps - Elbow extensor.");
         createItemWithTooltip("Bi", "Biceps - Elbow flexor.");
         createItemWithTooltip("AD", "Anterior Deltoid - Shoulder Flexor.");
         createItemWithTooltip("MD", "Middle Deltoid - Shoulder abductor.");
         createItemWithTooltip("PD", "Posterior Deltoid - Shoulder extensor.");
         createItemWithTooltip("Sp", "Supraspinatus - Elevates the humorous bone into the rotator cuff.");
         createItemWithTooltip("Traps", "Trapezius - Stabilises the shoulder girdle.");
         createItemWithTooltip("Rh", "Rhomboids	- Stabilises scapula.");



        // Set the model to the combo box
        comboBox->setModel(model);
    };

    // Create and set models for combo boxes
    createComboBoxModel(ui->comboBox_m1);
    createComboBoxModel(ui->comboBox_m2);
    createComboBoxModel(ui->comboBox_m3);
    createComboBoxModel(ui->comboBox_m4);
    createComboBoxModel(ui->comboBox_m5);

   // ui->label->setText(ui->comboBox_m1->itemData(ui->comboBox_m1->currentIndex(), Qt::ToolTipRole).toString());

    ui->widget_currentOne->setEnabled(false);
    ui->widget_currentTwo->setEnabled(false);
    ui->widget_currentThree->setEnabled(false);
    ui->widget_currentFour->setEnabled(false);
    ui->widget_currentFive->setEnabled(false);

    ui->radioButton->setChecked(true);

    ui->label_pLabel->setText(patientLabel);
    ui->label_pLabel->setAlignment(Qt::AlignCenter);
    ui->label_pLabel->setStyleSheet("color: blue;");


    StyleSheetOn = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                   "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, "
                   "stop : 0.0 #32cd32,stop : 0.5 #1e7b1e, stop : 0.55 #28a428, stop : 1.0 #46d246)";
    StyleSheetOff = "border: 1px solid #6593cf; border-radius: 2px; padding: 5px 15px 2px 5px;"
                    "background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #f5f9ff,"
                    "stop :   0.5 #c7dfff, stop :   0.55 #afd2ff, stop :   1.0 #c0dbff);"
                    "color: #0000;";


    pLabel = patientLabel;


    connect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    //connect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

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
    QDomNode Key_PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
    QDomElement Key_PWNodeVal = Key_PWNode.toElement();
    QDomNode Palmer_PWNode = root.elementsByTagName("PW_PalmerGrasp").at(0).firstChild();
    QDomElement Palmer_PWNodeVal = Palmer_PWNode.toElement();
    QDomNode Sensor_SettNode = root.elementsByTagName("Sensor_Settings").at(0).firstChild();
    QDomElement Sensor_SettNodeVal = Sensor_SettNode.toElement();
    QDomNode Frequency_SettNode = root.elementsByTagName("FrequencyIndex").at(0).firstChild();
    QDomElement Frequency_SettNodeVal = Frequency_SettNode.toElement();
    QDomNode FrequencyVal_SettNode = root.elementsByTagName("Frequency").at(0).firstChild();
    QDomElement FrequencyVal_SettNodeVal = FrequencyVal_SettNode.toElement();
    QDomNode Waveform_SettNode = root.elementsByTagName("WaveformIndex").at(0).firstChild();
    QDomElement Waveform_SettNodeVal = Waveform_SettNode.toElement();
    QDomNode WaveformVal_SettNode = root.elementsByTagName("Waveform").at(0).firstChild();
    QDomElement WaveformVal_SettNodeVal = WaveformVal_SettNode.toElement();
    QDomNode MuscleVal_SettNode = root.elementsByTagName("MuscleName").at(0).firstChild();
    QDomElement MuscleVal_SettNodeVal = MuscleVal_SettNode.toElement();

    if (!CurrentNodeVal.isNull())
    {

        currOneStored = root.elementsByTagName("CH1").at(0).firstChild().nodeValue().toFloat()*1000;
        currTwoStored = root.elementsByTagName("CH2").at(0).firstChild().nodeValue().toFloat()*1000;
        currThreeStored = root.elementsByTagName("CH3").at(0).firstChild().nodeValue().toFloat()*1000;
        currFourStored = root.elementsByTagName("CH4").at(0).firstChild().nodeValue().toFloat()*1000;
        currFiveStored = root.elementsByTagName("CH5").at(0).firstChild().nodeValue().toFloat()*1000;

        ui->widget_currentOne->value = currOneStored;
        ui->widget_currentTwo->value = currTwoStored;
        ui->widget_currentThree->value = currThreeStored;
        ui->widget_currentFour->value = currFourStored;
        ui->widget_currentFive->value = currFiveStored;

        ui->label_curr_one->setText(QString("%1 mA").arg(ui->widget_currentOne->value/m_currentDiv));
        ui->label_curr_two->setText(QString("%1 mA").arg(ui->widget_currentTwo->value/m_currentDiv));
        ui->label_curr_three->setText(QString("%1 mA").arg(ui->widget_currentThree->value/m_currentDiv));
        ui->label_curr_four->setText(QString("%1 mA").arg(ui->widget_currentFour->value/m_currentDiv));
        ui->label_curr_five->setText(QString("%1 mA").arg(ui->widget_currentFive->value/m_currentDiv));

        currentOneSetVal = ui->widget_currentOne->value/m_currentDiv;
        currentTwoSetVal = ui->widget_currentTwo->value/m_currentDiv;
        currentThreeSetVal = ui->widget_currentThree->value/m_currentDiv;
        currentFourSetVal = ui->widget_currentFour->value/m_currentDiv;
        currentFiveSetVal = ui->widget_currentFive->value/m_currentDiv;


    }
    else
    {
        ui->widget_currentOne->value = smallCurrent;
        ui->widget_currentTwo->value = smallCurrent;
        ui->widget_currentThree->value = smallCurrent;
        ui->widget_currentFour->value = smallCurrent;
        ui->widget_currentFive->value = smallCurrent;

        ui->label_curr_one->setText(QString("%1 mA").arg(ui->widget_currentOne->value/m_currentDiv));
        ui->label_curr_two->setText(QString("%1 mA").arg(ui->widget_currentTwo->value/m_currentDiv));
        ui->label_curr_three->setText(QString("%1 mA").arg(ui->widget_currentThree->value/m_currentDiv));
        ui->label_curr_four->setText(QString("%1 mA").arg(ui->widget_currentFour->value/m_currentDiv));
        ui->label_curr_five->setText(QString("%1 mA").arg(ui->widget_currentFive->value/m_currentDiv));

        currentOneSetVal = smallCurrent/m_currentDiv;
        currentTwoSetVal = smallCurrent/m_currentDiv;
        currentThreeSetVal = smallCurrent/m_currentDiv;
        currentFourSetVal = smallCurrent/m_currentDiv;
        currentFiveSetVal = smallCurrent/m_currentDiv;
    }

    if(!Key_PWNodeVal.isNull())
    {
        ui->pushButton->setStyleSheet(StyleSheetOn);
    }
    else
    {
        ui->pushButton->setStyleSheet(StyleSheetOff);
    }

    if(!Palmer_PWNodeVal.isNull())
    {
        ui->pushButton_2->setStyleSheet(StyleSheetOn);
    }
    else
    {
        ui->pushButton_2->setStyleSheet(StyleSheetOff);
    }
    if(!Sensor_SettNodeVal.isNull())
    {
        ui->pushButton_3->setStyleSheet(StyleSheetOn);
    }
    else
    {
        ui->pushButton_3->setStyleSheet(StyleSheetOff);
    }

    if(!Frequency_SettNodeVal.isNull())
    {
        oneFreqIndex = root.elementsByTagName("F1").at(0).firstChild().nodeValue().toInt();
        twoFreqIndex = root.elementsByTagName("F2").at(0).firstChild().nodeValue().toInt();
        threeFreqIndex = root.elementsByTagName("F3").at(0).firstChild().nodeValue().toInt();
        fourFreqIndex = root.elementsByTagName("F4").at(0).firstChild().nodeValue().toInt();
        fiveFreqIndex = root.elementsByTagName("F5").at(0).firstChild().nodeValue().toInt();

        ui->comboBox_frequency_1->setCurrentIndex(oneFreqIndex);
        ui->comboBox_frequency_2->setCurrentIndex(twoFreqIndex);
        ui->comboBox_frequency_3->setCurrentIndex(threeFreqIndex);
        ui->comboBox_frequency_4->setCurrentIndex(fourFreqIndex);
        ui->comboBox_frequency_5->setCurrentIndex(fiveFreqIndex);

        oneFreqStim = ui->comboBox_frequency_1->itemData(oneFreqIndex).toUInt();
        twoFreqStim = ui->comboBox_frequency_2->itemData(twoFreqIndex).toUInt();
        threeFreqStim = ui->comboBox_frequency_3->itemData(threeFreqIndex).toUInt();
        fourFreqStim = ui->comboBox_frequency_4->itemData(fourFreqIndex).toUInt();
        fiveFreqStim = ui->comboBox_frequency_5->itemData(fiveFreqIndex).toUInt();

        //tetra_grip_api::stimulation_set_frequency(m_channelOne, oneFreqStim);


    }


    else

    {
        oneFreqIndex = ui->comboBox_frequency_1->currentIndex();
        twoFreqIndex = ui->comboBox_frequency_2->currentIndex();
        threeFreqIndex = ui->comboBox_frequency_3->currentIndex();
        fourFreqIndex = ui->comboBox_frequency_4->currentIndex();
        fiveFreqIndex = ui->comboBox_frequency_5->currentIndex();

        ui->comboBox_frequency_1->setCurrentIndex(2);
        ui->comboBox_frequency_2->setCurrentIndex(2);
        ui->comboBox_frequency_3->setCurrentIndex(2);
        ui->comboBox_frequency_4->setCurrentIndex(2);
        ui->comboBox_frequency_5->setCurrentIndex(2);


    }

    if(!Waveform_SettNodeVal.isNull())
    {
        oneWaveIndex = root.elementsByTagName("W1").at(0).firstChild().nodeValue().toInt();
        twoWaveIndex = root.elementsByTagName("W2").at(0).firstChild().nodeValue().toInt();
        threeWaveIndex = root.elementsByTagName("W3").at(0).firstChild().nodeValue().toInt();
        fourWaveIndex = root.elementsByTagName("W4").at(0).firstChild().nodeValue().toInt();
        fiveWaveIndex = root.elementsByTagName("W5").at(0).firstChild().nodeValue().toInt();

        ui->comboBox_waveform_1->setCurrentIndex(oneWaveIndex);
        ui->comboBox_waveform_2->setCurrentIndex(twoWaveIndex);
        ui->comboBox_waveform_3->setCurrentIndex(threeWaveIndex);
        ui->comboBox_waveform_4->setCurrentIndex(fourWaveIndex);
        ui->comboBox_waveform_5->setCurrentIndex(fiveWaveIndex);

        oneWaveStim = ui->comboBox_waveform_1->itemData(oneWaveIndex).toUInt();
        twoWaveStim = ui->comboBox_waveform_2->itemData(twoWaveIndex).toUInt();
        threeWaveStim = ui->comboBox_waveform_3->itemData(threeWaveIndex).toUInt();
        fourWaveStim = ui->comboBox_waveform_4->itemData(fourWaveIndex).toUInt();
        fiveWaveStim = ui->comboBox_waveform_5->itemData(fiveWaveIndex).toUInt();

        //tetra_grip_api::stimulation_set_frequency(m_channelOne, oneFreqStim);


    }


    else

    {
        oneWaveIndex = ui->comboBox_waveform_1->currentIndex();
        twoWaveIndex = ui->comboBox_waveform_2->currentIndex();
        threeWaveIndex = ui->comboBox_waveform_3->currentIndex();
        fourWaveIndex = ui->comboBox_waveform_4->currentIndex();
        fiveWaveIndex = ui->comboBox_waveform_5->currentIndex();

        ui->comboBox_waveform_1->setCurrentIndex(0);
        ui->comboBox_waveform_2->setCurrentIndex(0);
        ui->comboBox_waveform_3->setCurrentIndex(0);
        ui->comboBox_waveform_4->setCurrentIndex(0);
        ui->comboBox_waveform_5->setCurrentIndex(0);


    }

    if(!MuscleVal_SettNodeVal.isNull())
    {
        oneMusIndex = root.elementsByTagName("MN1").at(0).firstChild().nodeValue().toInt();
        twoMusIndex = root.elementsByTagName("MN2").at(0).firstChild().nodeValue().toInt();
        threeMusIndex = root.elementsByTagName("MN3").at(0).firstChild().nodeValue().toInt();
        fourMusIndex = root.elementsByTagName("MN4").at(0).firstChild().nodeValue().toInt();
        fiveMusIndex = root.elementsByTagName("MN5").at(0).firstChild().nodeValue().toInt();

        ui->comboBox_m1->setCurrentIndex(oneMusIndex);
        ui->comboBox_m2->setCurrentIndex(twoMusIndex);
        ui->comboBox_m3->setCurrentIndex(threeMusIndex);
        ui->comboBox_m4->setCurrentIndex(fourMusIndex);
        ui->comboBox_m5->setCurrentIndex(fiveMusIndex);

    }


    else

    {
        oneMusIndex = ui->comboBox_m1->currentIndex();
        twoMusIndex = ui->comboBox_m2->currentIndex();
        threeMusIndex = ui->comboBox_m3->currentIndex();
        fourMusIndex = ui->comboBox_m4->currentIndex();
        fiveMusIndex = ui->comboBox_m5->currentIndex();

        ui->comboBox_m1->setCurrentIndex(0);
        ui->comboBox_m2->setCurrentIndex(0);
        ui->comboBox_m3->setCurrentIndex(0);
        ui->comboBox_m4->setCurrentIndex(0);
        ui->comboBox_m5->setCurrentIndex(0);


    }


//    connect(ui->pushButton_currOnOne, &QPushButton::clicked, ui->widget_currentOne, &CurrentButtonOne::setEnabled);
//    connect(ui->pushButton_currOnOne, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelOne);
    // For Channel 1
    connect(ui->pushButton_currOnOne, &QPushButton::clicked, [this]() {
        if (ui->pushButton_currOnOne->isChecked()) {
            ui->widget_currentOne->setEnabled(true);
            emit ui->widget_currentOne->getValue(ui->widget_currentOne->value);
        } else {
            setZeroCurrOnChannelOne();
            ui->widget_currentOne->setEnabled(false);
        }
    });

    // For Channel 2
    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, [this]() {
        if (ui->pushButton_currOnTwo->isChecked()) {
            ui->widget_currentTwo->setEnabled(true);
            emit ui->widget_currentTwo->getValue(ui->widget_currentTwo->value);
        } else {
            setZeroCurrOnChannelTwo();
            ui->widget_currentTwo->setEnabled(false);
        }
    });

    // For Channel 3
    connect(ui->pushButton_currOnThree, &QPushButton::clicked, [this]() {
        if (ui->pushButton_currOnThree->isChecked()) {
            ui->widget_currentThree->setEnabled(true);
            emit ui->widget_currentThree->getValue(ui->widget_currentThree->value);
        } else {
            setZeroCurrOnChannelThree();
            ui->widget_currentThree->setEnabled(false);
        }
    });

    // For Channel 4
    connect(ui->pushButton_currOnFour, &QPushButton::clicked, [this]() {
        if (ui->pushButton_currOnFour->isChecked()) {
            ui->widget_currentFour->setEnabled(true);
            emit ui->widget_currentFour->getValue(ui->widget_currentFour->value);
        } else {
            setZeroCurrOnChannelFour();
            ui->widget_currentFour->setEnabled(false);
        }
    });

    // For Channel 5
    connect(ui->pushButton_currOnFive, &QPushButton::clicked, [this]() {
        if (ui->pushButton_currOnFive->isChecked()) {
            ui->widget_currentFive->setEnabled(true);
            emit ui->widget_currentFive->getValue(ui->widget_currentFive->value);
        } else {
            setZeroCurrOnChannelFive();
            ui->widget_currentFive->setEnabled(false);
        }
    });


//    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, ui->widget_currentTwo, &CurrentButtonOne::setEnabled);
//    connect(ui->pushButton_currOnTwo, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelTwo);
//    connect(ui->pushButton_currOnThree, &QPushButton::clicked, ui->widget_currentThree, &CurrentButtonOne::setEnabled);
//    connect(ui->pushButton_currOnThree, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelThree);
//    connect(ui->pushButton_currOnFour, &QPushButton::clicked, ui->widget_currentFour, &CurrentButtonOne::setEnabled);
//    connect(ui->pushButton_currOnFour, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelFour);
//    connect(ui->pushButton_currOnFive, &QPushButton::clicked, ui->widget_currentFive, &CurrentButtonOne::setEnabled);
//    connect(ui->pushButton_currOnFive, &QPushButton::clicked, this, &stageProgram::setZeroCurrOnChannelFive);

    connect(ui->widget_currentOne, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelOne);
    connect(ui->widget_currentTwo, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelTwo);
    connect(ui->widget_currentThree, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelThree);
    connect(ui->widget_currentFour, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFour);
    connect(ui->widget_currentFive, &CurrentButtonOne::getValue, this, &stageProgram::setCurrOnChannelFive);


    connect(ui->groupBox_ch1, &QGroupBox::toggled, this, &stageProgram::onAdvancedSettingsToggledCh1);
    connect(ui->groupBox_ch2, &QGroupBox::toggled, this, &stageProgram::onAdvancedSettingsToggledCh2);
    connect(ui->groupBox_ch3, &QGroupBox::toggled, this, &stageProgram::onAdvancedSettingsToggledCh3);
    connect(ui->groupBox_ch4, &QGroupBox::toggled, this, &stageProgram::onAdvancedSettingsToggledCh4);
    connect(ui->groupBox_ch5, &QGroupBox::toggled, this, &stageProgram::onAdvancedSettingsToggledCh5);

    //connect(ui->widget_currentOne, &CurrentButtonOne::myButtonPressed, this, &stageProgram::setZeroCurrOnChannelOne);

//    connect(ui->widget_currentOne, &CurrentButtonOne::getValue,
//            [this](unsigned int current_uA) { if (ui->widget_currentOne->isEnabled())  setCurrOnChannelOne( current_uA) ; else  setZeroCurrOnChannelOne();} );

//    connect(ui->widget_currentOne, &CurrentButtonOne::getValue,
//            [this](unsigned int current_uA) {
//                qDebug() << "widget_currentOne->enabled(): " << ui->widget_currentOne->isEnabled();
//                qDebug() << "current_uA: " << current_uA;
//                setCurrOnChannelOne(ui->widget_currentOne->isEnabled() ? current_uA : 0);
//            } );


    //connect(ui->pushButton_currOnOne_2, &QPushButton::clicked, this,&stageProgram::setZeroCurrOnChannelOne);
    //slot
    connect(ui->pushButton_stimStart, &QPushButton::clicked, this, &stageProgram::startStopTimer);
    connect(ui->pushButton_stimStop, &QPushButton::clicked, this, &stageProgram::resetTimer);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(10);



    if(ui->tab)
    {
         //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
         tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
    }

    else if(ui->tab_3)
    {
       // disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
        tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
    }

//    QFile f(":/resources/config_setup.txt");
//    if(!f.open(QFile::ReadOnly))
//    {
//        QMessageBox::information(0, "config file error", f.errorString());
//    }
//    else
//    {
//        QByteArray config = f.readAll();
//        tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

//        // statusBar()->showMessage("Config file sent", 3000);

//    }






    if(!FrequencyVal_SettNode.isNull())
    {


        QString configtestfile = "config_current_test_"+pLabel;
        //QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+"config_setup.txt";
        QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configtestfile+".txt";

        //ui->label->setText("frequency node present");
        QFile f(txtWritePath);
        if(!f.open(QFile::ReadOnly))
        {
            QMessageBox::information(0, "Config file error, freqpresent", f.errorString());
        }
        else
        {
            QByteArray config = f.readAll();
            tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

        }
    }

    else
    {

        //  ui->label->setText("No frequency node!");
        QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+"config_setup.txt";
        QFile f(txtWritePath);
        if(!f.open(QFile::ReadOnly))
        {
            QMessageBox::information(0, "Config file error, no freqnode", f.errorString());
        }
        else
        {
            QByteArray config = f.readAll();
            tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());

        }


    }

    // reset all user intensity to 50%
     tetra_grip_api::do_stimulator_command( STIM_COMMAND_RESET_USER_INTENSITIES);

}

stageProgram::~stageProgram()
{
    delete ui;
}

void stageProgram::onAdvancedSettingsToggledCh1(bool checked)
{
    if (checked) {
        ui->comboBox_frequency_1->show();
        ui->comboBox_waveform_1->show();
        ui->groupBox_ch1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum); // Allow the group box to expand
                ui->groupBox_ch1->setFixedHeight(originalSize.height()); // Restore the original height

    } else {
        ui->comboBox_frequency_1->hide();
        ui->comboBox_waveform_1->hide();
        ui->groupBox_ch1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
               ui->groupBox_ch1->setFixedHeight(30); // Set a fixed height when collapsed
    }
}

void stageProgram::onAdvancedSettingsToggledCh2(bool checked)
{
    if (checked) {
        ui->comboBox_frequency_2->show();
        ui->comboBox_waveform_2->show();
        ui->groupBox_ch2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum); // Allow the group box to expand
                ui->groupBox_ch2->setFixedHeight(originalSize.height()); // Restore the original height

    } else {
        ui->comboBox_frequency_2->hide();
        ui->comboBox_waveform_2->hide();
        ui->groupBox_ch2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
               ui->groupBox_ch2->setFixedHeight(30); // Set a fixed height when collapsed
    }
}

void stageProgram::onAdvancedSettingsToggledCh3(bool checked)
{
    if (checked) {
        ui->comboBox_frequency_3->show();
        ui->comboBox_waveform_3->show();
        ui->groupBox_ch3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum); // Allow the group box to expand
                ui->groupBox_ch3->setFixedHeight(originalSize.height()); // Restore the original height

    } else {
        ui->comboBox_frequency_3->hide();
        ui->comboBox_waveform_3->hide();
        ui->groupBox_ch3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
               ui->groupBox_ch3->setFixedHeight(30); // Set a fixed height when collapsed
    }
}

void stageProgram::onAdvancedSettingsToggledCh4(bool checked)
{
    if (checked) {
        ui->comboBox_frequency_4->show();
        ui->comboBox_waveform_4->show();
        ui->groupBox_ch4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum); // Allow the group box to expand
                ui->groupBox_ch4->setFixedHeight(originalSize.height()); // Restore the original height

    } else {
        ui->comboBox_frequency_4->hide();
        ui->comboBox_waveform_4->hide();
        ui->groupBox_ch4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
               ui->groupBox_ch4->setFixedHeight(30); // Set a fixed height when collapsed
    }
}

void stageProgram::onAdvancedSettingsToggledCh5(bool checked)
{
    if (checked) {
        ui->comboBox_frequency_5->show();
        ui->comboBox_waveform_5->show();
        ui->groupBox_ch5->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum); // Allow the group box to expand
                ui->groupBox_ch5->setFixedHeight(originalSize.height()); // Restore the original height

    } else {
        ui->comboBox_frequency_5->hide();
        ui->comboBox_waveform_5->hide();
        ui->groupBox_ch5->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
               ui->groupBox_ch5->setFixedHeight(30); // Set a fixed height when collapsed
    }
}


void stageProgram::setCurrOnChannelOne(unsigned int current_uA)
{

    tetra_grip_api::stimulation_set_frequency(m_channelOne, oneFreqStim);
    tetra_grip_api::stimulation_set_current( m_channelOne, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelOne,0,180);
    tetra_grip_api::stimulation_set_waveform(m_channelOne, 0, oneWaveStim);



}

void stageProgram::setCurrOnChannelTwo(unsigned int current_uA)
{
    //currentTwoSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelTwo, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelTwo,0,180);
    tetra_grip_api::stimulation_set_frequency(m_channelTwo, twoFreqStim);
    tetra_grip_api::stimulation_set_waveform(m_channelTwo, 0, twoWaveStim);


}


void stageProgram::setCurrOnChannelThree(unsigned int current_uA)
{
    //currentThreeSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelThree, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelThree,0,180);
    tetra_grip_api::stimulation_set_frequency(m_channelThree, threeFreqStim);
    tetra_grip_api::stimulation_set_waveform(m_channelThree, 0, threeWaveStim);
}

void stageProgram::setCurrOnChannelFour(unsigned int current_uA)
{

   // currentFourSetVal = current_uA;
    tetra_grip_api::stimulation_set_current( m_channelFour, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFour,0,180);
    tetra_grip_api::stimulation_set_frequency(m_channelFour, fourFreqStim);
    tetra_grip_api::stimulation_set_waveform(m_channelFour, 0, fourWaveStim);
}

void stageProgram::setCurrOnChannelFive(unsigned int current_uA)
{

   // currentFiveSetVal = current_uA;
    tetra_grip_api::stimulation_set_frequency(m_channelFive, fiveFreqStim);
    tetra_grip_api::stimulation_set_current( m_channelFive, current_uA);
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFive,0,180);
    tetra_grip_api::stimulation_set_waveform(m_channelFive, 0, fiveWaveStim);

}

void stageProgram::setZeroCurrOnChannelOne()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelOne,0,0); // pw to 0 to turn off the stimulation
  //  ui->widget_currentOne->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelTwo()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelTwo,0,0); // pw to 0 to turn off the stimulation
   // ui->widget_currentTwo->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelThree()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelThree,0,0); // pw to 0 to turn off the stimulation
   // ui->widget_currentThree->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelFour()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFour,0,0); // pw to 0 to turn off the stimulation
   // ui->widget_currentFour->setDisabled(true);
}

void stageProgram::setZeroCurrOnChannelFive()
{
    tetra_grip_api::set_stimulation_target_pulse_width(m_channelFive,0,0); // pw to 0 to turn off the stimulation
    // ui->widget_currentFive->setDisabled(true);
}


void stageProgram::on_pushButton_stimStart_clicked()
{
    tetra_grip_api::stimulation_start(true);

    ui->comboBox_frequency_1->setEnabled(false);
    ui->comboBox_frequency_2->setEnabled(false);
    ui->comboBox_frequency_3->setEnabled(false);
    ui->comboBox_frequency_4->setEnabled(false);
    ui->comboBox_frequency_5->setEnabled(false);

    ui->comboBox_waveform_1->setEnabled(false);
    ui->comboBox_waveform_2->setEnabled(false);
    ui->comboBox_waveform_3->setEnabled(false);
    ui->comboBox_waveform_4->setEnabled(false);
    ui->comboBox_waveform_5->setEnabled(false);

    ui->radioButton->setEnabled(false);
    ui->radioButton_2->setEnabled(false);
    ui->radioButton_3->setEnabled(false);


}

void stageProgram::on_pushButton_stimStop_clicked()
{
    tetra_grip_api::stimulation_pause(true);

    ui->comboBox_frequency_1->setEnabled(true);
    ui->comboBox_frequency_2->setEnabled(true);
    ui->comboBox_frequency_3->setEnabled(true);
    ui->comboBox_frequency_4->setEnabled(true);
    ui->comboBox_frequency_5->setEnabled(true);

    ui->comboBox_waveform_1->setEnabled(true);
    ui->comboBox_waveform_2->setEnabled(true);
    ui->comboBox_waveform_3->setEnabled(true);
    ui->comboBox_waveform_4->setEnabled(true);
    ui->comboBox_waveform_5->setEnabled(true);

    ui->radioButton->setEnabled(true);
    ui->radioButton_2->setEnabled(true);
    ui->radioButton_3->setEnabled(true);

}

void stageProgram::closeEvent(QCloseEvent *event)
{

    tetra_grip_api::stimulation_pause(true);

    if(event->spontaneous()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "TetraGrip", "You are about to close Tetragrip Application, have you saved current and frequency settings?\n\n Click Yes to save the settings and close or Cancel to remian in this window",
                                      QMessageBox::Yes|QMessageBox::Cancel);
//        if (reply == QMessageBox::Close) {
//            event->accept();
//            //return;
//        }
        if (reply == QMessageBox::Yes) {

            saveToXMLFile();
            saveClicked = true;
            event->accept();
        }
        else if(reply == QMessageBox::Cancel) {
            event->ignore();
        }
    }

}

void stageProgram::resizeEvent(QResizeEvent *event)
{
    // Get the new size of the main window
    // Get the new size of the main window
     QSize newSize = event->size();

     // Scale factor based on the new size compared to the original size
     double scaleFactorWidth = static_cast<double>(newSize.width()) / originalSize.width();
     double scaleFactorHeight = static_cast<double>(newSize.height()) / originalSize.height();

     // Adjust the size and position of each widget based on the scale factor
     scaleWidgets(this, scaleFactorWidth, scaleFactorHeight);

     // Ensure to call the base class implementation
     QMainWindow::resizeEvent(event);
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
        case STIM_REG_NUM_SMART_SENSORS:
            if(value==0){
                QMessageBox::critical(this, "Sensor Not Connected!", "Shoulder sensor got disconnected \n\nReconnect the sensor and press Ok to continue");
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

            if(value/m_currentmA >= 120)
            {
                //ui->label_curr_one->setText(QString("MAX"));
                ui->label_curr_one->setStyleSheet("QLabel {  color : red; }");
            }
            else if (value/m_currentmA < 120)
            {
                ui->label_curr_one->setStyleSheet("QLabel {  color : black; }");
            }

            currentOneSetVal = value/m_currentmA;
            break;
        case 1: // channel 2

            ui->label_curr_two->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
                // ui->label_curr_two->setText(QString("MAX"));
                ui->label_curr_two->setStyleSheet("QLabel {  color : red; }");
            }
            else if (value/m_currentmA < 120)
            {
                ui->label_curr_two->setStyleSheet("QLabel {  color : black; }");
            }
            currentTwoSetVal = value/m_currentmA;
            break;
        case 2: //channel 3

            ui->label_curr_three->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
                //  ui->label_curr_three->setText(QString("MAX"));
                ui->label_curr_three->setStyleSheet("QLabel {  color : red; }");
            }
            else if (value/m_currentmA < 120)
            {
                ui->label_curr_three->setStyleSheet("QLabel {  color : black; }");
            }

            currentThreeSetVal = value/m_currentmA;
            break;
        case 3: // channel 4

            ui->label_curr_four->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
                // ui->label_curr_four->setText(QString("MAX"));
                ui->label_curr_four->setStyleSheet("QLabel {  color : red; }");
            }

            else if (value/m_currentmA < 120)
            {
                ui->label_curr_four->setStyleSheet("QLabel {  color : black; }");
            }

            currentFourSetVal = value/m_currentmA;
            break;
        case 4: // channle 5

            ui->label_curr_five->setText(QString("%1 mA").arg(value/m_currentmA));

            if(value/m_currentmA >= 120)
            {
                // ui->label_curr_five->setText(QString("MAX"));
                ui->label_curr_five->setStyleSheet("QLabel {  color : red; }");
            }

            else if (value/m_currentmA < 120)
            {
                ui->label_curr_five->setStyleSheet("QLabel {  color : black; }");
            }
            currentFiveSetVal = value/m_currentmA;
            break;
        }

        //        if (topic==TOPIC_CHANNEL && reg==STIM_ENGINE_REG_TARGET_FREQUENCY)
        //        {
        //            switch(index)
        //            {
        //            case 0:

        //               ui->label_2->setText(QString("%1 Hz").arg(value));
        //               break;
        //            }
        //        }


    }
}



void stageProgram::startStopTimer()
{
    if(watch->isRunning()) {
        //ui->startStopButton->setText("Restart");
        watch->pause();
    }
    else {
        //ui->startStopButton->setText("Pause");
        watch->start();
    }

}

void stageProgram::resetTimer()
{
    ui->hundredthsText->setText("00");
    ui->secondsText->setText("00");
    ui->minutesText->setText("00");
    watch->reset();
    QPalette p = ui->secondsText->palette();
    p.setColor(QPalette::Text, Qt::black);
    ui->secondsText->setPalette(p);
    ui->minutesText->setPalette(p);
    ui->hundredthsText->setPalette(p);

}



void stageProgram::onTimeout()
{
    QPalette p = ui->secondsText->palette();
    if(watch->isRunning())
    {
        qint64 time = watch->getTime();
        int h = time / 1000 / 60 / 60;
        int m = (time / 1000 / 60) - (h * 60);
        int s = (time / 1000) - (m * 60);
        int ms = time - ( s + ( m + ( h * 60)) * 60) * 1000;
        int ms_dis = ms / 10;
        if(ms_dis < 10) {
            ui->hundredthsText->setText(QStringLiteral("0%1").arg(ms_dis));
        }
        else {
            ui->hundredthsText->setText(QStringLiteral("%1").arg(ms_dis));
        }
        if(s < 10) {
            ui->secondsText->setText(QStringLiteral("0%1").arg(s));
            // p.setColor(QPalette::Base, Qt::white);
            //ui->secondsText->setPalette(p);
        }
        else {
            ui->secondsText->setText(QStringLiteral("%1").arg(s));

        }
        if(m < 10) {
            ui->minutesText->setText(QStringLiteral("0%1").arg(m));
        }
        else {
            ui->minutesText->setText(QStringLiteral("%1").arg(m));
        }

        if (s==10)
        {
            p.setColor(QPalette::Text, Qt::red);
            ui->secondsText->setPalette(p);
            ui->minutesText->setPalette(p);
            ui->hundredthsText->setPalette(p);


        }

    }

}



void stageProgram::on_pushButton_programKeyGrip_clicked()
{

    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

    }

    QDomDocument document;
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();
    QDomNode PWNode = root.elementsByTagName("PW_KeyGrip").at(0).firstChild();
    QDomElement PWNodeVal = PWNode.toElement();
    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();

    if(!PWNode.isNull())
    {
        ManageConfigFile configFile;
        configFile.keyGripFinal(pLabel);
    }

    else
    {
        ManageConfigFile configFile;
        configFile.keyGripTest(pLabel);
    }

    if(CurrentNode.isNull())
    {
        QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
        return;
    }



    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

    //disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this->close();
    keygripv2 = new ProgramKeyGripV2(pLabel, nullptr);
    keygripv2->setAttribute(Qt::WA_DeleteOnClose);
    keygripv2 -> show();
}

void stageProgram::on_pushButton_programPalmerGrasp_clicked()
{
    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

    }
    QDomDocument document;
    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();
    QDomNode PWNode = root.elementsByTagName("PW_PalmerGrasp").at(0).firstChild();
    QDomElement PWNodeVal = PWNode.toElement();
    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
    QDomElement CurrentNodeVal = CurrentNode.toElement();
    if(!PWNode.isNull())
    {
        ManageConfigFile configFile;
        configFile.palmerGraspFinal(pLabel);
    }
    else
    {
        ManageConfigFile configFile;
        configFile.palmerGraspTest(pLabel);
    }

    if(CurrentNode.isNull())
    {
        QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
        return;
    }

    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

    // disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    //  disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this->close();
    palmergrasp = new ProgramPalmerGrasp(pLabel,nullptr);
    palmergrasp->setAttribute(Qt::WA_DeleteOnClose);
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
    saveClicked = true;


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

    QDomElement newFrequencyTag = document.createElement(QString("FrequencyIndex"));
    QDomNode FrequencyNode = root.elementsByTagName("FrequencyIndex").at(0).firstChild();
    QDomElement FrequencyNodeVal = FrequencyNode.toElement();

    QDomElement newFrequencyValTag = document.createElement(QString("Frequency"));
    QDomNode FrequencyValNode = root.elementsByTagName("Frequency").at(0).firstChild();
    QDomElement FrequencyValNodeVal = FrequencyValNode.toElement();

    QDomElement newWaveformTag = document.createElement(QString("WaveformIndex"));
    QDomNode WaveformNode = root.elementsByTagName("WaveformIndex").at(0).firstChild();
    QDomElement WaveformNodeVal = WaveformNode.toElement();

    QDomElement newWaveformValTag = document.createElement(QString("Waveform"));
    QDomNode WaveformValNode = root.elementsByTagName("Waveform").at(0).firstChild();
    QDomElement WaveformValNodeVal = WaveformValNode.toElement();

    QDomElement newMuscleValTag = document.createElement(QString("MuscleName"));
    QDomNode MuscleValNode = root.elementsByTagName("MuscleName").at(0).firstChild();
    QDomElement MuscleValNodeVal = MuscleValNode.toElement();


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

    if (FrequencyNodeVal.isNull())
    {
        QDomElement f1Tag = document.createElement(QString("F1"));
        QDomText f1Val = document.createTextNode(QString::number(ui->comboBox_frequency_1->currentIndex()));
        f1Tag.appendChild(f1Val);
        newFrequencyTag.appendChild(f1Tag);

        QDomElement f2Tag = document.createElement(QString("F2"));
        QDomText f2Val = document.createTextNode(QString::number(ui->comboBox_frequency_2->currentIndex()));
        f2Tag.appendChild(f2Val);
        newFrequencyTag.appendChild(f2Tag);

        QDomElement f3Tag = document.createElement(QString("F3"));
        QDomText f3Val = document.createTextNode(QString::number(ui->comboBox_frequency_3->currentIndex()));
        f3Tag.appendChild(f3Val);
        newFrequencyTag.appendChild(f3Tag);

        QDomElement f4Tag = document.createElement(QString("F4"));
        QDomText f4Val = document.createTextNode(QString::number(ui->comboBox_frequency_4->currentIndex()));
        f4Tag.appendChild(f4Val);
        newFrequencyTag.appendChild(f4Tag);

        QDomElement f5Tag = document.createElement(QString("F5"));
        QDomText f5Val = document.createTextNode(QString::number(ui->comboBox_frequency_5->currentIndex()));
        f5Tag.appendChild(f5Val);
        newFrequencyTag.appendChild(f5Tag);

        root.appendChild(newFrequencyTag);
    }

    else
    {
        QDomElement root = document.documentElement();
        QDomNode SettingsNode = root.namedItem("FrequencyIndex");

        QDomNode f1 = SettingsNode.namedItem("F1");
        f1.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_1->currentIndex()));
        QDomNode f2 = SettingsNode.namedItem("F2");
        f2.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_2->currentIndex()));
        QDomNode f3 = SettingsNode.namedItem("F3");
        f3.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_3->currentIndex()));
        QDomNode f4 = SettingsNode.namedItem("F4");
        f4.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_4->currentIndex()));
        QDomNode f5 = SettingsNode.namedItem("F5");
        f5.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_5->currentIndex()));

    }

    if (FrequencyValNodeVal.isNull())
    {
        QDomElement f1Tag = document.createElement(QString("F1"));
        QDomText f1Val = document.createTextNode(QString::number(ui->comboBox_frequency_1->itemData(ui->comboBox_frequency_1->currentIndex()).toInt()/m_freqDiv));
        f1Tag.appendChild(f1Val);
        newFrequencyValTag.appendChild(f1Tag);

        QDomElement f2Tag = document.createElement(QString("F2"));
        QDomText f2Val = document.createTextNode(QString::number(ui->comboBox_frequency_2->itemData(ui->comboBox_frequency_2->currentIndex()).toInt()/m_freqDiv));
        f1Tag.appendChild(f1Val);
        f2Tag.appendChild(f2Val);
        newFrequencyValTag.appendChild(f2Tag);

        QDomElement f3Tag = document.createElement(QString("F3"));
        QDomText f3Val = document.createTextNode(QString::number(ui->comboBox_frequency_3->itemData(ui->comboBox_frequency_3->currentIndex()).toInt()/m_freqDiv));
        f1Tag.appendChild(f1Val);
        f3Tag.appendChild(f3Val);
        newFrequencyValTag.appendChild(f3Tag);

        QDomElement f4Tag = document.createElement(QString("F4"));
        QDomText f4Val = document.createTextNode(QString::number(ui->comboBox_frequency_4->itemData(ui->comboBox_frequency_4->currentIndex()).toInt()/m_freqDiv));
        f1Tag.appendChild(f1Val);
        f4Tag.appendChild(f4Val);
        newFrequencyValTag.appendChild(f4Tag);

        QDomElement f5Tag = document.createElement(QString("F5"));
        QDomText f5Val = document.createTextNode(QString::number(ui->comboBox_frequency_5->itemData(ui->comboBox_frequency_5->currentIndex()).toInt()/m_freqDiv));
        f1Tag.appendChild(f1Val);
        f5Tag.appendChild(f5Val);
        newFrequencyValTag.appendChild(f5Tag);

        root.appendChild(newFrequencyValTag);
    }

    else
    {
        QDomElement root = document.documentElement();
        QDomNode SettingsNode = root.namedItem("Frequency");

        QDomNode f1 = SettingsNode.namedItem("F1");
        f1.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_1->itemData(ui->comboBox_frequency_1->currentIndex()).toInt()/m_freqDiv));
        QDomNode f2 = SettingsNode.namedItem("F2");
        f2.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_2->itemData(ui->comboBox_frequency_2->currentIndex()).toInt()/m_freqDiv));
        QDomNode f3 = SettingsNode.namedItem("F3");
        f3.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_3->itemData(ui->comboBox_frequency_3->currentIndex()).toInt()/m_freqDiv));
        QDomNode f4 = SettingsNode.namedItem("F4");
        f4.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_4->itemData(ui->comboBox_frequency_4->currentIndex()).toInt()/m_freqDiv));
        QDomNode f5 = SettingsNode.namedItem("F5");
        f5.firstChild().setNodeValue(QString::number(ui->comboBox_frequency_5->itemData(ui->comboBox_frequency_5->currentIndex()).toInt()/m_freqDiv));

    }


    if (WaveformNodeVal.isNull())
    {
        QDomElement w1Tag = document.createElement(QString("W1"));
        QDomText w1Val = document.createTextNode(QString::number(ui->comboBox_waveform_1->currentIndex()));
        w1Tag.appendChild(w1Val);
        newWaveformTag.appendChild(w1Tag);

        QDomElement w2Tag = document.createElement(QString("W2"));
        QDomText w2Val = document.createTextNode(QString::number(ui->comboBox_waveform_2->currentIndex()));
        w2Tag.appendChild(w2Val);
        newWaveformTag.appendChild(w2Tag);

        QDomElement w3Tag = document.createElement(QString("W3"));
        QDomText w3Val = document.createTextNode(QString::number(ui->comboBox_waveform_3->currentIndex()));
        w3Tag.appendChild(w3Val);
        newWaveformTag.appendChild(w3Tag);

        QDomElement w4Tag = document.createElement(QString("W4"));
        QDomText w4Val = document.createTextNode(QString::number(ui->comboBox_waveform_4->currentIndex()));
        w4Tag.appendChild(w4Val);
        newWaveformTag.appendChild(w4Tag);

        QDomElement w5Tag = document.createElement(QString("W5"));
        QDomText w5Val = document.createTextNode(QString::number(ui->comboBox_waveform_5->currentIndex()));
        w5Tag.appendChild(w5Val);
        newWaveformTag.appendChild(w5Tag);

        root.appendChild(newWaveformTag);
    }

    else
    {
        QDomElement root = document.documentElement();
        QDomNode SettingsNode = root.namedItem("WaveformIndex");

        QDomNode w1 = SettingsNode.namedItem("W1");
        w1.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_1->currentIndex()));
        QDomNode w2 = SettingsNode.namedItem("W2");
        w2.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_2->currentIndex()));
        QDomNode w3 = SettingsNode.namedItem("W3");
        w3.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_3->currentIndex()));
        QDomNode w4 = SettingsNode.namedItem("W4");
        w4.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_4->currentIndex()));
        QDomNode w5 = SettingsNode.namedItem("W5");
        w5.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_5->currentIndex()));

    }

    if (WaveformValNodeVal.isNull())
    {
        QDomElement w1Tag = document.createElement(QString("W1"));
        QDomText w1Val = document.createTextNode(QString::number(ui->comboBox_waveform_1->itemData(ui->comboBox_waveform_1->currentIndex()).toInt()));
        w1Tag.appendChild(w1Val);
        newWaveformValTag.appendChild(w1Tag);

        QDomElement w2Tag = document.createElement(QString("W2"));
        QDomText w2Val = document.createTextNode(QString::number(ui->comboBox_waveform_2->itemData(ui->comboBox_waveform_2->currentIndex()).toInt()));

        w2Tag.appendChild(w2Val);
        newWaveformValTag.appendChild(w2Tag);

        QDomElement w3Tag = document.createElement(QString("W3"));
        QDomText w3Val = document.createTextNode(QString::number(ui->comboBox_waveform_3->itemData(ui->comboBox_waveform_3->currentIndex()).toInt()));

        w3Tag.appendChild(w3Val);
        newWaveformValTag.appendChild(w3Tag);

        QDomElement w4Tag = document.createElement(QString("W4"));
        QDomText w4Val = document.createTextNode(QString::number(ui->comboBox_waveform_4->itemData(ui->comboBox_waveform_4->currentIndex()).toInt()));

        w4Tag.appendChild(w4Val);
        newWaveformValTag.appendChild(w4Tag);

        QDomElement w5Tag = document.createElement(QString("W5"));
        QDomText w5Val = document.createTextNode(QString::number(ui->comboBox_waveform_5->itemData(ui->comboBox_waveform_5->currentIndex()).toInt()));

        w5Tag.appendChild(w5Val);
        newWaveformValTag.appendChild(w5Tag);

        root.appendChild(newWaveformValTag);
    }

    else
    {
        QDomElement root = document.documentElement();
        QDomNode SettingsNode = root.namedItem("Waveform");

        QDomNode w1 = SettingsNode.namedItem("W1");
        w1.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_1->itemData(ui->comboBox_waveform_1->currentIndex()).toInt()));
        QDomNode w2 = SettingsNode.namedItem("W2");
        w2.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_2->itemData(ui->comboBox_waveform_2->currentIndex()).toInt()));
        QDomNode w3 = SettingsNode.namedItem("W3");
        w3.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_3->itemData(ui->comboBox_waveform_3->currentIndex()).toInt()));
        QDomNode w4 = SettingsNode.namedItem("W4");
        w4.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_4->itemData(ui->comboBox_waveform_4->currentIndex()).toInt()));
        QDomNode w5 = SettingsNode.namedItem("W5");
        w5.firstChild().setNodeValue(QString::number(ui->comboBox_waveform_5->itemData(ui->comboBox_waveform_5->currentIndex()).toInt()));

    }

    if (MuscleValNodeVal.isNull())
    {
        QDomElement w1Tag = document.createElement(QString("MN1"));
        QDomText w1Val = document.createTextNode(QString::number(ui->comboBox_m1->currentIndex()));
        w1Tag.appendChild(w1Val);
        newMuscleValTag.appendChild(w1Tag);

        QDomElement w2Tag = document.createElement(QString("MN2"));
        QDomText w2Val = document.createTextNode(QString::number(ui->comboBox_m2->currentIndex()));
        w2Tag.appendChild(w2Val);
        newMuscleValTag.appendChild(w2Tag);

        QDomElement w3Tag = document.createElement(QString("MN3"));
        QDomText w3Val = document.createTextNode(QString::number(ui->comboBox_m3->currentIndex()));
        w3Tag.appendChild(w3Val);
        newMuscleValTag.appendChild(w3Tag);

        QDomElement w4Tag = document.createElement(QString("MN4"));
        QDomText w4Val = document.createTextNode(QString::number(ui->comboBox_m4->currentIndex()));
        w4Tag.appendChild(w4Val);
        newMuscleValTag.appendChild(w4Tag);

        QDomElement w5Tag = document.createElement(QString("MN5"));
        QDomText w5Val = document.createTextNode(QString::number(ui->comboBox_m5->currentIndex()));
        w5Tag.appendChild(w5Val);
        newMuscleValTag.appendChild(w5Tag);

        root.appendChild(newMuscleValTag);
    }

    else
    {
        QDomElement root = document.documentElement();
        QDomNode SettingsNode = root.namedItem("MuscleName");

        QDomNode w1 = SettingsNode.namedItem("MN1");
        w1.firstChild().setNodeValue(QString::number(ui->comboBox_m1->currentIndex()));
        QDomNode w2 = SettingsNode.namedItem("MN2");
        w2.firstChild().setNodeValue(QString::number(ui->comboBox_m2->currentIndex()));
        QDomNode w3 = SettingsNode.namedItem("MN3");
        w3.firstChild().setNodeValue(QString::number(ui->comboBox_m3->currentIndex()));
        QDomNode w4 = SettingsNode.namedItem("MN4");
        w4.firstChild().setNodeValue(QString::number(ui->comboBox_m4->currentIndex()));
        QDomNode w5 = SettingsNode.namedItem("MN5");
        w5.firstChild().setNodeValue(QString::number(ui->comboBox_m5->currentIndex()));

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






void stageProgram::on_tabWidget_currentChanged(int index)
{
    if (index == 1)
        {

            saveToXMLFile();
            QDomDocument document;
            QString xmlName = pLabel;
            QString xmlReadPath = QCoreApplication::applicationDirPath() + "/data/" + xmlName + ".xml";
            QFile xmlfile(xmlReadPath);

            if (!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Error opening XML file: " << xmlfile.errorString();
                return; // Return if the file cannot be opened
            }

            document.setContent(&xmlfile);
            QDomElement root = document.documentElement();
            xmlfile.close();

            QDomNodeList currentNodes = root.elementsByTagName("Current");
            QDomNodeList sp1Nodes = root.elementsByTagName("PW_Stroke1");
            QDomNodeList sp2Nodes = root.elementsByTagName("PW_Stroke2");
            QDomNodeList sp3Nodes = root.elementsByTagName("PW_Stroke3");
            QDomNodeList sp4Nodes = root.elementsByTagName("PW_Stroke4");
            QDomNodeList sp5Nodes = root.elementsByTagName("PW_Stroke5");
            QDomNodeList sp6Nodes = root.elementsByTagName("PW_Stroke6");

            ManageConfigFile configFile;

//            if (sp1Nodes.at(0).firstChild().isNull())
//                configFile.sProg1Test(pLabel);
//            else
//                configFile.sProg1Final(pLabel);

//            if (sp2Nodes.at(0).firstChild().isNull())
//                configFile.sProg2Test(pLabel);
//            else
//                configFile.sProg2Final(pLabel);

//            if (sp3Nodes.at(0).firstChild().isNull())
//                configFile.sProg3Test(pLabel);
//            else
//                configFile.sProg3Final(pLabel);

//            if (sp4Nodes.at(0).firstChild().isNull())
//                configFile.sProg4Test(pLabel);
//            else
//                configFile.sProg4Final(pLabel);

//            if (sp5Nodes.at(0).firstChild().isNull())
//                configFile.sProg5Test(pLabel);
//            else
//                configFile.sProg5Final(pLabel);

            // Always create test configurations for all nodes
            configFile.sProg1Test(pLabel);
            configFile.sProg2Test(pLabel);
            configFile.sProg3Test(pLabel);
            configFile.sProg4Test(pLabel);
            configFile.sProg5Test(pLabel);
            configFile.sProg6Test(pLabel);

            // Create final configurations only if the nodes are present
            if (!sp1Nodes.at(0).firstChild().isNull()) {
                configFile.sProg1Final(pLabel);

            }

            if (!sp2Nodes.at(0).firstChild().isNull()) {
                configFile.sProg2Final(pLabel);
            }

            if (!sp3Nodes.at(0).firstChild().isNull()) {
                configFile.sProg3Final(pLabel);
                //ui->label_2->setText("Sp3 presnet");
            }

            if (!sp4Nodes.at(0).firstChild().isNull()) {
                configFile.sProg4Final(pLabel);
            }

            if (!sp5Nodes.at(0).firstChild().isNull()) {
                configFile.sProg5Final(pLabel);
            }

            if (!sp6Nodes.at(0).firstChild().isNull()) {
                configFile.sProg6Final(pLabel);
            }
        }
        else
        {
            tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);
            // disconnect(&api, &tetra_grip_api::tetraGripSensorEvent, this, &stageProgram::sensorEventHandler);
        }

        // if (index == 2)
        // {
        //     if (!saveClicked)
        //     {
        //         QMessageBox::warning(this, "TetraGrip", "Save Current Settings", QMessageBox::Ok, QMessageBox::NoButton);
        //     }
        // }

}

void stageProgram::on_tabWidget_tabCloseRequested(int index)
{

}

void stageProgram::on_pushButton_shoulderControl_clicked()
{
    //    this->close();
    //    window = new ShoulderControl(pLabel);
    //    window -> show();

    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    // disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);

    this-> close();
    StageOneMain *newPatient = new StageOneMain(pLabel);
    //newPatient->setAttribute(Qt::WA_DeleteOnClose);
    newPatient-> show();
}

void stageProgram::on_pushButton_5_clicked()


{

    ManageConfigFile configFile1;
    configFile1.TetraGripFinal(pLabel);

    disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &stageProgram::stimStatusEventHandler);
    //disconnect(&api, &tetra_grip_api::tetraGripSensorEvent,this, &stageProgram::sensorEventHandler);
    this->close();
    statewindow = new StateTestWindow(pLabel,nullptr);
    statewindow->setAttribute(Qt::WA_DeleteOnClose);
    statewindow -> show();
}

void stageProgram::on_radioButton_3_clicked()
{
    tetra_grip_api::set_stimulation_ramp_rate(m_channelOne, 0, 9000); // 0.5 seconds
    tetra_grip_api::set_stimulation_ramp_rate(m_channelTwo, 0, 9000);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelThree , 0, 9000);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFour, 0, 9000);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFive, 0, 9000);
}

void stageProgram::on_radioButton_clicked()
{
    tetra_grip_api::set_stimulation_ramp_rate(m_channelOne, 0, 4500); // 1 second ((180-0)/(40*1))*1000
    tetra_grip_api::set_stimulation_ramp_rate(m_channelTwo, 0, 4500);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelThree, 0, 4500);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFour, 0, 4500);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFive, 0, 4500);
}

void stageProgram::on_radioButton_2_clicked()
{
    tetra_grip_api::set_stimulation_ramp_rate(m_channelOne, 0, 2250); // 2seconds
    tetra_grip_api::set_stimulation_ramp_rate(m_channelTwo, 0, 2250);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelThree, 0, 2250);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFour, 0, 2250);
    tetra_grip_api::set_stimulation_ramp_rate(m_channelFive, 0, 2250);
}


void stageProgram::on_comboBox_frequency_1_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_frequency_1->itemData(index).toUInt();
    oneFreqStim = value;
    tetra_grip_api::stimulation_set_frequency(m_channelOne, oneFreqStim);

}

void stageProgram::on_comboBox_frequency_2_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_frequency_2->itemData(index).toUInt();
    twoFreqStim = value;
    tetra_grip_api::stimulation_set_frequency(m_channelTwo, twoFreqStim);
}

void stageProgram::on_comboBox_frequency_3_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_frequency_3->itemData(index).toUInt();
    threeFreqStim = value;
    tetra_grip_api::stimulation_set_frequency(m_channelThree, threeFreqStim);
}

void stageProgram::on_comboBox_frequency_4_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_frequency_4->itemData(index).toUInt();
    fourFreqStim = value;
    tetra_grip_api::stimulation_set_frequency(m_channelFour, fourFreqStim);
}

void stageProgram::on_comboBox_frequency_5_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_frequency_5->itemData(index).toUInt();
    fiveFreqStim = value;
    tetra_grip_api::stimulation_set_frequency(m_channelFive, fiveFreqStim);
}

void stageProgram::on_comboBox_waveform_1_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_waveform_1->itemData(index).toUInt();
    oneWaveStim = value;
    tetra_grip_api::stimulation_set_waveform(m_channelOne,0, oneWaveStim);
}

void stageProgram::on_comboBox_waveform_2_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_waveform_2->itemData(index).toUInt();
    twoWaveStim = value;
    tetra_grip_api::stimulation_set_waveform(m_channelTwo,0, twoWaveStim);
}

void stageProgram::on_comboBox_waveform_3_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_waveform_3->itemData(index).toUInt();
    threeWaveStim = value;
    tetra_grip_api::stimulation_set_waveform(m_channelThree,0, threeWaveStim);
}

void stageProgram::on_comboBox_waveform_4_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_waveform_4->itemData(index).toUInt();
    fourWaveStim = value;
    tetra_grip_api::stimulation_set_waveform(m_channelFour,0, fourWaveStim);
}

void stageProgram::on_comboBox_waveform_5_currentIndexChanged(int index)
{
    unsigned int value = ui->comboBox_waveform_5->itemData(index).toUInt();
    fiveWaveStim = value;
    tetra_grip_api::stimulation_set_waveform(m_channelFive,0, fiveWaveStim);
}

void stageProgram::on_pushButton_stimSave_2_clicked()
{
//       this->close();
//        protocolwindow = new ProtocolWindow (pLabel, nullptr);
//        protocolwindow -> setAttribute(Qt::WA_DeleteOnClose);
//        protocolwindow -> show();
}

void stageProgram::on_pushButton_programStroke2_clicked()
{

//    QDomDocument document;
//    QString xmlName = pLabel;
//    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
//    QFile xmlfile(xmlReadPath);

//    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qDebug () << "Error opening XML file: "<<xmlfile.errorString();

//    }

//    document.setContent(&xmlfile);
//    QDomElement root = document.documentElement();
//    xmlfile.close();


//    QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
//    QDomElement CurrentNodeVal = CurrentNode.toElement();
//    QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke1").at(0).firstChild();
//    QDomElement SP1PWNodeVal = SP1PWNode.toElement();
//    if(SP1PWNode.isNull())
//    {


//        this->close();
//        strokewindow = new StrokeTrainingWindow(pLabel, 0,0);
//        strokewindow->show();

//    }
//    else
//    {
//        this->close();
//        strokewindow = new StrokeTrainingWindow(pLabel, 0,1);
//        strokewindow->show();
//    }

//    if(CurrentNode.isNull())
//    {
//        QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
//        return;
//    }

//    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

    QDomDocument document;
    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();
        return;
    }

    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QMap<QString, QString> defaultValues;

    // Add tooltip texts of combo boxes to defaultValues
    int index_m1 = ui->comboBox_m1->currentIndex();
    QString tooltipText_m1 = ui->comboBox_m1->itemData(index_m1, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m1"] = tooltipText_m1;

    int index_m2 = ui->comboBox_m2->currentIndex();
    QString tooltipText_m2 = ui->comboBox_m2->itemData(index_m2, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m2"] = tooltipText_m2;

    int index_m3 = ui->comboBox_m3->currentIndex();
    QString tooltipText_m3 = ui->comboBox_m3->itemData(index_m3, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m3"] = tooltipText_m3;

    int index_m4 = ui->comboBox_m4->currentIndex();
    QString tooltipText_m4 = ui->comboBox_m4->itemData(index_m4, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m4"] = tooltipText_m4;

    int index_m5 = ui->comboBox_m5->currentIndex();
    QString tooltipText_m5 = ui->comboBox_m5->itemData(index_m5, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m5"] = tooltipText_m5;

    // Extracting Current values
        QDomNodeList currentNodeList = root.elementsByTagName("Current").at(0).childNodes();
        for(int i = 0; i < currentNodeList.count(); i++) {
            QDomElement element = currentNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Current_" + element.tagName()] = element.text();
            }
        }

        // Extracting pulsewidth values
        QDomNodeList pulsewidthStrokeProg2NodeList = root.elementsByTagName("PW_Stroke1").at(0).childNodes();
        for(int i = 0; i < pulsewidthStrokeProg2NodeList.count(); i++) {
            QDomElement element = pulsewidthStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["PW_Stroke1_" + element.tagName()] = element.text();
            }
        }

        // Extracting Ramp values
        QDomNodeList rampStrokeProg2NodeList = root.elementsByTagName("Ramp_Stroke1").at(0).childNodes();
        for(int i = 0; i < rampStrokeProg2NodeList.count(); i++) {
            QDomElement element = rampStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Ramp_Stroke1_" + element.tagName()] = element.text();
            }
        }

        // Extracting Delay values
        QDomNodeList delayStrokeProg2IndexNodeList = root.elementsByTagName("Delay_Stroke1").at(0).childNodes();
        for(int i = 0; i < delayStrokeProg2IndexNodeList.count(); i++) {
            QDomElement element = delayStrokeProg2IndexNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Delay_Stroke1_" + element.tagName()] = element.text();
            }
        }

        // Extracting Duration values
        QDomNodeList TDur2NodeList = root.elementsByTagName("TDur_Stroke1").at(0).childNodes();
        for(int i = 0; i < TDur2NodeList.count(); i++) {
            QDomElement element = TDur2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["TDur_Stroke1_" + element.tagName()] = element.text();
            }
        }


        QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
        QDomElement CurrentNodeVal = CurrentNode.toElement();

        if(CurrentNode.isNull())
        {
            QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
            return;
        }
        QDomNode SP5PWNode = root.elementsByTagName("PW_Stroke1").at(0).firstChild();
        QDomElement SP5PWNodeVal = SP5PWNode.toElement();

        int configStatus = SP5PWNode.isNull() ? 0 : 1;


        this->close();
        strokewindow = new StrokeTrainingWindow(pLabel,defaultValues, 0,configStatus);
        strokewindow->show();

}


void stageProgram::on_pushButton_programStroke1_clicked()
{


        QDomDocument document;
        QString xmlName = pLabel;
        QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
        QFile xmlfile(xmlReadPath);

        if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug () << "Error opening XML file: "<<xmlfile.errorString();
            return;
        }

        document.setContent(&xmlfile);
        QDomElement root = document.documentElement();
        xmlfile.close();

        QMap<QString, QString> defaultValues;
        // Add tooltip texts of combo boxes to defaultValues
        int index_m1 = ui->comboBox_m1->currentIndex();
        QString tooltipText_m1 = ui->comboBox_m1->itemData(index_m1, Qt::ToolTipRole).toString();
        defaultValues["Tooltip_comboBox_m1"] = tooltipText_m1;

        int index_m2 = ui->comboBox_m2->currentIndex();
        QString tooltipText_m2 = ui->comboBox_m2->itemData(index_m2, Qt::ToolTipRole).toString();
        defaultValues["Tooltip_comboBox_m2"] = tooltipText_m2;

        int index_m3 = ui->comboBox_m3->currentIndex();
        QString tooltipText_m3 = ui->comboBox_m3->itemData(index_m3, Qt::ToolTipRole).toString();
        defaultValues["Tooltip_comboBox_m3"] = tooltipText_m3;

        int index_m4 = ui->comboBox_m4->currentIndex();
        QString tooltipText_m4 = ui->comboBox_m4->itemData(index_m4, Qt::ToolTipRole).toString();
        defaultValues["Tooltip_comboBox_m4"] = tooltipText_m4;

        int index_m5 = ui->comboBox_m5->currentIndex();
        QString tooltipText_m5 = ui->comboBox_m5->itemData(index_m5, Qt::ToolTipRole).toString();
        defaultValues["Tooltip_comboBox_m5"] = tooltipText_m5;

        // Extracting Current values
            QDomNodeList currentNodeList = root.elementsByTagName("Current").at(0).childNodes();
            for(int i = 0; i < currentNodeList.count(); i++) {
                QDomElement element = currentNodeList.at(i).toElement();
                if (!element.isNull()) {
                    defaultValues["Current_" + element.tagName()] = element.text();
                }
            }

            // Extracting pulsewidth values
            QDomNodeList pulsewidthStrokeProg2NodeList = root.elementsByTagName("PW_Stroke2").at(0).childNodes();
            for(int i = 0; i < pulsewidthStrokeProg2NodeList.count(); i++) {
                QDomElement element = pulsewidthStrokeProg2NodeList.at(i).toElement();
                if (!element.isNull()) {
                    defaultValues["PW_Stroke2_" + element.tagName()] = element.text();
                }
            }

            // Extracting Ramp values
            QDomNodeList rampStrokeProg2NodeList = root.elementsByTagName("Ramp_Stroke2").at(0).childNodes();
            for(int i = 0; i < rampStrokeProg2NodeList.count(); i++) {
                QDomElement element = rampStrokeProg2NodeList.at(i).toElement();
                if (!element.isNull()) {
                    defaultValues["Ramp_Stroke2_" + element.tagName()] = element.text();
                }
            }

            // Extracting Delay values
            QDomNodeList delayStrokeProg2IndexNodeList = root.elementsByTagName("Delay_Stroke2").at(0).childNodes();
            for(int i = 0; i < delayStrokeProg2IndexNodeList.count(); i++) {
                QDomElement element = delayStrokeProg2IndexNodeList.at(i).toElement();
                if (!element.isNull()) {
                    defaultValues["Delay_Stroke2_" + element.tagName()] = element.text();
                }
            }

            // Extracting Duration values
            QDomNodeList TDur2NodeList = root.elementsByTagName("TDur_Stroke2").at(0).childNodes();
            for(int i = 0; i < TDur2NodeList.count(); i++) {
                QDomElement element = TDur2NodeList.at(i).toElement();
                if (!element.isNull()) {
                    defaultValues["TDur_Stroke2_" + element.tagName()] = element.text();
                }
            }

            QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
            QDomElement CurrentNodeVal = CurrentNode.toElement();
            QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke2").at(0).firstChild();
            QDomElement SP1PWNodeVal = SP1PWNode.toElement();

            int configStatus = SP1PWNode.isNull() ? 0 : 1;

            if(CurrentNode.isNull())
            {
                QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
                return;
            }



            this->close();
            strokewindowtwo = new StrokeTrainingWindowTwo(pLabel,defaultValues, 0,configStatus);
            strokewindowtwo->show();


}


void stageProgram::on_pushButton_programStroke_3_clicked()
{

    QDomDocument document;
    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();
        return;
    }

    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QMap<QString, QString> defaultValues;
    // Add tooltip texts of combo boxes to defaultValues
    int index_m1 = ui->comboBox_m1->currentIndex();
    QString tooltipText_m1 = ui->comboBox_m1->itemData(index_m1, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m1"] = tooltipText_m1;

    int index_m2 = ui->comboBox_m2->currentIndex();
    QString tooltipText_m2 = ui->comboBox_m2->itemData(index_m2, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m2"] = tooltipText_m2;

    int index_m3 = ui->comboBox_m3->currentIndex();
    QString tooltipText_m3 = ui->comboBox_m3->itemData(index_m3, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m3"] = tooltipText_m3;

    int index_m4 = ui->comboBox_m4->currentIndex();
    QString tooltipText_m4 = ui->comboBox_m4->itemData(index_m4, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m4"] = tooltipText_m4;

    int index_m5 = ui->comboBox_m5->currentIndex();
    QString tooltipText_m5 = ui->comboBox_m5->itemData(index_m5, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m5"] = tooltipText_m5;

    // Extracting Current values
        QDomNodeList currentNodeList = root.elementsByTagName("Current").at(0).childNodes();
        for(int i = 0; i < currentNodeList.count(); i++) {
            QDomElement element = currentNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Current_" + element.tagName()] = element.text();
            }
        }

        // Extracting pulsewidth values
        QDomNodeList pulsewidthStrokeProg2NodeList = root.elementsByTagName("PW_Stroke3").at(0).childNodes();
        for(int i = 0; i < pulsewidthStrokeProg2NodeList.count(); i++) {
            QDomElement element = pulsewidthStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["PW_Stroke3_" + element.tagName()] = element.text();
            }
        }

        // Extracting Ramp values
        QDomNodeList rampStrokeProg2NodeList = root.elementsByTagName("Ramp_Stroke3").at(0).childNodes();
        for(int i = 0; i < rampStrokeProg2NodeList.count(); i++) {
            QDomElement element = rampStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Ramp_Stroke2_" + element.tagName()] = element.text();
            }
        }


        // Extracting Duration values
        QDomNodeList TDur2NodeList = root.elementsByTagName("TDur_Stroke3").at(0).childNodes();
        for(int i = 0; i < TDur2NodeList.count(); i++) {
            QDomElement element = TDur2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["TDur_Stroke3_" + element.tagName()] = element.text();
            }
        }

        QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
        QDomElement CurrentNodeVal = CurrentNode.toElement();
        QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke3").at(0).firstChild();
        QDomElement SP1PWNodeVal = SP1PWNode.toElement();

        int configStatus = SP1PWNode.isNull() ? 0 : 1;

        if(CurrentNode.isNull())
        {
            QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
            return;
        }


    this->close();
    strokewindowthree = new StrokeTrainingWindowThree(pLabel,defaultValues, 0,configStatus);
    strokewindowthree->show();
}


void stageProgram::on_pushButton_programStroke4_clicked()
{
    QDomDocument document;
    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();
        return;
    }

    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QMap<QString, QString> defaultValues;
    // Add tooltip texts of combo boxes to defaultValues
    int index_m1 = ui->comboBox_m1->currentIndex();
    QString tooltipText_m1 = ui->comboBox_m1->itemData(index_m1, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m1"] = tooltipText_m1;

    int index_m2 = ui->comboBox_m2->currentIndex();
    QString tooltipText_m2 = ui->comboBox_m2->itemData(index_m2, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m2"] = tooltipText_m2;

    int index_m3 = ui->comboBox_m3->currentIndex();
    QString tooltipText_m3 = ui->comboBox_m3->itemData(index_m3, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m3"] = tooltipText_m3;

    int index_m4 = ui->comboBox_m4->currentIndex();
    QString tooltipText_m4 = ui->comboBox_m4->itemData(index_m4, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m4"] = tooltipText_m4;

    int index_m5 = ui->comboBox_m5->currentIndex();
    QString tooltipText_m5 = ui->comboBox_m5->itemData(index_m5, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m5"] = tooltipText_m5;

    // Extracting Current values
        QDomNodeList currentNodeList = root.elementsByTagName("Current").at(0).childNodes();
        for(int i = 0; i < currentNodeList.count(); i++) {
            QDomElement element = currentNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Current_" + element.tagName()] = element.text();
            }
        }

        // Extracting pulsewidth values
        QDomNodeList pulsewidthStrokeProg2NodeList = root.elementsByTagName("PW_Stroke4").at(0).childNodes();
        for(int i = 0; i < pulsewidthStrokeProg2NodeList.count(); i++) {
            QDomElement element = pulsewidthStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["PW_Stroke4_" + element.tagName()] = element.text();
            }
        }

        // Extracting Ramp values
        QDomNodeList rampStrokeProg2NodeList = root.elementsByTagName("Ramp_Stroke4").at(0).childNodes();
        for(int i = 0; i < rampStrokeProg2NodeList.count(); i++) {
            QDomElement element = rampStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Ramp_Stroke4_" + element.tagName()] = element.text();
            }
        }


        // Extracting Duration values
        QDomNodeList TDur2NodeList = root.elementsByTagName("TDur_Stroke4").at(0).childNodes();
        for(int i = 0; i < TDur2NodeList.count(); i++) {
            QDomElement element = TDur2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["TDur_Stroke4_" + element.tagName()] = element.text();
            }
        }

        QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
        QDomElement CurrentNodeVal = CurrentNode.toElement();
        QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke4").at(0).firstChild();
        QDomElement SP1PWNodeVal = SP1PWNode.toElement();

        if(CurrentNode.isNull())
        {
            QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
            return;
        }


        QDomNode SP4PWNode = root.elementsByTagName("PW_Stroke4").at(0).firstChild();
        QDomElement SP4PWNodeVal = SP4PWNode.toElement();

        int configStatus = SP4PWNode.isNull() ? 0 : 1;


        this->close();
        strokewindowfour = new StrokeTrainingWindowFour(pLabel,defaultValues, 0,configStatus);
        strokewindowfour->show();


}


void stageProgram::on_pushButton_programStroke5_clicked()
{

    QDomDocument document;
    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();
        return;
    }

    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QMap<QString, QString> defaultValues;
    // Add tooltip texts of combo boxes to defaultValues
    int index_m1 = ui->comboBox_m1->currentIndex();
    QString tooltipText_m1 = ui->comboBox_m1->itemData(index_m1, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m1"] = tooltipText_m1;

    int index_m2 = ui->comboBox_m2->currentIndex();
    QString tooltipText_m2 = ui->comboBox_m2->itemData(index_m2, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m2"] = tooltipText_m2;

    int index_m3 = ui->comboBox_m3->currentIndex();
    QString tooltipText_m3 = ui->comboBox_m3->itemData(index_m3, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m3"] = tooltipText_m3;

    int index_m4 = ui->comboBox_m4->currentIndex();
    QString tooltipText_m4 = ui->comboBox_m4->itemData(index_m4, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m4"] = tooltipText_m4;

    int index_m5 = ui->comboBox_m5->currentIndex();
    QString tooltipText_m5 = ui->comboBox_m5->itemData(index_m5, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m5"] = tooltipText_m5;

    // Extracting Current values
        QDomNodeList currentNodeList = root.elementsByTagName("Current").at(0).childNodes();
        for(int i = 0; i < currentNodeList.count(); i++) {
            QDomElement element = currentNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Current_" + element.tagName()] = element.text();
            }
        }

        // Extracting pulsewidth values
        QDomNodeList pulsewidthStrokeProg2NodeList = root.elementsByTagName("PW_Stroke5").at(0).childNodes();
        for(int i = 0; i < pulsewidthStrokeProg2NodeList.count(); i++) {
            QDomElement element = pulsewidthStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["PW_Stroke5_" + element.tagName()] = element.text();
            }
        }

        // Extracting Ramp values
        QDomNodeList rampStrokeProg2NodeList = root.elementsByTagName("Ramp_Stroke5").at(0).childNodes();
        for(int i = 0; i < rampStrokeProg2NodeList.count(); i++) {
            QDomElement element = rampStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Ramp_Stroke5_" + element.tagName()] = element.text();
            }
        }

        // Extracting Delay values
        QDomNodeList delayStrokeProg2IndexNodeList = root.elementsByTagName("Delay_Stroke5").at(0).childNodes();
        for(int i = 0; i < delayStrokeProg2IndexNodeList.count(); i++) {
            QDomElement element = delayStrokeProg2IndexNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Delay_Stroke5_" + element.tagName()] = element.text();
            }
        }

        // Extracting Duration values
        QDomNodeList TDur2NodeList = root.elementsByTagName("TDur_Stroke5").at(0).childNodes();
        for(int i = 0; i < TDur2NodeList.count(); i++) {
            QDomElement element = TDur2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["TDur_Stroke5_" + element.tagName()] = element.text();
            }
        }


        QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
        QDomElement CurrentNodeVal = CurrentNode.toElement();

        if(CurrentNode.isNull())
        {
            QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
            return;
        }
        QDomNode SP5PWNode = root.elementsByTagName("PW_Stroke5").at(0).firstChild();
        QDomElement SP5PWNodeVal = SP5PWNode.toElement();

        int configStatus = SP5PWNode.isNull() ? 0 : 1;


        this->close();
        strokewindowfive = new StrokeTrainingWindowFive(pLabel,defaultValues, 0,configStatus);
        strokewindowfive->show();
}

void stageProgram::scaleWidgets(QWidget *parent, double scaleFactorWidth, double scaleFactorHeight)
{/*
    foreach (QWidget *widget, parent->findChildren<QWidget*>())
       {
           // Scale the size of the widget
           QSize originalSize = widget->size();
           int newWidth = originalSize.width() * scaleFactorWidth;
           int newHeight = originalSize.height() * scaleFactorHeight;
           widget->resize(newWidth, newHeight);

           // Scale the position of the widget
           QPoint originalPos = widget->pos();
           int newX = originalPos.x() * scaleFactorWidth;
           int newY = originalPos.y() * scaleFactorHeight;
           widget->move(newX, newY);

           // Recursively scale child widgets
           scaleWidgets(widget, scaleFactorWidth, scaleFactorHeight);
       }*/
}


void stageProgram::on_pushButton_IMUcontrol_clicked()
{
    //disconnect(&api, &tetra_grip_api::deviceError, this, &StageOneMain::connectionError);
   // disconnect(&api, &tetra_grip_api::tetraGripEvent,this, &StageOneMain::eventHandler);
    this->close();
    window = new ShoulderControl(pLabel, nullptr);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window -> show();
}


void stageProgram::on_pushButton_programStroke6_clicked()
{
    QDomDocument document;
    QString xmlName = pLabel;
    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
    QFile xmlfile(xmlReadPath);

    if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug () << "Error opening XML file: "<<xmlfile.errorString();
        return;
    }

    document.setContent(&xmlfile);
    QDomElement root = document.documentElement();
    xmlfile.close();

    QMap<QString, QString> defaultValues;
    // Add tooltip texts of combo boxes to defaultValues
    int index_m1 = ui->comboBox_m1->currentIndex();
    QString tooltipText_m1 = ui->comboBox_m1->itemData(index_m1, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m1"] = tooltipText_m1;

    int index_m2 = ui->comboBox_m2->currentIndex();
    QString tooltipText_m2 = ui->comboBox_m2->itemData(index_m2, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m2"] = tooltipText_m2;

    int index_m3 = ui->comboBox_m3->currentIndex();
    QString tooltipText_m3 = ui->comboBox_m3->itemData(index_m3, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m3"] = tooltipText_m3;

    int index_m4 = ui->comboBox_m4->currentIndex();
    QString tooltipText_m4 = ui->comboBox_m4->itemData(index_m4, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m4"] = tooltipText_m4;

    int index_m5 = ui->comboBox_m5->currentIndex();
    QString tooltipText_m5 = ui->comboBox_m5->itemData(index_m5, Qt::ToolTipRole).toString();
    defaultValues["Tooltip_comboBox_m5"] = tooltipText_m5;

    // Extracting Current values
        QDomNodeList currentNodeList = root.elementsByTagName("Current").at(0).childNodes();
        for(int i = 0; i < currentNodeList.count(); i++) {
            QDomElement element = currentNodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Current_" + element.tagName()] = element.text();
            }
        }

        // Extracting pulsewidth values
        QDomNodeList pulsewidthStrokeProg2NodeList = root.elementsByTagName("PW_Stroke6").at(0).childNodes();
        for(int i = 0; i < pulsewidthStrokeProg2NodeList.count(); i++) {
            QDomElement element = pulsewidthStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["PW_Stroke6_" + element.tagName()] = element.text();
            }
        }

        // Extracting Ramp values
        QDomNodeList rampStrokeProg2NodeList = root.elementsByTagName("Ramp_Stroke6").at(0).childNodes();
        for(int i = 0; i < rampStrokeProg2NodeList.count(); i++) {
            QDomElement element = rampStrokeProg2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["Ramp_Stroke6_" + element.tagName()] = element.text();
            }
        }


        // Extracting Duration values
        QDomNodeList TDur2NodeList = root.elementsByTagName("TDur_Stroke6").at(0).childNodes();
        for(int i = 0; i < TDur2NodeList.count(); i++) {
            QDomElement element = TDur2NodeList.at(i).toElement();
            if (!element.isNull()) {
                defaultValues["TDur_Stroke6_" + element.tagName()] = element.text();
            }
        }

        QDomNode CurrentNode = root.elementsByTagName("Current").at(0).firstChild();
        QDomElement CurrentNodeVal = CurrentNode.toElement();
        QDomNode SP1PWNode = root.elementsByTagName("PW_Stroke6").at(0).firstChild();
        QDomElement SP1PWNodeVal = SP1PWNode.toElement();

        if(CurrentNode.isNull())
        {
            QMessageBox::information(this, "Current and Frequency Settings not saved", "You can't configure Grip phases without setting proper current and frequency values on each channels.\n\nPlease goto previous tab and set the current and frequency levels in all the channels and press save button");
            return;
        }


        QDomNode SP4PWNode = root.elementsByTagName("PW_Stroke6").at(0).firstChild();
        QDomElement SP4PWNodeVal = SP4PWNode.toElement();

        int configStatus = SP4PWNode.isNull() ? 0 : 1;


        this->close();
        strokewindowsix = new StrokeTrainingWindowSix(pLabel,defaultValues, 0,configStatus);
        strokewindowsix->show();
}




