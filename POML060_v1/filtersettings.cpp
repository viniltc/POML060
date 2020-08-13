#include "filtersettings.h"
#include "ui_filtersettings.h"
#include <QDebug>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QtXml>


FilterSettings::FilterSettings(QString patientLabel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterSettings)
{
    ui->setupUi(this);
    pLabel = patientLabel;
    //ui->pushButton_save->setEnabled(false);

    //lpf
    ui->lineEdit_qlp->setText("0.707"); //Q default value
    ui->lineEdit_qlp->setVisible(false); // invisible for users
    ui->lineEdit_dblp->setText("10"); // Db default value
    ui->lineEdit_dblp->setVisible(false); // invisible for users
    ui->lineEdit_fclp->setText("4"); //fc def value

    ui->lineEdit_fs->setText("30"); // fs default value

    // hpf
    ui->lineEdit_fchp->setText("2"); // fc default value
    ui->lineEdit_qhp->setText("0.707"); //Q default value
    ui->lineEdit_dbhp->setText("10"); // Db default value
    ui->lineEdit_dbhp->setVisible(false); // invisible for users

    ui->lineEdit_qhp->setVisible(false); // invisible for users
    ui->label_3->setVisible(false); // invisible for users
    ui->label_4->setVisible(false); // invisible for users
    ui->label_15->setVisible(false); // invisible for users
    ui->label_16->setVisible(false); // invisible for users

    ui->radioButton_hp->setVisible(false);
    ui->radioButton_lp->setVisible(false);
    ui->radioButton_bp->setVisible(false);
}

FilterSettings::~FilterSettings()
{
    delete ui;
}

void FilterSettings::on_pushButton_save_clicked()
{

   // loadNewConfigFile();
    //ShoulderControl *back;
    this->close();
   // back = new ShoulderControl(pLabel);
    // back -> show();
}

void FilterSettings::biquadratic_filter_coefficient(double Q, double dB, double FS, double FC, int type, float *coeff)
{
    double n0, n1, n2, d1, d2;
        double b0, b1, b2, a0, a1, a2;
            double w0, cosW, sinW, A, alpha, beta;

        // intermediate parameters
        w0 = 2*M_PI*(FC/FS);
            cosW = cos(w0);
            sinW = sin(w0);
        A = pow(10, (dB/40));
        //printf("A = %lf, dB/40 = %lf\n", A, dB/40);
        alpha = sinW/(2*Q);
        beta = pow(A, 0.5)/Q;

        switch (type) {
        case 0: //Low Pass
            b0 = (1 - cosW)/2;
            b1 = 1 - cosW;
            b2 = (1 - cosW)/2;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 1: //High Pass
            b0 = (1 + cosW)/2;
            b1 = -(1 + cosW);
            b2 = (1 + cosW)/2;
            a0 = (1 + alpha);
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 2: //Band Pass Peak gain Q
            b0 = sinW/2;
            b1 = 0;
            b0 = -sinW/2;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 3: //Band Pass constant 0dB gain
            b0 = alpha;
            b1 = 0;
            b2 = -alpha;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 4: //Notch Filter
            b0 = 1;
            b1 = -2*cosW;
            b2 = 1;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 5: //All pass
            b0 = 1 - alpha;
            b1 = -2*cosW;
            b2 = 1 + alpha;
            a0 = 1 + alpha;
            a1 = -2*cosW;
            a2 = 1 - alpha;
            break;
        case 6: // Peaking EQ Filter
            b0 = 1 + (alpha*A);
            b1 = -2*cosW;
            b2 = 1 - (alpha*A);
            a0 = 1 + (alpha/A);
            a1 = -2*cosW;
            a2 = 1 - (alpha/A);
            break;
        case 7: //Low Shelving Filter
            b0 = A*((A + 1) - ((A -1)*cosW) + beta*sinW);
            b1 = 2*A*((A - 1) - ((A + 1)*cosW));
            b2 = A*((A + 1) - (A - 1)*cosW - beta*sinW);
            a0 = ((A + 1) + (A - 1)*cosW + beta*sinW);
            a1 = -2*((A - 1) + (A + 1)*cosW);
            a2 = ((A + 1) + (A - 1)*cosW - (beta*sinW));
            break;
        case 8: //High Shelving Filter
            b0 = A*((A + 1) + (A - 1)*cosW + beta*sinW);
            b1 = -2*A*((A - 1) + (A + 1)*cosW);
            b2 = A*((A + 1) + (A - 1)*cosW -beta*sinW);
            a0 = ((A + 1) - (A - 1)*cosW + beta*sinW);
            a1 = 2*((A - 1) - (A + 1)*cosW);
            a2 = ((A + 1) - (A - 1)*cosW - beta*sinW);
            break;
        default:
            break;

        };

        n0 = b0/a0;
        n1 = b1/a0;
        n2 = b2/a0;
        d1 = a1/a0;
        d2 = a2/a0;

        coeff[0] = (float)n0;
        coeff[1] = (float)n1;
        coeff[2] = (float)n2;
        coeff[3] = (float)d1;
        coeff[4] = (float)d2;
        //0:b2, 1:b1, 2:b0, 3:a1, 4:a2
}

void FilterSettings::on_pushButton_tune_clicked()
{
    FS = ui->lineEdit_fs->text().toDouble(); //Fs
    FC_lpf = ui->lineEdit_fclp->text().toDouble();   //Fc
    Q_lpf = ui->lineEdit_qlp->text().toDouble(); //Q
    dB_lpf = ui->lineEdit_dblp->text().toDouble(); // Db
    FC_hpf = ui->lineEdit_fchp->text().toDouble();   //Fc
    Q_hpf = ui->lineEdit_qhp->text().toDouble(); //Q
    dB_hpf = ui->lineEdit_dbhp->text().toDouble(); // Db

    biquadratic_filter_coefficient(Q_hpf, dB_hpf, FS, FC_hpf, 1, coeff_hpf); // hpf
    biquadratic_filter_coefficient(Q_lpf, dB_lpf, FS, FC_lpf, 0, coeff_lpf); // lpf


    saveToXMLFile();

    ManageConfigFile configFile;
    configFile.filterTest(pLabel);

   // ui->pushButton_save->setEnabled(true);
}

void FilterSettings::saveToXMLFile()
{
    QString filename = pLabel;
    QString path = QCoreApplication::applicationDirPath()+"/data/"+filename+".xml";
    QFile file(path);

    /* QT Append wont work!
     * Open the file read-only, read it all in, close it.
     * Make changes in-memory document.
     * Then open the file for overwrite, write all content, close file. */

    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {

        QMessageBox::information(this, "Unable to open XML file to read", file.errorString());
        return;
    }

     QDomDocument document;
     document.setContent(&file);
     QDomElement root = document.documentElement();


    file.close();

    QDomElement newFiltTag = document.createElement(QString("Filter_Coeffs"));

    QDomNode FiltNode = root.elementsByTagName("Filter_Coeffs").at(0).firstChild();
    QDomElement FiltNodeVal = FiltNode.toElement();

    if (FiltNodeVal.isNull())
    {
        QDomElement a1 = document.createElement("a1l");
        QDomText a1_value = document.createTextNode(QString::number(coeff_lpf[3]));
        a1.appendChild(a1_value);
        newFiltTag.appendChild(a1);

        QDomElement a2 = document.createElement("a2l");
        QDomText a2_value = document.createTextNode(QString::number(coeff_lpf[4]));
        a2.appendChild(a2_value);
        newFiltTag.appendChild(a2);

        QDomElement b0 = document.createElement("b0l");
        QDomText b0_value = document.createTextNode(QString::number(coeff_lpf[0]));
        b0.appendChild(b0_value);
        newFiltTag.appendChild(b0);

        QDomElement b1 = document.createElement("b1l");
        QDomText b1_value = document.createTextNode(QString::number(coeff_lpf[1]));
        b1.appendChild(b1_value);
        newFiltTag.appendChild(b1);

        QDomElement b2 = document.createElement("b2l");
        QDomText b2_value = document.createTextNode(QString::number(coeff_lpf[2]));
        b2.appendChild(b2_value);
        newFiltTag.appendChild(b2);

        QDomElement a1h = document.createElement("a1h");
        QDomText a1h_value = document.createTextNode(QString::number(coeff_hpf[3]));
        a1h.appendChild(a1h_value);
        newFiltTag.appendChild(a1h);

        QDomElement a2h= document.createElement("a2h");
        QDomText a2h_value = document.createTextNode(QString::number(coeff_hpf[4]));
        a2h.appendChild(a2h_value);
        newFiltTag.appendChild(a2h);

        QDomElement b0h = document.createElement("b0h");
        QDomText b0h_value = document.createTextNode(QString::number(coeff_hpf[0]));
        b0h.appendChild(b0h_value);
        newFiltTag.appendChild(b0h);

        QDomElement b1h = document.createElement("b1h");
        QDomText b1h_value = document.createTextNode(QString::number(coeff_hpf[1]));
        b1h.appendChild(b1h_value);
        newFiltTag.appendChild(b1h);

        QDomElement b2h = document.createElement("b2h");
        QDomText b2h_value = document.createTextNode(QString::number(coeff_hpf[2]));
        b2h.appendChild(b2h_value);
        newFiltTag.appendChild(b2h);

        root.appendChild(newFiltTag);
    }

    else
    {
          QDomElement root = document.documentElement();
          QDomNode SettingsNode = root.namedItem("Filter_Coeffs");

          QDomNode a1l = SettingsNode.namedItem("a1l");
          a1l.firstChild().setNodeValue(QString::number(coeff_lpf[3]));
          QDomNode a2l = SettingsNode.namedItem("a2l");
          a2l.firstChild().setNodeValue(QString::number(coeff_lpf[4]));
          QDomNode b0l = SettingsNode.namedItem("b0l");
          b0l.firstChild().setNodeValue(QString::number(coeff_lpf[0]));
          QDomNode b1l = SettingsNode.namedItem("b1l");
          b1l.firstChild().setNodeValue(QString::number(coeff_lpf[1]));
          QDomNode b2l = SettingsNode.namedItem("b2l");
          b2l.firstChild().setNodeValue(QString::number(coeff_lpf[2]));

          QDomNode a1h = SettingsNode.namedItem("a1h");
          a1h.firstChild().setNodeValue(QString::number(coeff_hpf[3]));
          QDomNode a2h = SettingsNode.namedItem("a2h");
          a2h.firstChild().setNodeValue(QString::number(coeff_hpf[4]));
          QDomNode b0h = SettingsNode.namedItem("b0h");
          b0h.firstChild().setNodeValue(QString::number(coeff_hpf[0]));
          QDomNode b1h = SettingsNode.namedItem("b1h");
          b1h.firstChild().setNodeValue(QString::number(coeff_hpf[1]));
          QDomNode b2h = SettingsNode.namedItem("b2h");
          b2h.firstChild().setNodeValue(QString::number(coeff_hpf[2]));

    }


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

void FilterSettings::loadNewConfigFile()
{
    QString configfilename = "config_filterSettiings_"+pLabel;
    QString xmlName = pLabel;

    QString xmlReadPath = QCoreApplication::applicationDirPath()+"/data/"+xmlName+".xml";
   // QString txtWritePath = ":/resources/"+configfilename+".txt";
    QString txtWritePath = QCoreApplication::applicationDirPath()+"/data/config_file/"+configfilename+".txt";

    QFile f(txtWritePath);
    if(!f.open(QFile::ReadOnly))
         {
             QMessageBox::information(0, "config file error", f.errorString());
         }
    else
         {
             QByteArray config = f.readAll();
             tetra_grip_api::send_long_register(STIM_LONG_REG_STIM_CONFIG_FILE, (size_t)config.length(), (uint8_t*)config.data());


         }
}
