#include "manageconfigfile.h"

#include <QCoreApplication>

ManageConfigFile::ManageConfigFile()
{


}

void ManageConfigFile::currentTest(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_current_test_"+pLabel;
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
        << "`  M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz "<<findXMLNodeValue(root, "Waveform", "W1")<<" \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz "<<findXMLNodeValue(root, "Waveform", "W2")<<" \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz "<<findXMLNodeValue(root, "Waveform", "W3")<<" \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz "<<findXMLNodeValue(root, "Waveform", "W4")<<" \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz "<<findXMLNodeValue(root, "Waveform", "W5")<<" \"Channel 5\" "<< "\n\n"

        << "  P P0 \"Current Test phase\" 0ms NONE 2000ms STOP STOP STOP "<< '\n'
        << "`                Delay  RR    rate    PW     "<< '\n'
        << "   O CH1 0mA  0ms    0ms   4500ns 180us RATE "<< '\n'
        << "   O CH2 0mA  0ms    0ms   4500ns 180us RATE "<< '\n'
        << "   O CH3 0mA  0ms    0ms   4500ns 180us RATE "<< '\n'
        << "   O CH4 0mA  0ms    0ms   4500ns 180us RATE "<< '\n'
        << "   O CH5 0mA  0ms    0ms   4500ns 180us RATE "<<  "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";


 }
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Key Grip Test\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


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
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 57us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"KeyGrip:Thumb adduct\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 57us RATE"<< '\n'
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Key Grip Final\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Palmar Grasp Test\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 92us RATE"<< "\n\n"

        << "  P P2 \"PalmerGrasp:Finger flexion\" 0ms NONE 2000ms STOP STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 57us RATE"<< '\n' //180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 92us RATE"<< "\n\n"

        << "  P P3 \"PalmerGrasp:Thumb opposes\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 57us RATE"<< '\n' //180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n' //180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n' //180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 92us RATE"<< "\n\n"

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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Palmar Grasp Final\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Engine tests\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

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
            << "R  R1   1  \""<<root.firstChildElement("Date").firstChild().nodeValue()<<"\"  ENGLISH        1                      1 " << "\n\n"

        << "`U ref, \"user name\", language, volume, number of activities"<< '\n'
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"TetraGrip"<<root.firstChildElement("Date").firstChild().nodeValue()<<"\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE 0 0 0 FALSE 2000ms" << "\n\n"

        <<  "`   Ref def min max NV" << '\n'
        <<   "V t   0   0   X FALSE ` time of last twitch" << '\n'
        <<   "V c   0   0   X FALSE ` no of twitches"<< '\n'
        <<   "V x1 "<<findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_ADP")<<" 0   360 TRUE ` PW adjust for KeyGrip"<< '\n'
        <<   "V x2 "<<findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_APB")<<" 0   360 TRUE ` PW adjust for PalmrGrasp"<< '\n'
        <<   "V stepsize 20 10 50 FALSE ` stepsize"<< "\n\n"

        << "  F LP1 2 1 " << findXMLNodeValue(root, "Filter_Coeffs", "a1l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "a2l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b0l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b1l")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b2l")<<" ` lp Fs=30Hz Fc=3Hz" << '\n' //a1 a2 bo b1 b2
        << "  F HP1 2 1 " << findXMLNodeValue(root, "Filter_Coeffs", "a1h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "a2h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b0h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b1h")<<" "<< findXMLNodeValue(root, "Filter_Coeffs", "b2h")<<" ` lp Fs=30Hz Fc=3Hz" << "\n\n" //a1 a2 bo b1 b2

        << "  B SA1 1 \"TetraGrip\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "  M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "  M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "  M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "  M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "  M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S SX 1 0 FALSE ACCEL_XS \"Shoulder Sensor\" 30Hz 30Hz \\LP1 \\HP1" <<'\n'
        << "  S SV 1 0 FALSE ACCEL_V \"Ver twitch\" 30Hz 30Hz \\LP1 \\HP1" << "\n\n"

        << "  P P0 \"Test phase\" 0ms NONE 2000ms P1 STOP P1 0.2s 80ms 100Hz 0`" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' GOTO P5"<<'\n'
        << "    X X1_2 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P1"<<"\n\n"

        << "  P P1 \"KeyGrip:Hand open\" 2000ms P2 2000ms P2 STOP P2 0.2s 80ms 600Hz 1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<< findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p1_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p1_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P2 \"KeyGrip:Finger flexion\" 2000ms P3 2000ms P3 STOP P3 0.2s 80ms 600Hz 1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p2_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p2_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p2_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p2_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        << "  P P3 \"KeyGrip:Thumb adduct\" 0ms NONE 10000ms P4 STOP P4 0.2s 80ms 600Hz 3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<<'\n'
        << "  `  double twitch to the KeyGrip intensity adjust phase P9"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 3" << '\n'
        << "    X X1_2 '= :c 2' GOTO P9" << '\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P4"<<"\n\n"

        << "  P P4 \"KeyGrip:Hand open\" 0ms NONE 2000ms P15 STOP P0 0.2s 80ms 600Hz 1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p4_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p4_EDC")<<"us "<<" RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "  `  double twitch to turn off single twitch to grip phase"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 1" << '\n'
        << "    X X1_2 '= :c 2' GOTO P0" << '\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P3"<<"\n\n"

        << "  P P5 \"PalmarGrasp:Hand open\" 2000ms P6 2000ms P6 STOP P6 0.2s 80ms 200Hz 1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p1_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p1_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p1_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p1_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P6 \"PalmarGrasp:Finger flexion\" 2000ms P7 2000ms P7 STOP P7 0.2s 80ms 200Hz 1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p2_APB")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p2_APB")<<"us "<<" RATE"<< "\n\n"

        << "  P P7 \"PalmarGrasp:Thumb opposes\" 0ms NONE 10000ms P8 STOP P8 0.2s 80ms 200Hz 3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<<"x2"<<" RATE"<< "\n\n"
        << "  `  double twitch to the palmrGrsp intensity adjust phase P10"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 3" << '\n'
        << "    X X1_2 '= :c 2' GOTO P10"<<'\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P8"<<"\n\n"

        << "  P P8 \"PalmarGrasp:Hand open\" 0ms NONE 2000ms P0 STOP P0 0.2s 80ms 200Hz 1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p4_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p4_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   600000ns 0us RATE"<< '\n'
        << "  `  double twitch to turn off single twitch to grip phase"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t ;c 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;c + :c 1 1' SOUND 0.1s 50ms 1000Hz 1" << '\n'
        << "    X X1_2 '= :c 2' GOTO P0" << '\n'
        << "    X X1_3 'AND = :c 1 > PHASE_MS + :t 2000' GOTO P7"<<"\n\n"

        << "  P P9 \"KeyGrip:Adj grip\" 0ms NONE 2000ms STOP STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "  ` protraction twitch to increment C4 PW in steps of 10us"<<'\n'
        << "  ` retraction twitch to decrement C4 PW in steps of 10us"<<'\n'
        << "  ` single shoulder twitch to back to P3"<<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t 0 0' GOTO NONE"<<'\n'
        << "    X X1_1 'AND AND AND > @SX "<<findXMLNodeValue(root, "Sensor_Settings", "protraction_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x1 + :x1 :stepsize 1' GOTO P11"<<'\n'
        << "    X X1_2 'AND AND AND < @SX "<<findXMLNodeValue(root, "Sensor_Settings", "retraction_treshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x1 - :x1 :stepsize 1' GOTO P12"<<'\n'
        << "    X X1_3 'AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 ' GOTO P3"<<"\n\n"

        << "  P P10 \"PalmarGrasp:Adj grip\" 0ms NONE 2000ms STOP STOP P8" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<< "x2"<<" RATE"<< '\n'
        << "  ` protraction twitch to increment C5 PW in steps of 10us" <<'\n'
        << "  ` retraction twitch to decrement C5 PW in steps of 10us" <<'\n'
        << "  ` single shoulder twitch to back to P7" <<'\n'
        << "    X X1_0 'AND < PHASE_MS 50 = ;t 0 0' GOTO NONE" <<'\n'
        << "    X X1_1 'AND AND AND > @SX "<<findXMLNodeValue(root, "Sensor_Settings", "protraction_threshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x2 + :x2 :stepsize 1' GOTO P13" <<'\n'
        << "    X X1_2 'AND AND AND < @SX "<<findXMLNodeValue(root, "Sensor_Settings", "retraction_treshold")<<" > - PHASE_MS :t 1000 > ;t PHASE_MS 0 > ;x2 - :x2 :stepsize 1' GOTO P14" <<'\n'
        << "    X X1_3 'AND > @SV "<<findXMLNodeValue(root, "Sensor_Settings", "vertical_threshold")<<" > - PHASE_MS :t 1000 ' GOTO P7" <<"\n\n"

        << "  P P11 \"KeyGrip:inc\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 2000Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P9" << "\n\n"

        << "  P P12 \"KeyGrip:dec\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_KeyGrip", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_KeyGrip", "Rmp_p3_ADP")<<"ns "<< "x1"<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 500Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P9" << "\n\n"

        << "  P P13 \"PalmarGrsp:inc\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<< "x2"<<" RATE"<< '\n'
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 2000Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P10" << "\n\n"

        << "  P P14 \"PalmarGrsp:dec\" 0ms NONE 2000ms STOP STOP NONE" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_EDC")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_EDC")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_FDS")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_FDS")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_Ulna")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_Ulna")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_ADP")<<"ns "<< findXMLNodeValue(root, "PW_PalmerGrasp", "PW_p3_ADP")<<"us "<<" RATE"<< '\n' // 180
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms "<<findXMLNodeValue(root, "Ramp_PalmerGrasp", "Rmp_p3_APB")<<"ns "<< "x2"<<" RATE"<< "\n\n"
        << "    X X11_1 '> PHASE_MS 500' SOUND 1s 150ms 500Hz 1" << '\n'
        << "    X X11_2 '> PHASE_MS 500' GOTO P10" << "\n\n"

        << "  P P15 \"Exercise:Rest\" 0ms NONE 2000ms P5 STOP P1 0.2s 80ms 400Hz 2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<" 0ms 0ms 600000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }

}

void ManageConfigFile::sProg1Test(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_test_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog1_t\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog1_t\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  4000ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  4000ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg2Test(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_test_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog2_t\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog2_t\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  4000ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  4000ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  4000ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg3Test(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_test_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog3_t\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog3_t\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg4Test(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_test_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog4_t\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog4_t\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< "\n\n"

        << "  P P4 \"P4: release\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg5Test(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg5_test_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog5_t\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog5_t\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: \" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: \" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  1000ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"P3: \" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  1000ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg1Final(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog1_f\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog1_f\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg2Final(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke_Prog2_f\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog2_f\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch3")<<"ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg3Final(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog3_f\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog3_f\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg4Final(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog4_f\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog4_f\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"

        << "  P P4 \"P4: release\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg4Final2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final2_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog4_ft\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog4_ft\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms P1 "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms  P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms P2 "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms  P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms P3 "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms  P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"

        << "  P P3 \"P3: extension\" "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms P4 "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms  P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"

        << "  P P4 \"P4: release\" "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms P1 "<< findXMLNodeValue(root, "TDur_Stroke4", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg4Final3(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final3_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog4_fI\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog4_fI\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE ELEVATION \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt 40' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt 40' GOTO P3"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"
        << "    X X1_4 '< @ArmTilt 5' GOTO P4"<< "\n\n"
        << "  P P4 \"P4: release\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"
        << "    X X1_5 '< @ArmTilt 40' GOTO P0"<< "\n\n"
        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg5Final(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg5_final_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog5_f\" "<< findXMLNodeValue(root, "TDur_Stroke5", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog5_f\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: \" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA 0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Phase 2: \" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke5", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"P3: \" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke5", "Delay_Ch2")<<"ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg1Final2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final2_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog1_ft\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog1_ft\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 1000ms P1 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" "<< findXMLNodeValue(root, "TDur_Stroke1", "OnTime1")<<"ms P2 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Release\" "<< findXMLNodeValue(root, "TDur_Stroke1", "OnTime1")<<"ms P0 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg1Final3(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final3_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog1_fI\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog1_fI\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

         << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
         << "  S ArmTilt 1 0 FALSE ELEVATION \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt 40' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P2"<< "\n\n"

        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt 40' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg2Final2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final2_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke_Prog2_ft\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog2_ft\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 1000ms P1 "<< findXMLNodeValue(root, "TDur_Stroke2", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" "<< findXMLNodeValue(root, "TDur_Stroke2", "OnTime1")<<"ms P2 "<< findXMLNodeValue(root, "TDur_Stroke2", "OnTime1")<<"ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Release\" "<< findXMLNodeValue(root, "TDur_Stroke2", "OnTime1")<<"ms P0 "<< findXMLNodeValue(root, "TDur_Stroke2", "OnTime1")<<"ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch3")<<"ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg2Final3(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final3_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke_Prog2_fI\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog2_fI\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE ELEVATION \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt 40' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke2", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P2"<< "\n\n"

        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke2", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke2", "RF_Step_Ch3")<<"ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"
        << "    X X1_3 '> @ArmTilt 40' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg3Final2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final2_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog3_ft\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog3_ft\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 1000ms P1 "<< findXMLNodeValue(root, "TDur_Stroke3", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "TDur_Stroke3", "OnTime1")<<"ms P2 "<< findXMLNodeValue(root, "TDur_Stroke3", "OnTime1")<<"ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "TDur_Stroke3", "OnTime1")<<"ms P1 "<< findXMLNodeValue(root, "TDur_Stroke3", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg3Final3(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final3_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog3_fI\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog3_fI\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE ELEVATION \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt 40' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke3", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke3", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt 40' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg5Final2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg5_final2_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog5_ft\" "<< findXMLNodeValue(root, "TDur_Stroke5", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog5_ft\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 1000ms P1 "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"Phase 1: \" "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P2 "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA 0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"Phase 2: \" "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P3 "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke5", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"P3: \" "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P2 "<< findXMLNodeValue(root, "TDur_Stroke5", "OnTime1")<<"ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke5", "Delay_Ch2")<<"ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg5Final3(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg5_final3_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog5_fI\" "<< findXMLNodeValue(root, "TDur_Stroke5", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog5_fI\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE ELEVATION \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt 40' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: \" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA 0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<" 0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P2"<< "\n\n"

        << "  P P2 \"Phase 2: \" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke5", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt 40' GOTO P3"<< "\n\n"

        << "  P P3 \"P3: \" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke5", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke5", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke5", "Delay_Ch2")<<"ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_4 '< @ArmTilt 5' GOTO P2"<< "\n\n"
        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg6Test(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_test_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog6_t\" 0min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog6_t\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 180us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P4 \"P4: rest\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg6Final(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog6_f\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog6_f\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P4 \"P4: rest\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg6Final2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final2_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog6_ft\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << "  B SA1 1 \"Prog6_ft\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  P P0 \"Neutral\" 1000ms P1 "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P2 "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P3 "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P3 \"P3: extension\" "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P4 "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"

        << "  P P4 \"P4: rest\" "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P1 "<< findXMLNodeValue(root, "TDur_Stroke6", "OnTime1")<<"ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManageConfigFile::sProg6Final3(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final3_"+pLabel;
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
        << "U U1    \""<< root.firstChildElement("Name").firstChild().nodeValue()<<"\"   ENGLISH   100%      1" << "\n\n"

        << "`A ref, \"activity name\",    max duration, max cycles, startingPW%, available/hidden" << '\n'
        << " A A1   \"Stroke Prog6_fI\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog6_fI\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE ELEVATION \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt 40' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt 40' GOTO P3"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_4 '< @ArmTilt 5' GOTO P4"<< "\n\n"

        << "  P P4 \"P4: rest\" 1000ms P0 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"


        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

QString ManageConfigFile::findXMLNodeValue(QDomElement const& root, QString const& parentname, QString const& childname)
{
    if(parentname == "Waveform")
    {
        QDomElement element = root.firstChildElement(parentname);
        if(element.firstChildElement(childname).firstChild().nodeValue()=="0")
        {
            return "ASYM";
        }
        else
        {
            return "SYM";
        }
    }
    else
    {

        QDomElement element = root.firstChildElement(parentname);
        return element.firstChildElement(childname).firstChild().nodeValue();
    }
}
