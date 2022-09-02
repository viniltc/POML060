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

private:

    QString findXMLNodeValue(QDomElement const& root, QString const& parentname, QString const& childname);

};

#endif // MANAGECONFIGFILE_H
