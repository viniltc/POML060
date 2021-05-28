#include "logwindow.h"
#include "ui_logwindow.h"
#include <QFile>
#include <QTextStream>

logwindow::logwindow(QString patientLabel,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logwindow)
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

logwindow::~logwindow()
{
    delete ui;
}
