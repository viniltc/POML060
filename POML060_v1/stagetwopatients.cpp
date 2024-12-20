#include "stagetwopatients.h"
#include "ui_stagetwopatients.h"
#include "stageonemain.h"
#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDir>
#include <QFileInfoList>
#include <QRegularExpression>

#include "tetra_grip_api.h"
#include "tetra_grip_reader.h"

StageTwoPatients::StageTwoPatients(QWidget *parent) :
//StageTwoPatients::StageTwoPatients(tetra_grip_api *api, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StageTwoPatients)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: white;");
    this->setFixedSize(this->width(),this->height());

    ui->pushButton_Open->setToolTip("Select a patient from the table and open for further setup ");
    ui->pushButton_New->setToolTip("To create a new patient profile");
    ui->pushButton_Modify->setToolTip("To modify patient details or to add notes");
    ui->pushButton_Remove->setToolTip("To permanantly remove a patient from the system");
    ui->pushButton_Home->setToolTip("To move back to home screen");
    ui->qLed_p2->setToolTip("Stimulator Battery and Connection Status LED");

  connect(&api, &tetra_grip_api::tetraGripEvent,this, &StageTwoPatients::eventHandlerTwo);

 // Setup table
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList{"Patient ID","Name", "Surname", "LastSession", "Note"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);




    QString path = QCoreApplication::applicationDirPath()+"/data/";

    QStringList qdiFilter("*.xml");


    QDirIterator qdi( path, qdiFilter, QDir::Files);
    while (qdi.hasNext())
    {
        parseDataEntry(qdi.next());
    }

    tetra_grip_api::get_battery_percentage();
    tetra_grip_api::get_sensor_status();
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
    StageOneMain *newPatient = new StageOneMain(selection[0].data().toString(),nullptr);
    newPatient ->setAttribute(Qt::WA_DeleteOnClose);
    newPatient-> show();
 }


}

void StageTwoPatients::on_pushButton_New_clicked()
{
    this->hide();
    stagetwonew = new StageTwoNew("", nullptr);
    stagetwonew -> setWindowTitle("New details");
    stagetwonew -> show();
}

void StageTwoPatients::on_pushButton_Modify_clicked()
{

 QModelIndexList selection=ui->tableWidget->selectionModel()->selectedRows(0);

 if(selection.empty())
 {
     QMessageBox::warning(this,"TetraGrip","Select a patient to modify details"
                          ,QMessageBox::Ok	,QMessageBox::NoButton);
     return;
 }

 else {
    this-> close();
    StageTwoNew *modifyPatient = new StageTwoNew(selection[0].data().toString(),nullptr);
    modifyPatient-> show();
 }

}

void StageTwoPatients::on_pushButton_Remove_clicked()
{

//    QMessageBox::StandardButton reply;
//      reply = QMessageBox::critical(this, "TetraGrip", "Removing the patient profile will permanently delete all the patient details and custom Tetragrip settings.\n\nAre you sure want to delete this patient?\n\nClick Yes to remove or No to go back to the provious window",
//                                    QMessageBox::Yes|QMessageBox::No);
//      if (reply == QMessageBox::Yes) {
//        QModelIndexList selection=ui->tableWidget->selectionModel()->selectedRows(0);
//        QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+selection[0].data().toString()+".xml";
//        QFile xmlfile(xmlReadPath);
//        xmlfile.remove();
//        ui->tableWidget->removeRow(selection[0].row());
//      }

//      else if(reply == QMessageBox::No) {
//          return;
//      }

    // Confirmation dialog
      QMessageBox::StandardButton reply;
      reply = QMessageBox::critical(this, "TetraGrip",
                                    "Removing the patient profile will permanently delete all the patient details, custom Tetragrip settings, and associated configuration files.\n\n"
                                    "Are you sure you want to delete this patient?\n\n"
                                    "Click Yes to remove or No to go back to the previous window.",
                                    QMessageBox::Yes | QMessageBox::No);

      if (reply == QMessageBox::Yes) {
          // Retrieve selected rows
          QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows(0);

          if (selection.isEmpty()) {
              QMessageBox::warning(this, "TetraGrip", "No patient selected. Please select a patient profile to remove.");
              return;
          }

         // int totalConfigFilesDeleted = 0; // Counter for all config files deleted
          QStringList allDeletedConfigFiles;

          // Iterate through selected rows
          for (const QModelIndex &index : selection) {
              QString patientName = index.data().toString();
              QString xmlFilePath = QCoreApplication::applicationDirPath() + "/data/" + patientName + ".xml";
              QFile xmlFile(xmlFilePath);

              // Delete the XML file
              if (xmlFile.exists()) {
                  if (!xmlFile.remove()) {
                      QMessageBox::warning(this, "TetraGrip", "Failed to delete patient XML file: " + xmlFilePath);
                      continue; // Skip to the next selected row
                  }
              } else {
                  QMessageBox::warning(this, "TetraGrip", "Patient XML file not found: " + xmlFilePath);
              }

              // Remove associated configuration files
              QString configDirectory = QCoreApplication::applicationDirPath() + "/data/config_file";
              QDir dir(configDirectory);

              QString configPattern = "*_" + patientName + ".txt";
              QFileInfoList configFiles = dir.entryInfoList(QStringList() << configPattern, QDir::Files);
              QStringList deletedConfigFilesForPatient; // List for current patient's config files
                          for (const QFileInfo &fileInfo : configFiles) {
                              QFile configFile(fileInfo.absoluteFilePath());
                              if (configFile.exists()) {
                                  if (configFile.remove()) {
                                      deletedConfigFilesForPatient.append(fileInfo.fileName());
                                  } else {
                                      QMessageBox::warning(this, "TetraGrip", "Failed to delete configuration file: " + fileInfo.absoluteFilePath());
                                  }
                              }
                          }

                          // Append to global list and notify user for this patient
                          allDeletedConfigFiles.append(deletedConfigFilesForPatient);

                          if (!deletedConfigFilesForPatient.isEmpty()) {
                              QMessageBox::information(this, "TetraGrip",
                                                        QString("Deleted the following configuration file(s) associated with the patient: %1\n\n%2")
                                                            .arg(patientName)
                                                            .arg(deletedConfigFilesForPatient.join("\n")));
                          } else {
                              QMessageBox::information(this, "TetraGrip",
                                                        QString("No configuration files found for the patient: %1").arg(patientName));
                          }

                          // Remove the row from the table
                          ui->tableWidget->removeRow(index.row());
                      }

                      // Final summary for all deletions
                      if (!allDeletedConfigFiles.isEmpty()) {
                          QMessageBox::information(this, "TetraGrip",
                                                    QString("Patient profiles removed successfully.\n\nDeleted configuration files:\n%1")
                                                        .arg(allDeletedConfigFiles.join("\n")));
                      } else {
                          QMessageBox::information(this, "TetraGrip", "Patient profiles removed successfully. No configuration files were found or deleted.");
                      }
                  }
                  // User clicked No
                  else if (reply == QMessageBox::No) {
                      return;
                  }

}

void StageTwoPatients::eventHandlerTwo( STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value)
{


    if (topic==TOPIC_STIMULATOR)
    {
        switch(reg)
        {
        case STIM_REG_BATTERY_CAPACITY_REMAINING:

            if(value<60)
                ui->qLed_p2->setOnColor(QLed::Red);
            else
                ui->qLed_p2->setOnColor(QLed::Green);
                ui->qLed_p2->setValue(true);
            break;
        case STIM_REG_NUM_SMART_SENSORS:
           if(value==0){
               QMessageBox::critical(this, "Sensor Not Connected!", "Shoulder sensor got disconnected \n\nReconnect the sensor and press Ok to continue");
           }

            break;
        }
    }

 }

void StageTwoPatients::on_pushButton_Home_clicked()
{
    this-> close();
    StageOneMain *newPatienth = new StageOneMain(nullptr);
    newPatienth->setAttribute(Qt::WA_DeleteOnClose);
    newPatienth-> show();
}

void StageTwoPatients::closeEvent(QCloseEvent *event)
{
    if(event->spontaneous()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "TetraGrip", "Are you sure want to quit Tetragrip App?\n\nAny unsaved patient profile will be deleted\n\nClick Yes to quit or No to remain in this window",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            event->accept();

        }
        else if(reply == QMessageBox::No) {
            event->ignore();
        }
    }

}
