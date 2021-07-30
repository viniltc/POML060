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

    while (!instream.atEnd()){
            QString line = instream.readLine();
            qDebug()<<line;
            ui->textBrowser->append(line); // append or setText (use readAll when setText is used)
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
