#ifndef STATETESTWINDOW_H
#define STATETESTWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include "tetra_grip_api.h"

namespace Ui {
class StateTestWindow;
}

class StateTestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StateTestWindow(QString, QWidget *parent = nullptr);
    ~StateTestWindow();
    void phaseEventHandler(STIM_GUI_TOPIC_T topic,uint8_t index, uint8_t reg, uint32_t value);
    void closeEvent (QCloseEvent *event);

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_loadConfig_clicked();

    void on_pushButton_logReset_clicked();

private:
    Ui::StateTestWindow *ui;
    QString pLabel;
    QString StyleSheetOn;
    QString StyleSheetOff;
};

#endif // STATETESTWINDOW_H
