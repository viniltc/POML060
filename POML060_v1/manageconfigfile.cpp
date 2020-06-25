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

        << "  P P0 \"Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"Thumb adduct\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P4 \"Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


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
        << "  P P0 \"Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p1_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p1_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p2_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p2_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p2_FDS").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p2_FDS").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P3 \"Thumb adduct\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_FDS").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_FDS").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_Ulna").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_Ulna").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_ADP").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_ADP").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P4 \"Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   "<<root.elementsByTagName("Rmp_p4_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p4_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< "\n\n"


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

        << "  P P0 \"Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P2 \"Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"Thumb opposes\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P4 \"Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


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
        << "  P P0 \"Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p1_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p1_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p2_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p2_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p2_FDS").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p2_FDS").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P3 \"Thumb opposes\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_FDS").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_FDS").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_Ulna").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_Ulna").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms "<<root.elementsByTagName("Rmp_p3_ADP").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p3_ADP").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P4 \"Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   "<<root.elementsByTagName("Rmp_p4_EDC").at(0).firstChild().nodeValue()<<"ns "<< root.elementsByTagName("PW_p4_EDC").at(0).firstChild().nodeValue()<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< "\n\n"


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
        << "  F LP3 2 1 " << root.elementsByTagName("a1").at(0).firstChild().nodeValue()<<" "<< root.elementsByTagName("a2").at(0).firstChild().nodeValue()<<" "<< root.elementsByTagName("b0").at(0).firstChild().nodeValue()<<" "<< root.elementsByTagName("b1").at(0).firstChild().nodeValue()<<" "<< root.elementsByTagName("b2").at(0).firstChild().nodeValue()<<" ` lp Fs=100Hz Fc=3Hz" << "\n\n"

        << "  F NULL1 1 1 0 1 0 "<< '\n'
        << "  F NULL2 2 1 0 0 1 0 0 "<<"\n\n"
        << "  B SA1 1 \"Tests\""<< '\n'
        << "   M Extensors 1 1 20mA 10us 250us 40Hz ASYM \"Wrist extensors\" "<< '\n'
        << "   M Flexors 1 2 20mA 10us 250us 40Hz ASYM \"Wrist flexors\" "<< "\n\n"

        << "   S ShoulderX 1 0 FALSE ACCEL_XS \"Sensor X\" 20Hz 20Hz \\LP3" << '\n'
        << "   S ShoulderY 1 0 FALSE ACCEL_YS \"Sensor Y\" 20Hz 20Hz \\LP3" << '\n'
        << "   S ShoulderZ 1 0 FALSE ACCEL_ZS \"Sensor Z\" 20Hz 20Hz \\LP3" << '\n'
        << "   S ShoulderV 1 0 FALSE ACCEL_V \"Vertical twitch\" 20Hz 20Hz \\LP3" << '\n'
        << "   S ShoulderH 1 0 FALSE ACCEL_H \"Horizontal twitch\" 20Hz 20Hz \\LP3" << '\n'
        << "   S ShoulderP 1 0 FALSE ACCEL_P \"Perp Humerous\" 20Hz 20Hz \\LP3" << "\n\n"

        << "   P P1 \"twitch when ready\" 0ms NONE 2000ms P2 STOP P2" << '\n'
        << "`     Ref        I     D   R   R   PW" << '\n'
        << "    O Extensors 20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    O Flexors   20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    X X1_1 '> @ShoulderY 2000' GOTO P2"<< '\n'
        << "    X X1_2 '> @ShoulderP 2000' GOTO P3"<< "\n\n"

        << "   P P2 \"Elev/dep. twitch!\" 1000ms P1 500ms P1 STOP P1" << '\n'
        << "    O Extensors 20mA  0ms 0ms 18000ns 180us RATE"<< '\n'
        << "    O Flexors   20mA  0ms 0ms 18000ns 0us RATE"<< "\n\n"

        << "   P P3 \"Pro/retract twitch!\" 1000ms P1 500ms P1 STOP P1" << '\n'
        << "    O Extensors 20mA  0ms 0ms 18000ns 0us RATE"<< '\n'
        << "    O Flexors   20mA  0ms 0ms 18000ns 180us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

QString ManageConfigFile::findNodeValue(QDomElement const& root, QString const& parentname, QString const& childname)
{
    QDomElement element = root.firstChildElement(parentname);
    return element.firstChildElement(childname).firstChild().nodeValue();
}
