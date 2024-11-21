#ifndef IMUDIALOG_H
#define IMUDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class IMUDialog;
}

class IMUDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IMUDialog(QString, QWidget *parent = nullptr);
    ~IMUDialog();
    QString pLabel;

private slots:
    void on_checkBox_stateChanged(int arg1);
    void saveToXMLFile();

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);



    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_radioButton_toggled(bool checked);



    void on_radioButton_2_toggled(bool checked);

    void on_pushButton_2_clicked();

signals:
    void radioButtonChanged(QString buttonText);
    void checkBoxChanged(bool status);

private:
    Ui::IMUDialog *ui;
    int elevationAngle = 30;
    int armFlexionAngle = 30;
    int IMUTimeout = 300;
};

#endif // IMUDIALOG_H
