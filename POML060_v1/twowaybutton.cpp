#include "twowaybutton.h"
#include "ui_twowaybutton.h"

TwoWayButton::TwoWayButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoWayButton)
{
    ui->setupUi(this);
}

TwoWayButton::~TwoWayButton()
{
    delete ui;
}
