#ifndef STROKETRAININGWINDOWFIVE_H
#define STROKETRAININGWINDOWFIVE_H

#include <QMainWindow>
#include<QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QPushButton>
#include <QCloseEvent>
#include "tetra_grip_api.h"
#include <QtXml/QDomDocument>
#include<QComboBox>
#include <QMap>

namespace Ui {
class StrokeTrainingWindowFive;
}

class StrokeTrainingWindowFive : public QMainWindow
{
    Q_OBJECT

public:
    explicit StrokeTrainingWindowFive(QString,QMap<QString, QString>, int, bool,QWidget *parent = nullptr);
    ~StrokeTrainingWindowFive();
    QString pLabel;
    int progPage;
    int configStatusId = 0;

public slots:

    void strokeTrainingPhaseEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
    //void closeEvent (QCloseEvent *event);
    int adjust_Ramp_Step_size(int pwvalue, float rmpvalue);
    void P1W1Changer_Prog5(int pwvalue);
    void P1W2Changer_Prog5(int pwvalue);
    void nextButton_Prog5(int pwvalue);

signals:
    void nxtButtonChanged_prog2(int id, int pwvalue);
    void P1W1_Value_Prog5(int);
    void P1W2_Value_Prog5(int);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_stimStart1_clicked();

    void on_pushButton_stimStop1_clicked();

    void on_btn_nextPhase1_clicked();

    void on_comboBox_p1pw1_currentIndexChanged(int index);

    void on_comboBox_p1pw2_currentIndexChanged(int index);



    void on_comboBox_p1rr1_currentIndexChanged(int index);

    void on_comboBox_p1rf1_currentIndexChanged(int index);

    void on_comboBox_p1rr2_currentIndexChanged(int index);

    void on_comboBox_p1rf2_currentIndexChanged(int index);

    void on_comboBox_p1d2_currentIndexChanged(int index);

    void on_comboBox_p1dur_currentIndexChanged(int index);

    void on_pushButton_configSet_clicked();

    void on_radioButton_p1t1_clicked();

    void on_radioButton_p1t2_clicked();

    void on_radioButton_p1t3_clicked();

    void on_comboBox_onTime1_currentIndexChanged(int index);

    void on_pushButton_triggerApply_clicked();

    void on_pushButton_download1_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::StrokeTrainingWindowFive *ui;

    QString defaultValuesToString(const QMap<QString, QString> &defaultValues);
    bool stimStopped = false;
    bool phaseOver = false;
    QString StyleSheetOn;
    QString StyleSheetOff;
    QString config_file_pr5;
    QString config_file_pr52;
    QString config_file_pr53;
    QString test_config_file_pr5;

    uint8_t m_channelOne = 0;
    uint8_t m_channelTwo = 1;
    uint8_t m_channelThree = 2;
    uint8_t m_channelFour = 3;
    uint8_t m_channelFive = 4;

    int PW_Ch1 = 180;
    int PW_Ch2 = 180;

    float RR_Ch1 = 300;
    float RR_Ch2 = 300;
    float RF_Ch1 = 300;
    float RF_Ch2 = 300;
    float Delay_Ch2 = 3000;
    float Training_Duration1 = 15;
    float ramp_step_size_rr1 = 300;
    float ramp_step_size_rr2 = 300;
    float ramp_step_size_rf1 = 300;
    float ramp_step_size_rf2 = 300;
    int frequency = 30;
    float onTime1 = 2000;
    bool buildNewConfig1=0;

    bool triggerTime = false;
    bool triggerButton = true;
    bool triggerIMU = false;

    bool resetLogStatus = false;
    QButtonGroup *btnGrp1 = nullptr;
    int m_currentBtn1 = 0;

    QSize originalSize;
    QMap<QWidget*, QRect> originalGeometries;
    QPixmap originalPixmap;
    QMap<QWidget*, int> originalFontSizes;

    void setupUiComponents(QMap<QString, QString> &defaultValues);
    void readXmlFile();
    void configureUiComponents();
    void setUiElementsEnabled(bool enabled);
    void configureConnections();
    void loadConfigFile(QString filename);
    void downloadConfigFile(QString filename);
    void saveToXMLFile();
    void loadCurrentWidgetValues(); // to lload all the widget parameter current values

protected:
     void closeEvent (QCloseEvent *event) override;
     void resizeEvent(QResizeEvent* event) override;

};

#endif // STROKETRAININGWINDOWFIVE_H
