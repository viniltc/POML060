#include "stageonemain.h"
#include "tetra_grip_api.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    tetra_grip_api();

    StageOneMain w;
    w.show();
    return a.exec();
}
