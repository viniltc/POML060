#ifndef STAGETWONEW_H
#define STAGETWONEW_H

#include <QMainWindow>
//#include "mainwindow.h"



namespace Ui {
class StageTwoNew;
}

class StageTwoNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit StageTwoNew(QWidget *parent = nullptr);
    ~StageTwoNew();
 //  MainWindow *modify_window;

signals:

    void tableUpdate();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_tabWidget_currentChanged(int index);

  //  void editdetails(QString name, QString sname, QString note);

private:
    Ui::StageTwoNew *ui;


};

#endif // STAGETWONEW_H
