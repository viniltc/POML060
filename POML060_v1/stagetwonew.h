#ifndef STAGETWONEW_H
#define STAGETWONEW_H

#include <QMainWindow>

namespace Ui {
class StageTwoNew;
}

class StageTwoNew : public QMainWindow
{
    Q_OBJECT

public:
    explicit StageTwoNew(QWidget *parent = nullptr);
    ~StageTwoNew();

private:
    Ui::StageTwoNew *ui;
};

#endif // STAGETWONEW_H
