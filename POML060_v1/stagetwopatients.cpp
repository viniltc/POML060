#include "stagetwopatients.h"
#include "ui_stagetwopatients.h"
#include "stageonemain.h"
#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QMessageBox>
#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"

StageTwoPatients::StageTwoPatients(QWidget *parent) :
//StageTwoPatients::StageTwoPatients(tetra_grip_api *api, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StageTwoPatients)
{
    ui->setupUi(this);

//    api->openSerialPort();
//    connect(api, SIGNAL(readyRead()), api, SLOT(readData()));
    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());



  connect(&api, &tetra_grip_api::tetraGripEvent,this, &StageTwoPatients::eventHandlerTwo);

 // Setup table
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList{"Patient ID","Name", "Surname", "LastSession", "Note"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);



  //  connect(ui->pushButton_4, SIGNAL(clicked()), ui->tableWidget,SLOT(cellClicked()));



    QString path = QCoreApplication::applicationDirPath()+"/data/";



    QStringList qdiFilter("*.xml");

  //  patientData data;

    QDirIterator qdi( path, qdiFilter, QDir::Files);
    while (qdi.hasNext())
    {
        parseDataEntry(qdi.next());
        //data.parseDataEntry(qdi.next());


    }



    tetra_grip_api::get_battery_percentage();
    tetra_grip_api::set_sensor_data_rate(SENSOR_ADDRESS_BROADCAST, 0);

}

void StageTwoPatients::parseDataEntry(const QString dataPath)
{

    QString patientID, firstName, surName, lastSession = "", pNote;

    // Load our XML file.
    QFile *xmlFile;
    xmlFile = new QFile(dataPath);
    if(!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,
                              "Error Loading Patient Data File",
                              QString("Could not load the patient XML data file at:\r\n  %0").arg(dataPath),
                              QMessageBox::Ok);
        return;
    }

    // Create an XML reader.
    QXmlStreamReader *xmlReader;
    xmlReader = new QXmlStreamReader(xmlFile);


    // Parse each element of the XML until we reach the end.
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        // If token is just StartDocument - go to next
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        // If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {

            if(xmlReader->name() == "Name") {
                firstName = xmlReader->readElementText();

            } else if(xmlReader->name() == "Surname") {
                surName = xmlReader->readElementText();
            }
            else if(xmlReader->name() == "Patient_ID") {
                patientID = xmlReader->readElementText();
            }
            else if(xmlReader->name() == "Date") {
                lastSession = xmlReader->readElementText();
            }
            else if(xmlReader->name() == "Clinician_Note") {
                pNote = xmlReader->readElementText();
            }
        }
    }

    if(xmlReader->hasError()) {
        QMessageBox::critical(this,
                              "Error Parsing Patient Data File",
                              QString("Error reading the patient file at:\r\n  %0,\r\nError:  %1").arg(dataPath,
                                                                                                       xmlReader->errorString()),
                              QMessageBox::Ok);
        return;
    }


    // close reader and flush file
    xmlReader->clear();
    xmlFile->close();


    // Delete
    delete xmlFile;
    delete xmlReader;


    // Add a new row to the table, with the data we parsed.
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                             0,
                             new QTableWidgetItem(patientID));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                             1,
                             new QTableWidgetItem(firstName));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                             2,
                             new QTableWidgetItem(surName));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                             3,
                             new QTableWidgetItem(lastSession));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                             4,
                             new QTableWidgetItem(pNote));
  //  ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,3, new QTableWidgetItem(lastSession))
}

StageTwoPatients::~StageTwoPatients()
{
    delete ui;
}


void StageTwoPatients::on_pushButton_Open_clicked()
{
    QModelIndexList selection=ui->tableWidget->selectionModel()->selectedRows(0);

 if(selection.empty())
 {
     QMessageBox::warning(this,"TetraGrip","Warning: Select a patient to start or add new patient"
                          ,QMessageBox::Ok	,QMessageBox::NoButton);

     return;
 }

 else {
    this-> close();
    StageOneMain *newPatient = new StageOneMain(selection[0].data().toString(),this);
    newPatient ->setAttribute(Qt::WA_DeleteOnClose);
    newPatient-> show();
 }


}

void StageTwoPatients::on_pushButton_New_clicked()
{
    this->close();
    stagetwonew = new StageTwoNew(this);
    stagetwonew -> setWindowTitle("New details");
    stagetwonew -> show();
}

void StageTwoPatients::on_pushButton_Modify_clicked()
{

}

void StageTwoPatients::on_pushButton_Remove_clicked()
{

}

void StageTwoPatients::eventHandlerTwo( STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value)
{
    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_BATTERY_CAPACITY_REMAINING:

            if(value<86)
                ui->qLed_p2->setOnColor(QLed::Red);
            else
                ui->qLed_p2->setOnColor(QLed::Green);
                ui->qLed_p2->setValue(true);
            break;
        }
    }

 }
