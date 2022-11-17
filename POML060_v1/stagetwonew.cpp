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
     pLabel = patientLabel;

     ui->tabWidget->setCurrentWidget(ui->tab);

     QDomDocument document;
     QString xmlName = pLabel;
     QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
     QFile xmlfile(xmlReadPath);

     if(!xmlfile.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         qDebug () << "Error opening XML file: "<<xmlfile.errorString();

     }
     document.setContent(&xmlfile);
     QDomElement root = document.documentElement();
     xmlfile.close();


     if (!root.elementsByTagName("Name").at(0).firstChild().nodeValue().isNull())
     {
        ui->lineEdit->setText(root.elementsByTagName("Name").at(0).firstChild().nodeValue());
        ui->lineEdit_2->setText(root.elementsByTagName("Surname").at(0).firstChild().nodeValue());
        ui->lineEdit_3->setText(root.elementsByTagName("Patient_ID").at(0).firstChild().nodeValue());
        ui->textEdit->setText(root.elementsByTagName("Clinician_Note").at(0).firstChild().nodeValue());
     }




     ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

StageTwoNew::~StageTwoNew()
{
    delete ui;
}

void StageTwoNew::on_pushButton_2_clicked()
{

//     hide();
//     StageTwoPatients().show();
     this->close();
     StageTwoPatients *back = new StageTwoPatients(this);
     back->show();
}

void StageTwoNew::on_pushButton_clicked()
{


    QString filename = pLabel;
    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
    QFile file(path);

    /* QT Append wont work!
     * Open the file read-only, read it all in, close it.
     * Make changes in-memory document.
     * Then open the file for overwrite, write all content, close file. */

    if(file.exists())
    {

    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {

        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
        return;
    }

     QDomDocument document;
     document.setContent(&file);
     QDomElement root = document.documentElement();


    file.close();

    QDomNode NameNode = root.namedItem("Name");
    NameNode.firstChild().setNodeValue(ui->lineEdit->text());
    QDomNode SNameNode = root.namedItem("Surname");
    SNameNode.firstChild().setNodeValue(ui->lineEdit_2->text());
    QDomNode PIDNode = root.namedItem("Patient_ID");
    PIDNode.firstChild().setNodeValue(ui->lineEdit_3->text());
    QDomNode DateNode = root.namedItem("Date");
    DateNode.firstChild().setNodeValue(ui->dateTimeEdit->text());
    QDomNode NoteNode = root.namedItem("Clinician_Note");
    NoteNode.firstChild().setNodeValue(ui->textEdit->toPlainText()+"("+ui->dateTimeEdit->text()+")");

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


    else
    {
            if(ui->lineEdit_3->text().isEmpty())
            {
            QMessageBox::critical(this, "Warning", "Please enter a valid patient ID");
            return;
            }
            QString text = ui->textEdit->toPlainText();
            if(text.isEmpty())
            QMessageBox::warning(this, "Warning", "Notes area is empty");

            QString filename = ui->lineEdit_3->text();
            QString date = ui->dateTimeEdit->text();
            qDebug()<<"Date Time: "<<date;
            QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
            QFile file(path);


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
            QDomText textNote = xml.createTextNode(ui->textEdit->toPlainText()+"("+ui->dateTimeEdit->text()+")");
            tagNote.appendChild(textNote);


            QTextStream output(&file);
            output << xml.toString();

            file.close();




    }

    if((ui->lineEdit_3->text().length()!=0) & (ui->textEdit->toPlainText().length()!=0))
        {

        this->close();
        StageTwoPatients *back = new StageTwoPatients(this);
        back->show();
        }

//    if(ui->lineEdit_3->text().isEmpty())
//    QMessageBox::critical(this, "Warning", "Please enter a valid patient ID");
//    QString text = ui->textEdit->toPlainText();
//    if(text.isEmpty())
//    QMessageBox::warning(this, "Warning", "Notes area is empty");

//    QString filename = ui->lineEdit_3->text();
//    QString date = ui->dateTimeEdit->text();
//    qDebug()<<"Date Time: "<<date;
//    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
//    QFile file(path);


//    if(!file.open(QFile::WriteOnly | QFile::Text))
//    {
//        qDebug () << "Error saving XML file...."; // replace this with Q Messange box later!!!!!!!!!!!
//        file.close();
//        return;
//    }

//    QDomDocument xml("tetragrip");

//    QDomElement root = xml.createElement("subject_details");
//    //root.setAttribute("category","run session");
//    xml.appendChild(root);

//    QDomElement tagName = xml.createElement("Name");
//    root.appendChild(tagName);
//    QDomText textName = xml.createTextNode(ui->lineEdit->text());
//    tagName.appendChild(textName);

//    QDomElement tagSname = xml.createElement("Surname");
//    root.appendChild(tagSname);
//    QDomText textSname = xml.createTextNode(ui->lineEdit_2->text());
//    tagSname.appendChild(textSname);

//    QDomElement tagPID = xml.createElement("Patient_ID");
//    root.appendChild(tagPID);
//    QDomText textPID = xml.createTextNode(ui->lineEdit_3->text());
//    tagPID.appendChild(textPID);

//    QDomElement tagDateTime = xml.createElement("Date");
//    root.appendChild(tagDateTime);
//    QDomText textDateTime = xml.createTextNode(ui->dateTimeEdit->text());
//    tagDateTime.appendChild(textDateTime);

//    QDomElement tagNote = xml.createElement("Clinician_Note");
//    root.appendChild(tagNote);
//    QDomText textNote = xml.createTextNode(ui->textEdit->toPlainText());
//    tagNote.appendChild(textNote);


//    QTextStream output(&file);
//    output << xml.toString();

//    file.close();


//    if((ui->lineEdit_3->text().length()!=0) & (ui->textEdit->toPlainText().length()!=0))
//        {

//        this->close();
//        StageTwoPatients *back = new StageTwoPatients(this);
//        back->show();
//        }

}

void StageTwoNew::on_tabWidget_currentChanged(int index)
{
    if(index==1 | index ==2)
    {
       if(ui->lineEdit_3->text().isEmpty())

       QMessageBox::critical(this, "Warning", "Please enter a valid patient ID");
    }
}

void StageTwoNew::closeEvent(QCloseEvent *event)
{
    if(event->spontaneous()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "TetraGrip", "You are about to leave this window, have you correctly setup a patient?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            event->accept();
            //return;
        }
        else if(reply == QMessageBox::No) {
            event->ignore();
        }
    }
}
