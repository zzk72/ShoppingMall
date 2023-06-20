#include "loginwgt.h"
#include<QtDebug>
#include"./server/sqlserver.h"
#include<QJsonObject>
LoginWgt::LoginWgt(ShoppingManager *p) :
    ui(new Ui::LoginWgt)
{
    ui->setupUi(this);
    this->setStyleSheet("QLineEdit::hover{background-color:rgb(0, 170, 255);border:0.5px solid rgb(0, 255, 255);border-width:0.5px;}");
    client = p;
    QRegExp ps("^[A-Za-z0-9]+$");//由数字和26个英文字母组成的字符串
    QRegExp us("^[A-Za-z0-9\u4e00-\u9fa5]+$");
    QRegExpValidator *latitude1 = new QRegExpValidator(us, this);
    QRegExpValidator *latitude2 = new QRegExpValidator(ps, this);
    ui->line_user->setValidator(latitude1);
    ui->line_password->setValidator(latitude2);
    connect(ui->btn_login,SIGNAL(clicked()),this,SLOT(loginUser()));
    connect(this,SIGNAL(signal_login(QJsonObject)),client,SLOT(someoneLogin(QJsonObject)));
}

LoginWgt::~LoginWgt()
{
    delete ui;
}

void LoginWgt::paintEvent(QPaintEvent*)
{
    QPainter paint_win(this);
    QPixmap map_win;
    map_win.load(":/images/5.png");
    paint_win.drawPixmap(0,0,this->width(),this->height(),map_win);
}

void LoginWgt::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Return){
        ui->btn_login->click();
    }
}

void LoginWgt::loginUser(){

    client->connectToDB();
    if(!client->getConnected()){QMessageBox::warning(nullptr,"错误","LoginWgt::loginUser！");return;}
    QString _username = ui->line_user->text();
    QString _password = ui->line_password->text();
    if(_username != "" && _password != ""){
        QJsonObject obj;
        QString sql = QString("manager_name = '%1' AND manager_password = '%2'").arg(_username).arg(_password);
        qDebug()<<sql;
        obj.insert("want",QJsonValue("*"));
        obj.insert("isDistinct",QJsonValue("true"));
        obj.insert("restriction",QJsonValue(sql));

        QJsonArray result;
        SQLServer* tmp_sqlServer=client->getTotalServer()->sqlserver;
        tmp_sqlServer->selectSth("managers",obj,result);

        if(!result.isEmpty()){
            QJsonObject val = result[0].toObject();
            emit signal_login(val);
            this->close();
        }else{
            QMessageBox::warning(nullptr,"错误","用户不存在或密码错误！");
            return;
        }
    }
    if(_username == ""){
        QMessageBox::warning(nullptr,"错误","用户名为空！");
    }
    if(_password == ""){
        QMessageBox::warning(nullptr,"错误","密码为空！");
    }
    //qDebug()<<_password;
}


