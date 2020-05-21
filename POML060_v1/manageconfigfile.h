#ifndef MANAGECONFIGFILE_H
#define MANAGECONFIGFILE_H

#include <QObject>

class ManageConfigFile
{
public:
    ManageConfigFile();
    QString pLabel;
    void keyGripTest(QString patientLabel);
    void keyGripFinal(QString patientLabel);
};

#endif // MANAGECONFIGFILE_H
