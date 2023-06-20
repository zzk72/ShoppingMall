#include "productitem.h"
#include "ui_productitem.h"
#include<QtDebug>

ProductItem::ProductItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductItem)
{
    ui->setupUi(this);
    path_head=":/images/products/";
}

ProductItem::~ProductItem()
{
    delete ui;
}
void ProductItem:: setPath(QString path){
    QString pic_add=path_head+path;
    QPixmap pixmap(pic_add);
    if(pixmap.isNull()){
        pixmap.load(path_head+"picerror.jpg");
    }
    ui->pic_lab->setPixmap(pixmap);
    pixmap.scaled(ui->pic_lab->size(), Qt::KeepAspectRatio);
    ui->pic_lab->setScaledContents(true);
}

void ProductItem::setLabNumColor(bool bo)
{
    if(bo == false)
    {
        ui->lab_num->setStyleSheet("QLabel{color:rgb(255,61,61)}");
    }
    else {
        ui->lab_num->setStyleSheet("QLabel{color:rgb(150,150,150)}");
    }
}

void ProductItem::setLabNumText(QString str)
{
    ui->lab_num->setText(str);
}

void ProductItem::setLabNameText(QString str)
{
    ui->lab_name->setText(str);
}

void ProductItem::setLabrankingText(QString str)
{
    ui->lab_ranking->setText(str);
}

void ProductItem::setLabMessageText(QString str)
{
    ui->lab_message->setShowText(str);
}

void ProductItem::setId(QString id){
    int num = id.toInt();
    this->id = num;
    return;
}

void ProductItem::mouseDoubleClickEvent(QMouseEvent *event){
    emit signal_proitem_dblclick(id);
    return;
}

