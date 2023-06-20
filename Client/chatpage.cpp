#include "chatpage.h"
#include "ui_chatpage.h"
#include<QHostAddress>
ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);



}
void ChatPage::slot_person_ready(Person* person){
    port=7721;
   m_socket=new QTcpSocket(this);
   m_socket->connectToHost(QHostAddress::LocalHost,port);
   connect(ui->btnSend,&QPushButton::clicked,this,&ChatPage::sendMessage);
   connect(m_socket,&QTcpSocket::readyRead,this,&ChatPage::receiveMessage);

   //发送头部文件
   QJsonObject obj;
   obj.insert("id",QString(person->getId()));
   obj.insert("data",QString("head"));
   obj.insert("name",person->name);

   QJsonDocument document(obj);
   m_socket->write(document.toJson());

}
ChatPage::~ChatPage()
{
    delete ui;
}
void ChatPage::sendMessage(){
    const QByteArray send_data=ui->textSend->toPlainText().toUtf8();
    QJsonObject obj;

    //数据为空就返回
    if(send_data.isEmpty())
        return;

    obj.insert("id",QString(client->getPerson()->getId()));
    obj.insert("data",QString(send_data));
    obj.insert("name",client->getPerson()->name);

    QJsonDocument document(obj);
    m_socket->write(document.toJson());

    ui->textRecv->setTextColor(Qt::blue);
    ui->textRecv->append("send>"+send_data+"\n");
    ui->textSend->clear();
    ui->textRecv->setTextColor(Qt::black);
}
void ChatPage::receiveMessage(){
    const QByteArray recv_data=(m_socket->readAll());
    QString id,name,msg;
    if(recv_data.isEmpty()){
        return;
    }
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(recv_data, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) {  // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if(object.contains("name")){
                name=object.value("name").toString();
            }
            if(object.contains("id")){
                id=object.value("id").toString();
            }
            if(object.contains("data")){
                msg=object.value("data").toString();
            }
         }
      }


    ui->textRecv->append(QString("recev from %1>\n%2\n").arg(name).arg(msg));
}


