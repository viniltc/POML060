#ifndef MANAGECONFIGFILE_H
#define MANAGECONFIGFILE_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QtXml/QDomDocument>

class ManageConfigFile
{
public:
    ManageConfigFile();
    QString pLabel;
    void currentTest(QString patientLabel);
    void keyGripTest(QString patientLabel);
    void keyGripFinal(QString patientLabel);
    void palmerGraspTest(QString patientLabel);
    void palmerGraspFinal(QString patientLabel);
    void filterTest(QString patientLabel);
    void TetraGripFinal(QString patientLabel);
    void sProg1Test(QString patientLabel);
    void sProg2Test(QString patientLabel);
    void sProg3Test(QString patientLabel);
    void sProg4Test(QString patientLabel);
    void sProg5Test(QString patientLabel);
    void sProg1Final(QString patientLabel);
    void sProg2Final(QString patientLabel);
    void sProg3Final(QString patientLabel);
    void sProg4Final(QString patientLabel);
    void sProg4Final2(QString patientLabel);
    void sProg4Final3(QString patientLabel);
    void sProg5Final(QString patientLabel);
    void sProg1Final2(QString patientLabel);
    void sProg1Final3(QString patientLabel);
    void sProg2Final2(QString patientLabel);
    void sProg2Final3(QString patientLabel);
    void sProg3Final2(QString patientLabel);
    void sProg3Final3(QString patientLabel);
    void sProg5Final2(QString patientLabel);
    void sProg5Final3(QString patientLabel);
    void sProg6Test(QString patientLabel);
    void sProg6Final(QString patientLabel);
    void sProg6Final2(QString patientLabel);
    void sProg6Final3(QString patientLabel);

private:

    QString findXMLNodeValue(QDomElement const& root, QString const& parentname, QString const& childname);

};

#endif // MANAGECONFIGFILE_H
