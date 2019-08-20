#ifndef STAGEPROGRAM_H
#define STAGEPROGRAM_H

#include <QMainWindow>

namespace Ui {
class stageProgram;
}

class stageProgram : public QMainWindow
{
    Q_OBJECT

public:
    explicit stageProgram(QWidget *parent = nullptr);
    ~stageProgram();

private:
    Ui::stageProgram *ui;
};

#endif // STAGEPROGRAM_H
