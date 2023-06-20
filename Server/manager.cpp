#include "manager.h"
#include<QDebug>
QString FLAG_CHARACTER = "2";//服务端标识
ShoppingManager::ShoppingManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingManager)
{
    ui->setupUi(this);

    ui->SideNav->setBarRadious(5);
    ui->SideNav->setItemRadious(5);
    ui->SideNav->setItemStartColor(QColor(191, 65, 249));
    ui->SideNav->setItemEndColor((QColor(187, 83, 217)));
    ui->SideNav->setOrientation(Qt::Horizontal);
    ui->SideNav->addItem(tr("商品管理"));
    ui->SideNav->addItem(tr("订单管理"));
    ui->SideNav->addItem(tr("客户服务"));
    ui->SideNav->setEnableKeyMove(true);//允许键盘操作
    ui->SideNav->moveTo(0);//默认选择第一项
    ui->PageStack->setCurrentIndex(0);//默认第一页

    ui->ProductP->manager = this;
    ui->OrderP->manager = this;
    ui->ChatP->manager=this;
    helpConnect();

    isconnected = false;


    TotalServer = new Server(this);

}

ShoppingManager::~ShoppingManager()
{
    delete ui;
}

void ShoppingManager::helpConnect(){
    connect(ui->SideNav,SIGNAL(itemClicked(qintptr,QString)),this,SLOT(changePage(qintptr)));
}

void ShoppingManager::changePage(qintptr index){
    qintptr pagecount = ui->PageStack->count();
    if(index >= pagecount){
        ui->PageStack->setCurrentIndex(0);
    }
    else {
        ui->PageStack->setCurrentIndex(index);
    }
    //qDebug()<<ui->PageStack->currentIndex();
    return;
}

bool ShoppingManager::connectToDataBase(QString SQLkind,QString ip,int port,QString dbname,QString username,QString password){
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        db = QSqlDatabase::addDatabase(SQLkind);
    }
    db.setHostName(ip);
    db.setPort(port);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);
    if(db.open()){
        qDebug()<<"连接数据库成功！";
        //emit signal_connectDatabase(true);
        return true;
    }
    else {
        qDebug()<<"连接数据库失败！";

        //emit signal_connectDatabase(false);
    }
    return false;
}

QSqlDatabase ShoppingManager::getDataBase(){
    return this->db;
}
Server* ShoppingManager::getTotalServer(){
    return this->TotalServer;
}
void ShoppingManager::closeEvent(QCloseEvent *event){
    qDebug()<<"业务端窗口关闭";
  //  disConnect();
    return QWidget::closeEvent(event);
}



bool ShoppingManager::connectToDB(){
    this->connectToDataBase("QMYSQL","127.0.0.1",3306,"ShoppingMall","root","zzlk624802");
    isconnected = true;
    return true;
}


void ShoppingManager::error(QChar character, QString errmsg){
    if(character == "2"){
       qDebug()<<errmsg;
       QMessageBox::warning(nullptr,"客户端错误","客户端错误:"+errmsg);
    }
    else if(character == "3"){
        qDebug()<<errmsg;
       QMessageBox::warning(nullptr,"服务端错误","服务端错误:"+errmsg);
    }
}

void ShoppingManager::someoneLogin(QJsonObject obj){
    this->show();
    this->logined_user = new Person(obj);
    qDebug()<<obj<<"Login!";
}

Person* ShoppingManager::getPerson(){
    return this->logined_user;
}

bool ShoppingManager::getConnected(){
    return this->isconnected;
}

void ShoppingManager::loadStyleSheet(const QString &styleSheetFile){
        QFile file(styleSheetFile);
        file.open(QFile::ReadOnly);
        if (file.isOpen()){
            QString styleSheet = this->styleSheet();
            styleSheet += QLatin1String(file.readAll());//读取样式表文件
            this->setStyleSheet(styleSheet);//把文件内容传参
            file.close();
        }else{
            QMessageBox::information(this,"tip","cannot find qss file");
        }
}

