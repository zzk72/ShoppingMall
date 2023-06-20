#ifndef SHOPPINGCLIENT_H
#define SHOPPINGCLIENT_H

#include <QWidget>
#include "slidenavigation.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include<QMessageBox>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include <QMouseEvent>
#include <QPainter>
#include<QJsonParseError>
#include"user.h"
extern QString FLAG_CHARACTER;
extern QString PIC_PATH;
namespace Ui {
class ShoppingClient;
}

class ShoppingClient : public QWidget
{
    Q_OBJECT

public:
    explicit ShoppingClient(QWidget *parent = nullptr);
    ~ShoppingClient();
    void setServerIP(QString ip);
    void setServerPort(qintptr port);
    bool connectTo();//连接服务器
    bool disConnect();
    bool getConnected();

    void closeEvent(QCloseEvent *event);
    QByteArray sendCHTTPMsg(QString CHTTP,QJsonObject jsonobj);//转化并发送报文，返回查询结果
    QJsonArray parseResponse(QByteArray data);//解析查询返回的报文
    QString parseHead(QByteArray data);//解析报文头部
    void error(QChar character,QString errmsg);
    QTcpSocket* getSocket();
    Person* getPerson();


    QTcpSocket *m_socket;
private:
    Ui::ShoppingClient *ui;
    void helpConnect();//绑定信号和槽
    QString server_IP;//服务端IP地址
    qintptr server_port;//服务端端口
    Person *logined_user;
    bool isconnected;
    bool isData;
    QByteArray Data;
public slots:
    void changePage(qintptr index);
private slots:
    void someoneLogin(QJsonObject);
signals:
    void signal_someonelogin(Person*);
    void signal_client_ready(ShoppingClient*);
};

#endif // SHOPPINGCLIENT_H
