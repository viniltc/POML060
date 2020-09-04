#include "stagetwonew.h"
#include "ui_stagetwonew.h"
#include "stagetwopatients.h"
#include <QDomDocument>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QtXml>

StageTwoNew::StageTwoNew(QString patientLabel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StageTwoNew)
{
     ui->setupUi(this);
     QDir dir(QCoreApplication::applicationDirPath());
     QString location = dir.relativeFilePath("./PatientDat");
    // qDebug()<<"This is the location::"<<location;
    //QString path = location+"/"+filename+".xml";
    // connect(, SIGNAL(editdetails(QString, QString, QString)),this,SLOT(editdetails(QString, QString, QString)));
   pLabel = patientLabel;

}

StageTwoNew::~StageTwoNew()
{
    delete ui;
}

void StageTwoNew::on_pushButton_2_clicked()
{

     hide();
     StageTwoPatients().show();
}

void StageTwoNew::on_pushButton_clicked()
{

    if(ui->lineEdit_3->text().isEmpty())
    QMessageBox::critical(this, "Warning", "Please enter a valid patient ID");
    QString text = ui->textEdit->toPlainText();
    if(text.isEmpty())
    QMessageBox::warning(this, "Warning", "Notes area is empty");

    QString filename = ui->lineEdit_3->text();
    QString date = ui->dateTimeEdit->text();
    qDebug()<<"Date Time: "<<date;
  // QString location = dir.relativeFilePath("../PatientData");
    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
    QFile file(path);


//    QString filename = QFileDialog::getSaveFileName(this, "Save Xml", location.append(ui->lineEdit_3->text()), "Xml files (*.xml)");

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug () << "Error saving XML file...."; // replace this with Q Messange box later!!!!!!!!!!!
        file.close();
        return;
    }

    QDomDocument xml("tetragrip");

    QDomElement root = xml.createElement("subject_details");
    //root.setAttribute("category","run session");
    xml.appendChild(root);

    QDomElement tagName = xml.createElement("Name");
    root.appendChild(tagName);
    QDomText textName = xml.createTextNode(ui->lineEdit->text());
    tagName.appendChild(textName);

    QDomElement tagSname = xml.createElement("Surname");
    root.appendChild(tagSname);
    QDomText textSname = xml.createTextNode(ui->lineEdit_2->text());
    tagSname.appendChild(textSname);

    QDomElement tagPID = xml.createElement("Patient_ID");
    root.appendChild(tagPID);
    QDomText textPID = xml.createTextNode(ui->lineEdit_3->text());
    tagPID.appendChild(textPID);

    QDomElement tagDateTime = xml.createElement("Date");
    root.appendChild(tagDateTime);
    QDomText textDateTime = xml.createTextNode(ui->dateTimeEdit->text());
    tagDateTime.appendChild(textDateTime);

    QDomElement tagNote = xml.createElement("Clinician_Note");
    root.appendChild(tagNote);
    QDomText textNote = xml.createTextNode(ui->textEdit->toPlainText());
    tagNote.appendChild(textNote);


    QTextStream output(&file);
    output << xml.toString();

    file.close();


    if((ui->lineEdit_3->text().length()!=0) & (ui->textEdit->toPlainText().length()!=0))
        {

        this->close();
        StageTwoPatients *back = new StageTwoPatients(this);
        back->show();
        }

    //  emit tableUpdate();
}

void StageTwoNew::on_tabWidget_currentChanged(int index)
{
    if(index==1 | index ==2)
    {
       if(ui->lineEdit_3->text().isEmpty())

       QMessageBox::critical(this, "Warning", "Please enter a valid patient ID");
    }
}
