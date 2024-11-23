#include "stimulatorlogwindow.h"
#include "ui_stimulatorlogwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "stageonemain.h"

StimulatorLogWindow::StimulatorLogWindow(QString patientLabel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StimulatorLogWindow)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    ui->label_5->setText("Click open the log file button and select a log file to summarise");



}

StimulatorLogWindow::~StimulatorLogWindow()
{
    delete ui;
}

void StimulatorLogWindow::on_pushButton_clicked()
{
    StageOneMain *back;
    this->close();
    back = new StageOneMain(pLabel,nullptr);
    back->setAttribute(Qt::WA_DeleteOnClose);
    back -> show();
}

void StimulatorLogWindow::on_pushButton_3_clicked()
{
    // to reset stimulator log
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "TetraGrip", "Are you sure want to reset the stimulator log?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          tetra_grip_api::do_stimulator_command(STIM_COMMAND_RESET_LOG_HEADER);
          tetra_grip_api::do_stimulator_command(STIM_COMMAND_RESET_LOG_PHASE_DATA);
      }

      else if(reply == QMessageBox::No) {
          return;
      }
}

void StimulatorLogWindow::closeEvent(QCloseEvent *event)
{
    if(event->spontaneous()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "TetraGrip", "Are you sure want to quit Tetragrip App?\n\nClick Yes to quit or No to remain in this window",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            event->accept();

        }
        else if(reply == QMessageBox::No) {
            event->ignore();
        }
    }

}

QString StimulatorLogWindow::getLastWordInFilePath(const QString &filePath)
{

        QString cleanedPath = filePath.trimmed().remove(QRegExp("[\\\\/]$"));


        int lastSeparatorIndex = cleanedPath.lastIndexOf(QRegExp("[\\\\/]"));


        QString lastWord = cleanedPath.mid(lastSeparatorIndex + 1);

        return lastWord;
}






void StimulatorLogWindow::on_pushButton_4_clicked()
{
    // Open a file dialog to select the log file
    QString configFileName = QFileDialog::getOpenFileName(
                this,
                "Open the usage log file",
                QCoreApplication::applicationDirPath() + "/data/log file/"
                );

    if (configFileName.isEmpty()) {
        return;
    }

    QString lastWord = getLastWordInFilePath(configFileName);

    // Extract patient name from the file name
    QString fileName = QFileInfo(configFileName).fileName();
    QString patientName;
    QRegExp rx("log_file_([^_]+)_\\d+"); // Extracts the part between "log_file_" and the numeric suffix
    if (rx.indexIn(fileName) != -1) {
        patientName = rx.cap(1); // Extracted patient name
    } else {
        patientName = "Unknown"; // Fallback if the file name doesn't match the expected pattern
    }


    //  ui->label_3->setText("Patient Name: " + patientName);
    QFile f(configFileName);

    // Open the file
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::information(0, "File Error", f.errorString());
        return;
    }

    // Extract the first line from the log file
    QTextStream in(&f);
    QString firstLine;
    if (!in.atEnd()) {
        firstLine = in.readLine(); // Read the first line
    }




    QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Tetragrip Usage Log",
                "Are you sure you want to load " + lastWord + "?",
                QMessageBox::Yes | QMessageBox::No
                );

    if (reply == QMessageBox::No) {
        f.close();
        return;
    }

    ui->label_3->setText("Patient ID: " + patientName);
    ui->label_4->setText(firstLine);

    // Parse the file content

    QStringList logLines;
    while (!in.atEnd()) {
        logLines.append(in.readLine());
    }
    f.close();

    // Extract data for the table
    QList<QStringList> tableData;
    QString powerOnTime, unpausedTime; // Variables to store extracted values

    for (const QString &line : logLines) {
           if (line.contains("Power cycles:")) {
               QStringList row = {"Power cycles", line.split(":")[1].trimmed(), ""};
               tableData.append(row);
           } else if (line.contains("Unpause events:")) {
               QStringList row = {"Unpaused events", line.split(":")[1].trimmed(), ""};
               tableData.append(row);
           } else if (line.contains("Power-on time:")) {
               powerOnTime = line.split(":")[1].trimmed();
           } else if (line.contains("Unpaused time:")) {
               unpausedTime = line.split(":")[1].trimmed();
           } else if (line.contains("Phase")) {
               QRegExp rxPhase("Phase\\s*(\\d+)\\s*(\\d+)\\s*.*\\s*(\\d+)"); // Match phase details
               if (rxPhase.indexIn(line) != -1) {
                   QString phaseNumber = rxPhase.cap(1);
                   QString count = rxPhase.cap(2);
                   QString totalTime = rxPhase.cap(3) + " ms";

                   // Add only phases with non-zero count
                   if (count.toInt() > 0) {
                       QStringList row = {QString("Phase %1").arg(phaseNumber), count, totalTime};
                       tableData.append(row);
                   }
               }
           }
       }

       // Update Power-on time and Unpaused time in the table
       for (QStringList &row : tableData) {
           if (row[0] == "Power cycles") {
               row[2] = powerOnTime; // Assign Power-on time
           } else if (row[0] == "Unpaused events") {
               row[2] = unpausedTime; // Assign Unpaused time
           }
       }

       // Populate the QTableWidget
       QTableWidget *tableWidget = ui->tableWidget; // Assuming tableWidget is defined in your UI
       tableWidget->setRowCount(tableData.size());
       tableWidget->setColumnCount(3);
       tableWidget->setHorizontalHeaderLabels({"Name", "Count", "Total time"});

       for (int i = 0; i < tableData.size(); ++i) {
           for (int j = 0; j < 3; ++j) {
               tableWidget->setItem(i, j, new QTableWidgetItem(tableData[i][j]));
           }
       }
}
