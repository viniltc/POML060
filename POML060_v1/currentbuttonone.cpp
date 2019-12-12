#include "currentbuttonone.h"
#include "ui_currentbuttonone.h"

CurrentButtonOne::CurrentButtonOne(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentButtonOne)
{
    ui->setupUi(this);
}

CurrentButtonOne::~CurrentButtonOne()
{
    delete ui;
}
