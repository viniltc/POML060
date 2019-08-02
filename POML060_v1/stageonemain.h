#ifndef STAGEONEMAIN_H
#define STAGEONEMAIN_H

#include <QMainWindow>

namespace Ui {
class StageOneMain;
}

class StageOneMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit StageOneMain(QWidget *parent = nullptr);
    ~StageOneMain();

private:
    Ui::StageOneMain *ui;
};

#endif // STAGEONEMAIN_H
