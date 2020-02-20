#include "manageconfigfile.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QtXml/QDomDocument>
#include <QCoreApplication>

ManageConfigFile::ManageConfigFile()
{


}

void ManageConfigFile::keyGripTest(QString patientLabel)
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


        stream_1 << "`R ref, version, \"config_ID\", menu language, Power timeout (hours), Number of users" <<'\n'
            << " R  R1   1  \"Template for setting parameters\"  ENGLISH  1  1 " << '\n'

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "  U U1    \"Any user\"   ENGLISH   100%      1" << '\n'

        << "  `A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << "    A A1   \"Setup stim levels\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << '\n'

        << "   B SA1 1 \"Engine tests\" " << '\n'
        << "  ` These limits apply to all phases" << '\n'
        << "  ` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "    M CH1 1 1 120mA 10us 450us 40Hz ASYM \"EDC\" "<< '\n'
        << "    M CH2 1 2 120mA 10us 450us 40Hz ASYM \"FDS+FDP\" "<< '\n'
        << "    M CH3 1 3 120mA 10us 450us 40Hz ASYM \"Ulna nerve\" "<< '\n'
        << "    M CH4 1 4 120mA 10us 450us 40Hz ASYM \"ADP\" "<< '\n'
        << "    M CH5 1 5 120mA 10us 450us 40Hz ASYM \"EPL\" "<< '\n'
        << "  P P0 \"Test phase\" 0ms NONE 2000ms STOP STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'

        << "  P P1 \"Hand open\" 0ms NONE 2000ms STOP STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'

        << "  P P2 \"Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'

        << "  P P3 \"Thumb adduct\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'

        << "  P P4 \"Hand open\" 0ms NONE 2000ms STOP STOP STOP" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< root.elementsByTagName("CH1").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 180us RATE"<< '\n'
        << "    O CH2 "<< root.elementsByTagName("CH2").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< root.elementsByTagName("CH3").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< root.elementsByTagName("CH4").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< root.elementsByTagName("CH5").at(0).firstChild().nodeValue()<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'


        <<"@--------" ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}
