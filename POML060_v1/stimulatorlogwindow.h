#ifndef STIMULATORLOGWINDOW_H
#define STIMULATORLOGWINDOW_H

#include <QWidget>


namespace Ui {
class StimulatorLogWindow;
}

class StimulatorLogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StimulatorLogWindow(QString ,QWidget *parent = nullptr);
    ~StimulatorLogWindow();
    QString pLabel;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::StimulatorLogWindow *ui;
    void closeEvent (QCloseEvent *event);
    QString getLastWordInFilePath(const QString& filePath);
    QString formatTime(int seconds);
    QString formatTimeFromMilliseconds(int milliseconds);
    QString configFileName;
};

#endif // STIMULATORLOGWINDOW_H
