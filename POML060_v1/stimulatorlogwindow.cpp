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

    QString name = "log_file_"+pLabel+".txt";
   // QString newfilename = QCoreApplication::applicationDirPath()+"/data/log_file/"+namestring+".txt";

    QFile file(name);

    file.open(QIODevice::ReadOnly|QIODevice::Text);

    QTextStream instream(& file);

//    while (!instream.atEnd()){
//            QString line = instream.readLine();
//            qDebug()<<line;
//            ui->textBrowser->append(line); // append or setText (use readAll when setText is used)
//        }



    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList{"","Count", "Total time"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    QString pcycles = "";
    QString unpaused_events = "";

    QRegExp rx("(\\ |\\,|\\.|\\:|\\t)");
    int nb_line = 0;
    while(!instream.atEnd())
    {

        QString line = instream.readLine();



        if (nb_line >= 4)
        {
            ui->textBrowser->append(line);
        }



        if(nb_line == 4 )
        {
            QStringList slist = line.split(rx);
            if(slist.size() >3) {
                pcycles = slist[3];
            } else {
                qDebug() << "Problem with string" << slist << line;
            }
        }

        if(nb_line == 5 )
        {
            QStringList slist = line.split(rx);
            if(slist.size() >3) {
                unpaused_events = slist[3];
            } else {
                qDebug() << "Problem with string" << slist << line;
            }
        }

        if(nb_line == 6 )
        {
            QStringList slist = line.split(rx);
            // slist[4]= QString::number(slist[4].toInt()/(1000));

            if(slist.size() >3) {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         0,
                                         new QTableWidgetItem("Power cycles"));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         1,
                                         new QTableWidgetItem( pcycles));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         2,
                                         new QTableWidgetItem(slist[3]));
            } else {
                qDebug() << "Tablewidget (powercycles),Problem with string" << slist << line;
            }
        }


        if(nb_line == 7 )
        {
            QStringList slist = line.split(rx);
            // slist[4]= QString::number(slist[4].toInt()/(1000));

            if(slist.size() >3) {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         0,
                                         new QTableWidgetItem("Unpaused Events"));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         1,
                                         new QTableWidgetItem( unpaused_events));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         2,
                                         new QTableWidgetItem(slist[3]));
            } else {
                qDebug() << "Tablewidget (unpausedevents),Problem with string" << slist << line;
            }
        }

        if(nb_line == 12 )
        {
            QStringList slist = line.split(rx);
            if(slist.size() >8) {

                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         0,
                                         new QTableWidgetItem("Key Grip"));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         1,
                                         new QTableWidgetItem( slist[5]));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         2,
                                         new QTableWidgetItem(slist[8]+ "s"));
            } else {
                qDebug() << "Tablewidget (keygrip), Problem with string" << slist << line;
            }

        }

        if(nb_line == 16 )
        {
            QStringList slist = line.split(rx);
            if(slist.size() >8) {

                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         0,
                                         new QTableWidgetItem("Palmar Grasp "));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         1,
                                         new QTableWidgetItem( slist[5]));
                ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                         2,
                                         new QTableWidgetItem(slist[8]+ "s"));
            } else {
                qDebug() << "Tablewidget (Palmar),Problem with string" << slist << line;
            }

        }

        if(nb_line == 24 )
        {
            QStringList slist = line.split(rx);
                 if(slist.size() >7) {

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                     0,
                                     new QTableWidgetItem("Exercise"));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                     1,
                                     new QTableWidgetItem( slist[4] ));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1,
                                     2,
                                     new QTableWidgetItem(slist[7]+ "s"));
        } else {
            qDebug() << "Tablewidget (exercise), Problem with string" << slist << line;
        }

        }


        ++nb_line;
    }



     file.close();
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
        reply = QMessageBox::information(this, "TetraGrip", "Are you sure want to quit Tetragrip App?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            event->accept();

        }
        else if(reply == QMessageBox::No) {
            event->ignore();
        }
    }

}
