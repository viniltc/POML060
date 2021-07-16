#ifndef PROGRAMKEYGRIPV2_H
#define PROGRAMKEYGRIPV2_H

#include <QMainWindow>
#include<QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QPushButton>
#include "tetra_grip_api.h"
#include <QtXml/QDomDocument>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ProgramKeyGripV2; }
QT_END_NAMESPACE

class ProgramKeyGripV2 : public QMainWindow
{
    Q_OBJECT

public:
    ProgramKeyGripV2(QString, QWidget *parent = nullptr);
    ~ProgramKeyGripV2();
    void paintEvent(QPaintEvent *e);

    int distance(QPoint, QPoint);
        QString pLabel;
        int pw_value = 0;



public slots:

        void changeP1value(int);
        void changeP2value(int);
        void keyGripPhaseEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
        void nextBtn(int pwvalue);
        //void nextBtn();
        void prevBtn(int pwvalue);
        void paintBtn(int id, int pwvalue);
        void getPWValue (int);
        int adjust_PW_range(int value);
        int adjust_Ramp_Step_size(int pwvalue, float rmpvalue);
        void getRampStepSize(void);
//        void onTimeout();
//        void startStopTimer();
//        void resetTimer();
        void closeEvent (QCloseEvent *event);


signals:

        void buttonOne(bool);
        void buttonTwo(bool);
        void buttonChanged(int id, int pwvalue);
        void pulseWidthValue(int);
        void lastPhase(int id);


private:
    Ui::ProgramKeyGripV2 *ui;
     bool FDS_dragging = false; // status var to see if we are dragging
     bool Ulna_dragging = false;
     bool ADP_dragging = false;
     bool EDC_Seg1_dragging = false;
     bool EDC_Seg3_dragging = false;
     bool EDC_Seg2_dragging = false;

     //EDC segments
     QPoint p41 = QPoint(150,300);
     QPoint p42 = QPoint(170,214); //250
     QPoint p43 = QPoint(250,214); //250
     QPoint p44 = QPoint(270,280);
     QPoint p45 = QPoint(450,280);
     QPoint p46 = QPoint(470,214); //250
     QPoint p47 = QPoint(550,214); //250
     QPoint p48 = QPoint(570,300);

     //FDS segments
     QPoint p11 = QPoint(230,300);
     QPoint p12 = QPoint(280,214);//170
     QPoint p13 = QPoint(450,214);//170
     QPoint p14 = QPoint(500,300);

     //Ulna segments
     QPoint p31 = QPoint(300,300);
     QPoint p32 = QPoint(320,214); //200
     QPoint p33 = QPoint(450,214); //200
     QPoint p34 = QPoint(470,300);

     //ADP segments
     QPoint p21 = QPoint(300,300);
     QPoint p22 = QPoint(320,214); //240
     QPoint p23 = QPoint(450,214); //240
     QPoint p24 = QPoint(470,300);



     QPoint handOpen1 =    QPoint(150,300);
     QPoint handOpen2 =    QPoint(150,100);
     QPoint fingerFlexion1 =  QPoint(230,300);
     QPoint fingerFlexion2 =  QPoint(230,100);
     QPoint thumbAdduct1 = QPoint(300,300);
     QPoint thumbAdduct2 = QPoint(300,100);
     QPoint handOpen11 = QPoint(450,300);
     QPoint handOpen22 = QPoint(450,100);
     QPoint handOpen111 = QPoint(550,300);
     QPoint handOpen222 = QPoint(550,100);

         QPoint *CurPoint1=nullptr;
         QPoint *CurPoint2=nullptr;
         QPoint *CurPoint3=nullptr;
         QPoint *CurPoint4=nullptr;

         bool FDS_checked = false;
         bool Ulna_checked = false;
         bool ADP_checked = false;
         bool EDC_Seg1_checked = false;
         bool EDC_Seg3_checked= false;
         bool EDC_Seg2_checked= false;

         int m_currentBtn = 0;
         QButtonGroup *btnGrp = nullptr;

         uint8_t m_channelOne = 0;
         uint8_t m_channelTwo = 1;
         uint8_t m_channelThree = 2;
         uint8_t m_channelFour = 3;
         uint8_t m_channelFive = 4;

         bool phaseOver = false;
         bool stimStopped = false;

         int PW_phase1_EDC = 180;
         int PW_phase2_EDC = 57;
         int PW_phase2_FDS = 180;
         int PW_phase3_EDC = 57;
         int PW_phase3_FDS = 180;
         int PW_phase3_Ulna = 180;
         int PW_phase3_ADP = 180;
         int PW_phase4_EDC = 180;

         int PW_EDC1 = 0;
         int PW_EDC2 = 0;
         int PW_EDC3 = 0;
         int PW_FDS = 0;
         int PW_Ulna = 0;
         int PW_ADP = 0;

         int Y_EDC1 ;
         int Y_EDC2 ;
         int Y_EDC3 ;
         int Y_FDS ;
         int Y_Ulna ;
         int Y_ADP ;

         int Y_coordinates_EDC1 = 217;
         int Y_coordinates_EDC2 = 279;
         int Y_coordinates_EDC3 = 215;
         int Y_coordinates_FDS = 217;
         int Y_coordinates_Ulna = 215;
         int Y_coordinates_ADP = 215;
         int X_coordinate_Common = 0;

         float ramp_phase1 = 0;
         float ramp_phase2 = 0;
         float ramp_phase3 = 0;
         float ramp_phase4 = 0;

         float ramp_stepsize_phase1_EDC = 0;
         float ramp_stepsize_phase2_EDC = 0;
         float ramp_stepsize_phase2_FDS = 0;
         float ramp_stepsize_phase3_ADP = 0;
         float ramp_stepsize_phase3_Ulna = 0;
         float ramp_stepsize_phase3_FDS = 0;
         float ramp_stepsize_phase3_EDC = 0;
         float ramp_stepsize_phase4_EDC = 0;

         Stopwatch* watch;

         float currOneStored =0;
         float currTwoStored =0 ;
         float currThreeStored=0 ;
         float currFourStored =0;
         float currFiveStored=0 ;
         float m_currentDiv = 1000.0;

         QString StyleSheetOn;
         QString StyleSheetOff;

         QString findXMLNodeValue(QDomElement const& root, QString const& parentname, QString const& childname);
         QString config_file_name;
         QString test_config_file_name;




protected:
   // we then override / make our own of these function to track mouse movement and clicks
         void mousePressEvent(QMouseEvent *event) ;
         void mouseReleaseEvent(QMouseEvent *event) ;
         void mouseMoveEvent(QMouseEvent *event) ;
private slots:

         void on_pushButton_back_keypro_clicked();
         void on_pushButton_nextPhase_clicked();
         void on_pushButton_stimStart_clicked();
         void on_pushButton_stimStop_clicked();
         void on_btn_nextPhase_clicked();
         void saveToXMLFile();
         void loadConfigFile(QString fileName);
         void sendConfigFile(int id);

         void on_comboBox_1_currentIndexChanged(int index);
         void on_comboBox_2_currentIndexChanged(int index);
         void on_comboBox_3_currentIndexChanged(int index);
         void on_comboBox_4_currentIndexChanged(int index);
         void on_pushButton_keyGrip_clicked();
         void on_pushButton_save_clicked();
};

#endif // PROGRAMKEYGRIPV2_H
