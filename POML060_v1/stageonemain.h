#ifndef STAGEONEMAIN_H
#define STAGEONEMAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class StageOneMain; }
QT_END_NAMESPACE

class StageOneMain : public QMainWindow
{
    Q_OBJECT

public:
    StageOneMain(QWidget *parent = nullptr);
    ~StageOneMain();

private:
    Ui::StageOneMain *ui;
};
#endif // STAGEONEMAIN_H
