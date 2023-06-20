#include "homepage.h"
#include "ui_homepage.h"
#include<QJsonDocument>
#include<QJsonObject>
#include"home_mywidget.h"
HomePage::HomePage(ShoppingClient *parent) :
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
    CarouselImageWindow *p = new CarouselImageWindow();//添加轮播图
    p->addImage(":/images/picturewall/1.png");
    p->addImage(":/images/picturewall/2.png");
    p->addImage(":/images/picturewall/3.png");
    p->addImage(":/images/picturewall/4.png");
    p->addImage(":/images/picturewall/5.png");
    p->addImage(":/images/picturewall/6.png");
    p->startPlay();
    ui->lunboLayout->addWidget(p);
    pic_path= ":/images/users/123.jpg";
    QPixmap pixmap(pic_path);
    if(pixmap.isNull()){
        pixmap.load(pic_path+"picerror.jpg");
    }
    ui->pic_label->setPixmap(pixmap);
    pixmap.scaled(ui->pic_label->size(), Qt::KeepAspectRatio);
    ui->pic_label->setScaledContents(true);
    mep=new MePage();

}
void HomePage::MeP_show(){
  mep->show();
}
void HomePage:: slot_person_ready(Person* person){
    ui->name_lab->setText(client->getPerson()->name);
    mep->client=client;
    mep->getSomeone(person);
    connect(ui->call_MeP,&home_mywidget::signal_callMeP_click,this,&HomePage::MeP_show);

}
HomePage::~HomePage()
{
    delete ui;
}
