#include "stimulatorlogwindow.h"
#include "ui_stimulatorlogwindow.h"
#include <QFile>
#include <QTextStream>

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
    QString line;

    while (!instream.atEnd()){
            line = instream.readLine();
            ui->textBrowser->setText(line+"\n");
        }


     file.close();
}

StimulatorLogWindow::~StimulatorLogWindow()
{
    delete ui;
}
