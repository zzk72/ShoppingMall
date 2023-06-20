#include "home_mywidget.h"
#include "ui_home_mywidget.h"

home_mywidget::home_mywidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::home_mywidget)
{
    ui->setupUi(this);
}

home_mywidget::~home_mywidget()
{
    delete ui;
}
void home_mywidget::mouseDoubleClickEvent(QMouseEvent *event){
    emit signal_callMeP_click();
    return;
}
