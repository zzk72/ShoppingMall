#ifndef HANDLESERVER_H
#define HANDLESERVER_H

#include <QObject>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonParseError>
#include"sqlserver.h"
#include"tcpserver.h"
//信息总处理

class HandleServer:public QObject
{
    Q_OBJECT
public:
    HandleServer(SQLServer*);
    void jsonResReady(QString head,QJsonArray res,qintptr port,QString errmsg="");
    QString getRandomOrderNum();
private:
    SQLServer *sql;
    TcpServer *tcp;
    //
    void handleUserLogin(QJsonObject body,qintptr port);
    void handleManagerLogin(QJsonObject body,qintptr port);
    void handleRegister(QJsonObject body,qintptr port);
    void handleSearchProduct(QJsonObject body,qintptr port);
    void handleAddCart(QJsonObject body,qintptr port);
    void handleDelCart(QJsonObject body,qintptr port);
    void handleUpdateCart(QJsonObject body,qintptr port);
    void handleSearchCart(QJsonObject body,qintptr port);
    void handleBuySth(QJsonObject body,qintptr port);
    bool createOrderItems(QJsonArray wannabuy,QJsonObject map,QString ordernum);
    void handleSearchOrder(QJsonObject body,qintptr port);
    void handleSearchOrderItems(QJsonObject body,qintptr port);

    void k_handleChat(QJsonObject body,qintptr port);
public slots:
    void handleRequest(const QString&,const qintptr, const QByteArray);//分发信息到相应处理函数 控制器
signals:
    void signal_responeReady(const QByteArray,qintptr);
};

#endif // HANDLESERVER_H
