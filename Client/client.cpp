#include "client.h"
#include "ui_client.h"


QString FLAG_CHARACTER = "1";//客户端标识
QString PIC_PATH = ":/images/";
ShoppingClient::ShoppingClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingClient)
{

    ui->setupUi(this);
    //添加并初始化选择栏
    ui->BottomNav->setBarRadious(5);
    ui->BottomNav->setItemRadious(5);
    ui->BottomNav->setItemStartColor(QColor(42, 176, 254));
    ui->BottomNav->setItemEndColor((QColor(37, 156, 225)));
    ui->BottomNav->addItem(tr("首页"));
    ui->BottomNav->addItem(tr("商城"));
    ui->BottomNav->addItem(tr("购物车"));
    ui->BottomNav->addItem(tr("历史记录"));
    ui->BottomNav->addItem(tr("聊天"));
    ui->BottomNav->setOrientation(Qt::Vertical);

    ui->BottomNav->setEnableKeyMove(true);//允许键盘操作
    ui->BottomNav->moveTo(0);//默认选择第一项
    ui->PageStack->setCurrentIndex(0);//默认第一页
    m_socket = new QTcpSocket();

    //设计的ui自定义控件
    ui->HomeP->client = this;
    ui->ShopP->client = this;
    ui->CartP->client = this;
    ui->OrderP->client = this;
    ui->ChatP->client=this;

    helpConnect();//将选择栏各按钮与qstackwidget中的各页connect,将各页之间相应绑定
    isconnected = false;
    isData=false;
}

ShoppingClient::~ShoppingClient()
{
    delete ui;
}

void ShoppingClient::closeEvent(QCloseEvent *event){
    qDebug()<<"客户端窗口关闭";
    disConnect();
    return QWidget::closeEvent(event);
}

void ShoppingClient::helpConnect(){
    connect(ui->BottomNav,SIGNAL(itemClicked(qintptr,QString)),this,SLOT(changePage(qintptr)));
    connect(ui->ShopP,SIGNAL(signal_addToCart(Product*)),ui->CartP,SLOT(addToCart(Product*)));
    connect(ui->CartP,SIGNAL(signal_updateUserMoney(int)),ui->HomeP->mep,SLOT(updateUserMoney(int)));
    connect(this,SIGNAL(signal_someonelogin(Person*)),ui->ChatP,SLOT(slot_person_ready(Person*)));
    connect(this,SIGNAL(signal_someonelogin(Person*)),ui->HomeP,SLOT(slot_person_ready(Person*)));

}

void ShoppingClient::changePage(qintptr index){
    qintptr pagecount = ui->PageStack->count();
    if(index >= pagecount){
        ui->PageStack->setCurrentIndex(0);
    }
    else {
        ui->PageStack->setCurrentIndex(index);
    }
    return;
}

void ShoppingClient::setServerIP(QString ip){
    server_IP = ip;
    return;
}

void ShoppingClient::setServerPort(qintptr port){
    server_port = port;
    return;
}

bool ShoppingClient::connectTo(){
    setServerIP("127.0.0.1");
    setServerPort(7722);//

    //qDebug()<<"ShoppingClient::connectTo= 7722";

    m_socket->connectToHost(server_IP,server_port);
    if(!m_socket->waitForConnected(3000)){
        qDebug()<<"连接服务器失败！";
        QMessageBox::warning(this,tr("张珍奎的通信"),tr("连接服务端失败!"),QMessageBox::Yes);
        return false;
    }
    else {
        qDebug()<<"连接服务器成功！";
        //sendCHTTPMsg("10010",QJsonObject());
        isconnected = true;
    }
    return true;
}

bool ShoppingClient::disConnect(){
    m_socket->disconnectFromHost();
    if(m_socket->state() == QTcpSocket::UnconnectedState){
        qDebug()<<"客户端断开连接成功！";
        isconnected = false;
    }
    return true;
}

//将 CHTTP和body转化为QJsonObject->QJsonDocument->QByteArray发送
//返回QByteArray型数据请求结果
QByteArray ShoppingClient::sendCHTTPMsg(QString CHTTP, QJsonObject jsonobj){//CHTTP为自定协议码
    QJsonObject content;
    content.insert("head",QJsonValue(CHTTP));//设置报文头部
    content.insert("body",QJsonValue(jsonobj));//设置报文内容
    QJsonDocument document;
    document.setObject(content);
    QByteArray arr = document.toJson();//类型转换
    this->m_socket->write(arr);
    if(m_socket->waitForReadyRead(5000)){
        if(!isData){
           Data = m_socket->readAll();
            qDebug()<<"客户端已成功发送消息并收到响应";
            return Data;
        }else{
            isData=false;
            return Data;
        }

    }else{
        QMessageBox::warning(nullptr,tr("错误！"),tr(" ShoppingClient::sendCHTTP出错！服务器未及时响应"));
        return QByteArray();
    }
}
QJsonArray ShoppingClient::parseResponse(QByteArray data){
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) {  // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("head")) {
                QString head = object.value("head").toString();
                if(object.contains("result")){
                    QJsonValue result = object.value("result");
                    if(result.isArray()){
                        return result.toArray();
                    }
                    qDebug()<<"result:"<<object.value("result");
                }
                else{QMessageBox::warning(nullptr,tr("错误"),tr("接受数据错误！"));}
            }
        }
    }
    return QJsonArray();
}

QString ShoppingClient::parseHead(QByteArray data){//解析报文头部
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) {  // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("head")) {
                QString head = object.value("head").toString();
                if(head[0] == "1"){
                    return "1";
                }
                else if(head[0] == "2"){
                    QString errmsg = object.value("error").toString();
                    return "2"+errmsg;
                }
                else if(head[0] == "3"){
                    QString errmsg = object.value("error").toString();
                    return "3"+errmsg;
                }
            }
        }
    }
    return "";
}

void ShoppingClient::error(QChar character, QString errmsg){
    if(character == "2"){
       qDebug()<<errmsg;
       QMessageBox::warning(nullptr,"客户端错误","客户端错误:"+errmsg);
    }
    else if(character == "3"){
        qDebug()<<errmsg;
       QMessageBox::warning(nullptr,"服务端错误","服务端错误:"+errmsg);
    }
}

void ShoppingClient::someoneLogin(QJsonObject obj){
    this->show();
    this->logined_user = new Person(obj);
    signal_someonelogin(this->logined_user);
    qDebug()<<obj<<"Login!";
}

Person* ShoppingClient::getPerson(){
    return this->logined_user;
}
QTcpSocket* ShoppingClient::getSocket(){
    return m_socket;
}

bool ShoppingClient::getConnected(){
    return  this->isconnected;
}
