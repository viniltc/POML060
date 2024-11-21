#ifndef MANANGECONFIGEXTENDED_H
#define MANANGECONFIGEXTENDED_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QtXml/QDomDocument>

class ManangeConfigExtended
{
public:
    ManangeConfigExtended();
    QString pLabel;
    void sProg1Final3_1_1(QString patientLabel);
    void sProg1Final3_2_1(QString patientLabel);
    void sProg1Final3_1_2(QString patientLabel);
    void sProg1Final3_2_2(QString patientLabel);

    void sProg2Final3_1_1(QString patientLabel);
    void sProg2Final3_2_1(QString patientLabel);
    void sProg2Final3_1_2(QString patientLabel);
    void sProg2Final3_2_2(QString patientLabel);

    void sProg3Final3_1_1(QString patientLabel);
    void sProg3Final3_2_1(QString patientLabel);
    void sProg3Final3_1_2(QString patientLabel);
    void sProg3Final3_2_2(QString patientLabel);

    void sProg4Final3_1_1(QString patientLabel);
    void sProg4Final3_2_1(QString patientLabel);
    void sProg4Final3_1_2(QString patientLabel);
    void sProg4Final3_2_2(QString patientLabel);

    void sProg6Final3_1_1(QString patientLabel);
    void sProg6Final3_2_1(QString patientLabel);
    void sProg6Final3_1_2(QString patientLabel);
    void sProg6Final3_2_2(QString patientLabel);


private:

    QString findXMLNodeValue(QDomElement const& root, QString const& parentname, QString const& childname);
};

#endif // MANANGECONFIGEXTENDED_H
