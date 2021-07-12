#include "manageconfigfile.h"

#include <QCoreApplication>

ManageConfigFile::ManageConfigFile()
{


}

void ManageConfigFile::keyGripTest(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

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



    QFile originalFile(txtWritePath);
    if(originalFile.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream stream_1(& originalFile);




        stream_1 << "`R ref, version, \"config_ID\",                   menu language, Power timeout (hours), Number of users" <<'\n'
            << "R  R1   1  \"Template for setting parameters\"  ENGLISH        1                      1 " << "\n\n"

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "U U1    \"Any user\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Setup stim levels\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "  M CH1 1 1 120mA 10us 450us 40Hz ASYM \"EDC\" "<< '\n'
        << "  M CH2 1 2 120mA 10us 450us 40Hz ASYM \"FDS+FDP\" "<< '\n'
        << "  M CH3 1 3 120mA 10us 450us 40Hz ASYM \"Ulna nerve\" "<< '\n'
        << "  M CH4 1 4 120mA 10us 450us 40Hz ASYM \"ADP\" "<< '\n'
        << "  M CH5 1 5 120mA 10us 450us 40Hz ASYM \"EPL\" "<< "\n\n"

        << "  P P0 \"KeyGrip:Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"KeyGrip:Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"KeyGrip:Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"KeyGrip:Thumb adduct\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P4 \"KeyGrip:Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::keyGripFinal(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_keygrip_"+pLabel;
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



    QFile originalFile(txtWritePath);
    if(originalFile.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream stream_1(& originalFile);




        stream_1 << "`R ref, version, \"config_ID\",                   menu language, Power timeout (hours), Number of users" <<'\n'
            << "R  R1   1  \"Template for setting parameters\"  ENGLISH        1                      1 " << "\n\n"

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "U U1    \"Any user\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Setup stim levels\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "  M CH1 1 1 120mA 10us 450us 40Hz ASYM \"EDC\" "<< '\n'
        << "  M CH2 1 2 120mA 10us 450us 40Hz ASYM \"FDS+FDP\" "<< '\n'
        << "  M CH3 1 3 120mA 10us 450us 40Hz ASYM \"Ulna nerve\" "<< '\n'
        << "  M CH4 1 4 120mA 10us 450us 40Hz ASYM \"ADP\" "<< '\n'
        << "  M CH5 1 5 120mA 10us 450us 40Hz ASYM \"EPL\" "<< "\n\n"
        << "  P P0 \"KeyGrip:Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P1 \"KeyGrip:Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<< findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p1_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p1_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P2 \"KeyGrip:Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p2_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p2_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p2_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p2_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P3 \"KeyGrip:Thumb adduct\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P4 \"KeyGrip:Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p4_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p4_EDC")<<"us "<<" RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< "\n\n"


        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::palmerGraspTest(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_palmergrasp_test_"+pLabel;
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



    QFile originalFile(txtWritePath);
    if(originalFile.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream stream_1(& originalFile);




        stream_1 << "`R ref, version, \"config_ID\",                   menu language, Power timeout (hours), Number of users" <<'\n'
            << "R  R1   1  \"Template for setting parameters\"  ENGLISH        1                      1 " << "\n\n"

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "U U1    \"Any user\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Setup stim levels\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "  M CH1 1 1 120mA 10us 450us 40Hz ASYM \"EDC\" "<< '\n'
        << "  M CH2 1 2 120mA 10us 450us 40Hz ASYM \"FDS+FDP\" "<< '\n'
        << "  M CH3 1 3 120mA 10us 450us 40Hz ASYM \"Ulna nerve\" "<< '\n'
        << "  M CH4 1 4 120mA 10us 450us 40Hz ASYM \"ADP\" "<< '\n'
        << "  M CH5 1 5 120mA 10us 450us 40Hz ASYM \"EPL\" "<< "\n\n"

        << "  P P0 \"PalmerGrasp:Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"PalmerGrasp:Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P2 \"PalmerGrasp:Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P3 \"PalmerGrasp:Thumb opposes\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n' //180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P4 \"PalmerGrasp:Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }

}

void ManageConfigFile::palmerGraspFinal(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_palmergrasp_"+pLabel;
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



    QFile originalFile(txtWritePath);
    if(originalFile.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream stream_1(& originalFile);




        stream_1 << "`R ref, version, \"config_ID\",                   menu language, Power timeout (hours), Number of users" <<'\n'
            << "R  R1   1  \"Template for setting parameters\"  ENGLISH        1                      1 " << "\n\n"

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "U U1    \"Any user\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Setup stim levels\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "  M CH1 1 1 120mA 10us 450us 40Hz ASYM \"EDC\" "<< '\n'
        << "  M CH2 1 2 120mA 10us 450us 40Hz ASYM \"FDS+FDP\" "<< '\n'
        << "  M CH3 1 3 120mA 10us 450us 40Hz ASYM \"Ulna nerve\" "<< '\n'
        << "  M CH4 1 4 120mA 10us 450us 40Hz ASYM \"ADP\" "<< '\n'
        << "  M CH5 1 5 120mA 10us 450us 40Hz ASYM \"EPL\" "<< "\n\n"
        << "  P P0 \"PalmerGrasp:Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P1 \"PalmerGrasp:Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p1_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p1_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p1_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p1_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P2 \"PalmerGrasp:Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P3 \"PalmerGrasp:Thumb opposes\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p4_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P4 \"PalmerGrasp:Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p4_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p4_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< "\n\n"


        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::filterTest(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_filterSettiings_"+pLabel;
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



    QFile originalFile(txtWritePath);
    if(originalFile.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream stream_1(& originalFile);


        stream_1 <<"R  R1   1  \"Testing twitch sensor\" ENGLISH  1 1 " << "\n"

        << "U U1    \"Any user\"   ENGLISH   100%   1" << "\n"

        << " A A1   \"Twitch Freq tests\" 0min  0  0%  AVAILABLE FALSE FALSE TRUE TRUE" << "\n"
        << "  F LP1 2 1 " << findXMLNodeValue(root, "Filter_Coeffs", "a1l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "a2l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b0l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b1l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b2l")<<" ` lp Fs=30Hz Fc=3Hz" << '\n' //a1 a2 bo b1 b2
        << "  F HP1 2 1 " << findXMLNodeValue(root, "Filter_Coeffs", "a1h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "a2h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b0h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b1h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b2h")<<" ` lp Fs=30Hz Fc=3Hz" << "\n\n" //a1 a2 bo b1 b2
        << "  F NULL1 1 1 0 1 0 "<< '\n'
        << "  F NULL2 2 1 0 0 1 0 0 "<<"\n\n"
        << "  B SA1 1 \"Tests\""<< '\n'
        << "   M Extensors 1 1 20mA 10us 250us 40Hz ASYM \"Wrist extensors\" "<< '\n'
        << "   M Flexors 1 2 20mA 10us 250us 40Hz ASYM \"Wrist flexors\" "<< "\n\n"

        << "   S ShoulderX 1 0 FALSE ACCEL_XS \"Sensor X\" 30Hz 30Hz \\LP1 \\HP1" << '\n'
        << "   S ShoulderY 1 0 FALSE ACCEL_YS \"Sensor Y\" 30Hz 30Hz \\LP1 \\HP1" << '\n'
        << "   S ShoulderZ 1 0 FALSE ACCEL_ZS \"Sensor Z\" 30Hz 30Hz \\LP1 \\HP1" << '\n'
        << "   S ShoulderV 1 0 FALSE ACCEL_V \"Vertical twitch\" 30Hz 30Hz \\LP1 \\HP1" << '\n'
        << "   S ShoulderH 1 0 FALSE ACCEL_H \"Horizontal twitch\" 30Hz 30Hz \\LP1 \\HP1" << '\n'
        << "   S ShoulderP 1 0 FALSE ACCEL_P \"Perp Humerous\" 30Hz 30Hz \\LP1 \\HP1" << "\n\n"

        << "   P P1 \"twitch when ready\" 0ms NONE 2000ms P2 STOP P2" << '\n'
        << "`     Ref        I     D   R   R   PW" << '\n'
        << "    O Extensors 20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    O Flexors   20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    X X1_1 '> @ShoulderY 2000' GOTO P2"<< '\n'
        << "    X X1_2 '> @ShoulderP 2000' GOTO P3"<< "\n\n"

        << "   P P2 \"Elev/dep. twitch!\" 1000ms P1 500ms P1 STOP P1" << '\n'
        << "    O Extensors 20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    O Flexors   20mA  0ms 0ms 18000ns 0us RATE"<< "\n\n"

        << "   P P3 \"Pro/retract twitch!\" 1000ms P1 500ms P1 STOP P1" << '\n'
        << "    O Extensors 20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    O Flexors   20mA  0ms 0ms 18000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::TetraGripFinal(QString patientLabel)
{

    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_tetraGrip_"+pLabel;
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



    QFile originalFile(txtWritePath);
    if(originalFile.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream stream_1(& originalFile);




        stream_1 << "`R ref, version, \"config_ID\",                   menu language, Power timeout (hours), Number of users" <<'\n'
            << "R  R1   1  \"Template for setting parameters\"  ENGLISH        1                      1 " << "\n\n"

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "U U1    \"Any user\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Setup stim levels\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE 0 0 0 FALSE 2000ms" << "\n\n"

        <<  "`   Ref def min max NV" << '\n'
        <<   "V t   0   0   X FALSE ` time of last twitch" << '\n'
        <<   "V c   0   0   X FALSE ` number of twitches"<< '\n'
        <<   "V x1 "<<findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_ADP")<<" 0   360 TRUE ` PW adjust for KeyGrip"<< '\n'
        <<   "V x2 "<<findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_APB")<<" 0   360 TRUE ` PW adjust for PalmrGrasp"<< '\n'
        <<   "V stepsize 10 10 50 FALSE ` stepsize"<< "\n\n"

        << "  F LP1 2 1 " << findXMLNodeValue(root, "Filter_Coeffs", "a1l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "a2l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b0l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b1l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b2l")<<" ` lp Fs=30Hz Fc=3Hz" << '\n' //a1 a2 bo b1 b2
        << "  F HP1 2 1 " << findXMLNodeValue(root, "Filter_Coeffs", "a1h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "a2h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b0h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b1h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b2h")<<" ` lp Fs=30Hz Fc=3Hz" << "\n\n" //a1 a2 bo b1 b2

        << "  B SA1 1 \"TetraGrip\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "  M C1 1 1 120mA 10us 450us 40Hz ASYM \"EDC\" "<< '\n'
        << "  M C2 1 2 120mA 10us 450us 40Hz ASYM \"FDS\" "<< '\n'
        << "  M C3 1 3 120mA 10us 450us 40Hz ASYM \"Ulna\" "<< '\n'
        << "  M C4 1 4 120mA 10us 450us 40Hz ASYM \"ADP\" "<< '\n'
        << "  M C5 1 5 120mA 10us 450us 40Hz ASYM \"Opp\" "<< "\n\n"

        << "  S SX 1 0 FALSE ACCEL_XS \"Sensor X\" 30Hz 30Hz \\LP1 \\HP1" <<'\n'
        << "  S SV 1 0 FALSE ACCEL_V \"Ver twitch\" 30Hz 30Hz \\LP1 \\HP1" << "\n\n"

        << "  P P0 \"KeyGrip:Test phase\" 0ms NONE 2000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' GOTO P5"<<'\n'
        << "    X X1_2 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P1"<<"\n\n"

        << "  P P1 \"KeyGrip:Hand open\" 2000ms P2 2000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<< findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p1_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p1_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P2 \"KeyGrip:Finger flexion\" 2000ms P3 2000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p2_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p2_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p2_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p2_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P3 \"KeyGrip:Thumb adduct\" 0ms NONE 2000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<<'\n'
        << "  `  double twitch to the KeyGrip intensity adjust phase P9"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 3" << '\n'
        << "    X X1_2 '= :c 2' GOTO P9" << '\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P4"<<"\n\n"

        << "  P P4 \"KeyGrip:Hand open\" 0ms NONE 2000ms P15 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p4_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p4_EDC")<<"us "<<" RATE"<< '\n'
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "  `  double twitch to turn off single twitch to grip phase"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 1" << '\n'
        << "    X X1_2 '= :c 2' GOTO P0" << '\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P3"<<"\n\n"

        << "  P P5 \"PalmarGrasp:Hand open\" 2000ms P6 2000ms P6 STOP P6" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p1_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p1_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p1_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p1_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P6 \"PalmarGrasp:Finger flexion\" 2000ms P7 2000ms P7 STOP P7" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P7 \"PalmarGrasp:Thumb opposes\" 0ms NONE 2000ms P8 STOP P8" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<<"x2"<<" RATE"<< "\n\n"
        << "  `  double twitch to the palmrGrsp intensity adjust phase P10"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 3" << '\n'
        << "    X X1_2 '= :c 2' GOTO P10"<<'\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P8"<<"\n\n"

        << "  P P8 \"PalmarGrasp:Hand open\" 0ms NONE 2000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p4_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p4_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "  `  double twitch to turn off single twitch to grip phase"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 1" << '\n'
        << "    X X1_2 '= :c 2' GOTO P0" << '\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P7"<<"\n\n"

        << "  P P9 \"KeyGrip:Adj grip\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "  ` protraction twitch to increment C4 PW in steps of 10us"<<'\n'
        << "  ` retraction twitch to decrement C4 PW in steps of 10us"<<'\n'
        << "  ` single shoulder twitch to back to P3"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SX "<<findXMLNodeValue(root, "Sensor_Settings", "protraction_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x1 + :x1 :stepsize 1' GOTO P11"<<'\n'
        << "    X X1_2 'AND AND AND < @SX "<<findXMLNodeValue(root, "Sensor_Settings", "retraction_treshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x1 - :x1 :stepsize 1' GOTO P12"<<'\n'
        << "    X X1_3 'AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 ' GOTO P3"<<"\n\n"

        << "  P P10 \"PalmarGrasp:Adj grip\" 0ms NONE 2000ms STOP STOP P8" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<< "x2"<<" RATE"<< '\n'
        << "  ` protraction twitch to increment C5 PW in steps of 10us" <<'\n'
        << "  ` retraction twitch to decrement C5 PW in steps of 10us" <<'\n'
        << "  ` single shoulder twitch to back to P7" <<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t 0 0' GOTO NONE" <<'\n'
        << "    X X1_1 'AND AND AND > @SX "<<findXMLNodeValue(root, "Sensor_Settings", "protraction_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x2 + :x2 :stepsize 1' GOTO P13" <<'\n'
        << "    X X1_2 'AND AND AND < @SX "<<findXMLNodeValue(root, "Sensor_Settings", "retraction_treshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x2 - :x2 :stepsize 1' GOTO P14" <<'\n'
        << "    X X1_3 'AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 ' GOTO P7" <<"\n\n"

        << "  P P11 \"KeyGrip:inc\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 2000Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P9" << "\n\n"

        << "  P P12 \"KeyGrip:dec\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 500Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P9" << "\n\n"

        << "  P P13 \"PalmarGrsp:inc\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<< "x2"<<" RATE"<< '\n'
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 2000Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P10" << "\n\n"

        << "  P P14 \"PalmarGrsp:dec\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<< "x2"<<" RATE"<< "\n\n"
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 500Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P10" << "\n\n"

        << "  P P15 \"Exercise:Rest\" 0ms NONE 2000ms P5 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O C1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O C5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }

}

QString ManageConfigFile::findXMLNodeValue(QDomElement const& root, QString const& parentname, QString const& childname)
{
    QDomElement element = root.firstChildElement(parentname);
    return element.firstChildElement(childname).firstChild().nodeValue();
}
