#include "manangeconfigextended.h"
#include <QCoreApplication>

ManangeConfigExtended::ManangeConfigExtended()
{

}

void ManangeConfigExtended::sProg1Final3_1_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final311_"+pLabel;
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
        << " A A1   \"Stroke P1_fI11\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog1_fI11\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

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
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg1Final3_2_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final321_"+pLabel;
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
        << " A A1   \"Stroke P1_fI21\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

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
         << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

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
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg1Final3_1_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final312_"+pLabel;
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
        << " A A1   \"Stroke P1_fI12\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke1", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"


        << "  P P2 \"Release\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA "<< findXMLNodeValue(root, "Delay_Stroke1", "Delay_Ch2")<<"ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch1")<<"ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke1", "RF_Step_Ch2")<<"ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg1Final3_2_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg1_final322_"+pLabel;
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
        << " A A1   \"Stroke P1_fI22\" "<< findXMLNodeValue(root, "TDur_Stroke1", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

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
         << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
       << "    X X1_2 '< @ArmTilt 5' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg2Final3_1_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final311_"+pLabel;
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
        << " A A1   \"Stroke_Prog2_fI11\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog2_fI11\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

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
         << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg2Final3_2_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final321_"+pLabel;
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
        << " A A1   \"Stroke_Prog2_fI21\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog2_fI21\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

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
         << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg2Final3_1_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final312_"+pLabel;
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
        << " A A1   \"Stroke_Prog2_fI12\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog2_fI12\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg2Final3_2_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg2_final322_"+pLabel;
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
        << " A A1   \"Stroke_Prog2_fI22\" "<< findXMLNodeValue(root, "TDur_Stroke2", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog2_fI22\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"Phase 1: reaching\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg3Final3_1_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final311_"+pLabel;
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
        << " A A1   \"Stroke Prog3_fI11\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog3_fI11\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

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
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg3Final3_2_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final321_"+pLabel;
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
        << " A A1   \"Stroke Prog3_fI21\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog3_fI21\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
         << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

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
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg3Final3_1_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final312_"+pLabel;
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
        << " A A1   \"Stroke Prog3_fI12\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog3_fI12\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg3Final3_2_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg3_final322_"+pLabel;
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
        << " A A1   \"Stroke Prog3_fI22\" "<< findXMLNodeValue(root, "TDur_Stroke3", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog3_fI22\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 5' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg4Final3_1_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final311_"+pLabel;
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
        << " A A1   \"Stroke Prog4_fI11\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog4_fI11\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 3' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"
        << "    X X1_4 '< @ArmTilt 3' GOTO P4"<< "\n\n"
        << "  P P4 \"P4: release\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"
         << "    X X1_5 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"
        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg4Final3_2_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final321_"+pLabel;
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
        << " A A1   \"Stroke Prog4_fI21\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog4_fI21\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 3' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"
        << "    X X1_4 '< @ArmTilt 3' GOTO P4"<< "\n\n"
        << "  P P4 \"P4: release\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"
         << "    X X1_5 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"
        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg4Final3_1_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final312_"+pLabel;
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
        << " A A1   \"Stroke Prog4_fI12\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog4_fI12\" " << '\n'
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
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"


        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P3 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"


        << "  P P3 \"P3: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P4 6000ms P4 STOP P4" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
         << "    X X1_4 '< @ArmTilt 3' GOTO P0"<< "\n\n"
        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg4Final3_2_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg4_final322_"+pLabel;
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
        << " A A1   \"Stroke Prog4_fI22\" "<< findXMLNodeValue(root, "TDur_Stroke4", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog4_fI22\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"

        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"


        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P3 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch4")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch4")<<"us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke4", "RF_Step_Ch5")<<"ns "<< findXMLNodeValue(root, "PW_Stroke4", "PW_Ch5")<<"us RATE"<< "\n\n"


        << "  P P3 \"P3: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P4 6000ms P4 STOP P4" << '\n'
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
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
         << "    X X1_4 '< @ArmTilt 3' GOTO P0"<< "\n\n"
        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg6Final3_1_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final311_"+pLabel;
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
        << " A A1   \"Stroke Prog6_fI11\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog6_fI11\" " << '\n'
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
       << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 3' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P3"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_4 '< @ArmTilt 3' GOTO P4"<< "\n\n"

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

void ManangeConfigExtended::sProg6Final3_2_1(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final321_"+pLabel;
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
        << " A A1   \"Stroke Prog6_fI21\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog6_fI21\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
       << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" 0ms NONE 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_2 '< @ArmTilt 3' GOTO P2"<< "\n\n"

        << "  P P2 \"P2: flexion\" 0ms NONE 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_3 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P3"<< "\n\n"

        << "  P P3 \"P3: extension\" 0ms NONE 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_4 '< @ArmTilt 3' GOTO P4"<< "\n\n"

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

void ManangeConfigExtended::sProg6Final3_1_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final312_"+pLabel;
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
        << " A A1   \"Stroke Prog6_fI12\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog6_fI12\" " << '\n'
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
       << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "elevation_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P3 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P3 \"P3: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P4 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P4 \"P4: rest\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_4 '< @ArmTilt 3' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

void ManangeConfigExtended::sProg6Final3_2_2(QString patientLabel)
{
    QDomDocument document;
    pLabel = patientLabel;

    QString configfilename = "config_sProg6_final322_"+pLabel;
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
        << " A A1   \"Stroke Prog6_fI22\" "<< findXMLNodeValue(root, "TDur_Stroke6", "Duration")<<"min          0           0%           AVAILABLE FALSE FALSE TRUE TRUE" << "\n\n"

        << " F LP1 2 1 -0.877225 0.310992 0.108442 0.216884 0.108442 ` lp Fs=30Hz Fc=3Hz"<<'\n'
        << " F HP1 2 1 -1.41893 0.553217 0.743038 -1.48608 0.743038 ` lp Fs=30Hz Fc=3Hz" << "\n\n"

        << " F NULL1 1 1 0 1 0"<<'\n'
        << " F NULL2 2 1 0 0 1 0 0" << "\n\n"

        << "  B SA1 1 \"Prog6_fI22\" " << '\n'
        << "` These limits apply to all phases" << '\n'
        << "` M ref stim, channel, max current, minPW, maxPW, freq, waveform, output name" << '\n'
        << "   M CH1 1 1 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F1") <<"Hz"<<" ASYM \"Channel 1\" "<< '\n'
        << "   M CH2 1 2 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F2") <<"Hz"<<" ASYM \"Channel 2\" "<< '\n'
        << "   M CH3 1 3 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F3") <<"Hz"<<" ASYM \"Channel 3\" "<< '\n'
        << "   M CH4 1 4 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F4") <<"Hz"<<" ASYM \"Channel 4\" "<< '\n'
        << "   M CH5 1 5 120mA 10us 450us "<< findXMLNodeValue(root, "Frequency", "F5") <<"Hz"<<" ASYM \"Channel 5\" "<< "\n\n"


        << "  S ArmX 1 0 FALSE ACCEL_Y \"arm accel\" 30Hz 30Hz" <<'\n'
        << "  S ArmTilt 1 0 FALSE FLEX_AB \"Upper arm angle\" 30Hz 30Hz"<<"\n\n"

        << "  P P0 \"Neutral\" 0ms NONE 6000ms P1 STOP P1" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
       << "    X X1_1 '> @ArmTilt "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "flexion_angle") <<"' GOTO P1"<< "\n\n"

        << "  P P1 \"P1: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P2 6000ms P2 STOP P2" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P2 \"P2: flexion\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P3 6000ms P3 STOP P3" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch2")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch2")<<"us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch3")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch3")<<"us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P3 \"P3: extension\" "<< findXMLNodeValue(root, "IMU_Stroke_Settings", "release_timeout") <<"ms P4 6000ms P4 STOP P4" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1") <<"mA"<<"  0ms    0ms   "<< findXMLNodeValue(root, "Ramp_Step_Stroke6", "RR_Step_Ch1")<<"ns "<< findXMLNodeValue(root, "PW_Stroke6", "PW_Ch1")<<"us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n\n"


        << "  P P4 \"P4: rest\" 0ms NONE 6000ms P0 STOP P0" << '\n'
        << "`                Delay  RR    rate    PW" << '\n'
        << "    O CH1 "<< findXMLNodeValue(root, "Current", "CH1")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH2 "<< findXMLNodeValue(root, "Current", "CH2")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH3 "<< findXMLNodeValue(root, "Current", "CH3")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH4 "<< findXMLNodeValue(root, "Current", "CH4")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< '\n'
        << "    O CH5 "<< findXMLNodeValue(root, "Current", "CH5")<<"mA"<<"  0ms    0ms   6000ns 0us RATE"<< "\n"
        << "    X X1_4 '< @ArmTilt 3' GOTO P0"<< "\n\n"

        <<"@--------" << '\n' ;

        originalFile.close();
        qDebug() << "Writing finished";
    }
}

QString ManangeConfigExtended::findXMLNodeValue(const QDomElement &root, const QString &parentname, const QString &childname)
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
