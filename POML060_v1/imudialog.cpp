#include "imudialog.h"
#include "ui_imudialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QtXml>

IMUDialog::IMUDialog(QString patientLabel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IMUDialog)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    ui->radioButton->setChecked(true);
    QMetaObject::invokeMethod(this, [this]() {
           emit radioButtonChanged(ui->radioButton->text());
       }, Qt::QueuedConnection);

    QMetaObject::invokeMethod(this, [this]() {
           emit  checkBoxChanged(false);
       }, Qt::QueuedConnection);
    ui->spinBox_3->setRange(1, 90);         // Set the range from 1 to 90
    ui->spinBox_3->setSingleStep(1);        // Set the step increment to 1
    ui->spinBox_3->setValue(30);
    ui->spinBox_2->setRange(1, 90);         // Set the range from 1 to 90
    ui->spinBox_2->setSingleStep(1);        // Set the step increment to 1
    ui->spinBox_2->setValue(30);

    armFlexionAngle = ui->spinBox_3->value();
    elevationAngle = ui->spinBox_2->value();



    QComboBox* timeoutDurComboBoxes[] { ui->comboBox };
    const double timeoutValues[] {2, 3, 4, 5, 6, 7, 8};
    for(auto* cb : timeoutDurComboBoxes)
    {
        for(auto val : timeoutValues)
        {
            cb->addItem(QString::number(static_cast<int>(val * 1000)) + " ms", QVariant(static_cast<int>(val * 1000)));
        }
    }

    ui->comboBox->setCurrentIndex(0);    // Set default index to 0
    ui->comboBox->setEnabled(false);     // Disable the combo box initially

    // Retrieve the integer value of the selected item and assign it to IMUTimeout
     IMUTimeout = ui->comboBox->currentData().toInt();
    //ui->label_5->setText(QString::number(IMUTimeout)); // Display IMUTimeout on label

}

IMUDialog::~IMUDialog()
{
    delete ui;
}

void IMUDialog::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)  // Qt::Checked is 2
    {
        ui->comboBox->setEnabled(true);
        emit checkBoxChanged(true);
    }
    else if (arg1 == Qt::Unchecked)  // Qt::Unchecked is 0
    {
        ui->comboBox->setEnabled(false);
        emit checkBoxChanged(false);
    }
}

void IMUDialog::saveToXMLFile()
{
    QString filename = pLabel;
    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
    QFile file(path);

    /* QT Append wont work!
     * Open the file read-only, read it all in, close it.
     * Make changes in-memory document.
     * Then open the file for overwrite, write all content, close file. */

    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {

        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
        return;
    }

     QDomDocument document;
     document.setContent(&file);
     QDomElement root = document.documentElement();


    file.close();

    QDomElement newFiltTag = document.createElement(QString("IMU_Stroke_Settings"));

    QDomNode FiltNode = root.elementsByTagName("IMU_Stroke_Settings").at(0).firstChild();
    QDomElement FiltNodeVal = FiltNode.toElement();

    if (FiltNodeVal.isNull())
    {
        QDomElement a1 = document.createElement("elevation_angle");
        QDomText a1_value = document.createTextNode(QString::number(elevationAngle));
        a1.appendChild(a1_value);
        newFiltTag.appendChild(a1);

        QDomElement a2 = document.createElement("flexion_angle");
        QDomText a2_value = document.createTextNode(QString::number(armFlexionAngle));
        a2.appendChild(a2_value);
        newFiltTag.appendChild(a2);

        QDomElement b0 = document.createElement("release_timeout");
        QDomText b0_value = document.createTextNode(QString::number(IMUTimeout));
        b0.appendChild(b0_value);
        newFiltTag.appendChild(b0);
        root.appendChild(newFiltTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("IMU_Stroke_Settings");

          QDomNode a1l = SettingsNode.namedItem("elevation_angle");
          a1l.firstChild().setNodeValue(QString::number(elevationAngle));
          QDomNode a2l = SettingsNode.namedItem("flexion_angle");
          a2l.firstChild().setNodeValue(QString::number(armFlexionAngle));
          QDomNode b0l = SettingsNode.namedItem("release_timeout");
          b0l.firstChild().setNodeValue(QString::number(IMUTimeout));

    }


    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        qDebug () << "Error saving XML file....";
        QMessageBox::information(this, "Unable to open XML file to write", file.errorString());
        return;
    }

    QTextStream output(&file);
    output << document.toString();
    file.close();

    qDebug()<< "Finished";
}


void IMUDialog::on_spinBox_2_valueChanged(int arg1)
{
    elevationAngle = ui->spinBox_2->value();

   // ui->label_3->setText(QString::number(elevationAngle));
  //  saveToXMLFile();
}


void IMUDialog::on_spinBox_3_valueChanged(int arg1)
{
    armFlexionAngle = ui->spinBox_3->value();
     //   ui->label_4->setText(QString::number(armFlexionAngle));
        //saveToXMLFile();
}




void IMUDialog::on_pushButton_clicked()
{
    saveToXMLFile();
    this->close();
}


void IMUDialog::on_comboBox_currentIndexChanged(int index)
{
    IMUTimeout = ui->comboBox->currentData().toInt();
 //  ui->label_5->setText(QString::number(IMUTimeout));
}


void IMUDialog::on_radioButton_toggled(bool checked)
{
    if (checked) {
            emit radioButtonChanged(ui->radioButton->text());
        }
}


void IMUDialog::on_radioButton_2_toggled(bool checked)
{
    if (checked) {
           emit radioButtonChanged(ui->radioButton_2->text());
       }
}


void IMUDialog::on_pushButton_2_clicked()
{
    this->close();
}

