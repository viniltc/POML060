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
   // ui->label_5->setText("Click open the log file button and select a log file to summarise");
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    ui->pushButton->setToolTip("Return to the main screen to access other functionalities.");
    ui->pushButton_2->setToolTip("Save the summarized log table as a CSV file in the summary folder.");
    ui->pushButton_3->setToolTip("Reset the stimulator usage log. Note: This action is irreversible.");
    ui->pushButton_4->setToolTip("Open and load a usage log file to view its summary in the table.");
    ui->pushButton_5->setToolTip("Assign the stimulator log to the current user with today's timestamp.");

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
      reply = QMessageBox::question(this, "Reset Log", "Are you sure want to reset the stimulator log?\n\n(This action cannot be redone)",
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

QString StimulatorLogWindow::formatTime(int seconds)
{
    if (seconds < 60) {
           return QString("%1 s").arg(seconds);
       } else if (seconds < 3600) {
           return QString("%1 min %2 s").arg(seconds / 60).arg(seconds % 60);
       } else {
           int hours = seconds / 3600;
           int minutes = (seconds % 3600) / 60;
           return QString("%1 h %2 min %3 s").arg(hours).arg(minutes).arg(seconds % 60);
       }
}

QString StimulatorLogWindow::formatTimeFromMilliseconds(int milliseconds)
{
    int seconds = milliseconds / 1000; // Convert to seconds
        if (seconds < 60) {
            return QString("%1 s").arg(seconds); // Seconds
        } else if (seconds < 3600) {
            return QString("%1 min %2 s").arg(seconds / 60).arg(seconds % 60); // Minutes and seconds
        } else {
            int hours = seconds / 3600;
            int minutes = (seconds % 3600) / 60;
            return QString("%1 h %2 min %3 s").arg(hours).arg(minutes).arg(seconds % 60); // Hours, minutes, seconds
        }
}


void StimulatorLogWindow::on_pushButton_4_clicked()
{
     configFileName = QFileDialog::getOpenFileName(
           this, "Open the usage log file", QCoreApplication::applicationDirPath() + "/data/log file/"
       );

       if (configFileName.isEmpty()) {
           return; // User canceled the file dialog
       }

       QString fileName = QFileInfo(configFileName).fileName();
       QString patientName;
       QRegExp rx("log_file_([^_]+)_\\d+");
       if (rx.indexIn(fileName) != -1) {
           patientName = rx.cap(1);
       } else {
           patientName = "Unknown";
       }

       QFile f(configFileName);
       if (!f.open(QFile::ReadOnly | QFile::Text)) {
           QMessageBox::information(0, "File Error", f.errorString());
           return;
       }

       QTextStream in(&f);
       QString firstLine;
       if (!in.atEnd()) {
           firstLine = in.readLine();
       }

       QMessageBox::StandardButton reply = QMessageBox::question(
           this, "Tetragrip Usage Log", "Are you sure you want to load " + patientName + "?", QMessageBox::Yes | QMessageBox::No
       );

       if (reply == QMessageBox::No) {
           f.close();
           return;
       }

       ui->label_3->setText("Patient ID: " + patientName);
       ui->label_4->setText(firstLine);

       QStringList logLines;
       while (!in.atEnd()) {
           logLines.append(in.readLine());
       }
       f.close();

       QList<QStringList> tableData;
       QString powerOnTime, unpausedTime;

       for (const QString &line : logLines) {
           if (line.contains("Power cycles:")) {
               QStringList row = {"Power cycles", line.split(":")[1].trimmed(), ""};
               tableData.append(row);
           } else if (line.contains("Unpause events:")) {
               QStringList row = {"Unpaused events", line.split(":")[1].trimmed(), ""};
               tableData.append(row);
           } else if (line.contains("Power-on time:")) {
               powerOnTime = line.split(":")[1].trimmed();
               if (powerOnTime.endsWith("s")) {
                   powerOnTime.chop(1);
               }
               powerOnTime = formatTime(powerOnTime.toInt());
           } else if (line.contains("Unpaused time:")) {
               unpausedTime = line.split(":")[1].trimmed();
               if (unpausedTime.endsWith("s")) {
                   unpausedTime.chop(1);
               }
               unpausedTime = formatTime(unpausedTime.toInt());
           } else if (line.contains(QRegExp("^\\d+\\s+"))) { // Match Phase data rows
               QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

               if (fields.size() >= 5) {
                   QString phaseNumber = fields[0];
                   QString count = fields[1];
                   QString totalTime = fields[4];

                   bool ok;
                   int totalTimeMs = totalTime.toInt(&ok);
                   if (ok) {
                       totalTime = formatTimeFromMilliseconds(totalTimeMs);
                   }

                   if (count.toInt() > 0) {
                       QStringList row = {QString("Phase %1").arg(phaseNumber), count, totalTime};
                       tableData.append(row);
                   }
               }
           }
       }

       for (QStringList &row : tableData) {
           if (row[0] == "Power cycles") {
               row[2] = powerOnTime;
           } else if (row[0] == "Unpaused events") {
               row[2] = unpausedTime;
           }
       }

       QTableWidget *tableWidget = ui->tableWidget;
       tableWidget->setRowCount(tableData.size());
       tableWidget->setColumnCount(3);
       tableWidget->setHorizontalHeaderLabels({"Name", "Count", "Total time"});

       for (int i = 0; i < tableData.size(); ++i) {
           for (int j = 0; j < 3; ++j) {
               tableWidget->setItem(i, j, new QTableWidgetItem(tableData[i][j]));
           }
       }
}



void StimulatorLogWindow::on_pushButton_5_clicked()
{
    // Show a message box to confirm the action
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Assign Log file", "Are you sure you want to assign the stimulator usage log to this user with today's timestamp?",
                                       QMessageBox::Ok | QMessageBox::Cancel);

        // If the user selects Cancel, exit the function
        if (reply != QMessageBox::Ok) {
            qDebug() << "User canceled the log file creation.";
            return;
        }

        // Define file names
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString namestring = "log_file_" + pLabel + "_" + timestamp; // Add timestamp to file name
        QString originalFilename = "logfilename.txt";

        // Construct the path for the new file
        QString saveDir = QCoreApplication::applicationDirPath() + "/data/log file";
        QDir dir(saveDir);

        // Ensure the directory exists, create if it doesn't
        if (!dir.exists()) {
            if (!dir.mkpath(saveDir)) {
                qDebug() << "Failed to create directory:" << saveDir;
                return;
            }
        }

        QString newFilename = saveDir + "/" + namestring + ".txt";

        QFile originalFile(originalFilename);
        QFile newFile(newFilename);

        // Open the original log file
        if (!originalFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Error opening original log file: " << originalFile.errorString();
            return; // Exit if the file can't be opened
        }

        // Open the new file for writing
        if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Error creating new log file: " << newFile.errorString();
            originalFile.close(); // Ensure the original file is closed
            return; // Exit if the file can't be created
        }

        QTextStream instream(&originalFile);
        QTextStream outstream(&newFile);

        // Add date and time information to the new file
        outstream << "Last log assigned to this user:  "
                  << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n\n";

        // Copy content from the original file to the new file
        while (!instream.atEnd()) {
            QString line = instream.readLine();
            outstream << line << "\n";
        }

        // Close the files
        originalFile.close();
        newFile.close();

        qDebug() << "New log file created successfully: " << newFilename;

}

void StimulatorLogWindow::on_pushButton_2_clicked()
{
    // Ensure table is not empty
       if (ui->tableWidget->rowCount() == 0) {
           QMessageBox::warning(this, "Save Error", "The table is empty. Nothing to save.");
           return;
       }

       // Get the log file folder and create the "summary" subfolder
       QString logFileFolder = QCoreApplication::applicationDirPath() + "/data/log file/";
       QString summaryFolder = logFileFolder + "summary";

       QDir dir;
       if (!dir.exists(summaryFolder)) {
           if (!dir.mkpath(summaryFolder)) {
               QMessageBox::critical(this, "Save Error", "Failed to create the summary folder.");
               return;
           }
       }

       // Generate the CSV file name
       QString csvFileName = summaryFolder + "/summary_" + QFileInfo(configFileName).baseName() + ".csv";

       QFile csvFile(csvFileName);
       if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
           QMessageBox::critical(this, "Save Error", "Failed to save the CSV file: " + csvFile.errorString());
           return;
       }

       // Write table content to the CSV file
       QTextStream out(&csvFile);

       // Write the headers
       QStringList headers;
       for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
           headers << ui->tableWidget->horizontalHeaderItem(col)->text();
       }
       out << headers.join(",") << "\n";

       // Write the table rows
       for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
           QStringList rowContent;
           for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
               rowContent << ui->tableWidget->item(row, col)->text();
           }
           out << rowContent.join(",") << "\n";
       }

       csvFile.close();

       // Notify the user
       QMessageBox::information(this, "Save Successful", "Table content saved to: " + csvFileName);
}

