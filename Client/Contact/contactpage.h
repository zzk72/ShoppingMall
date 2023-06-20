#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

#include <QWidget>
#include<QUdpSocket>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QJsonObject>
#include"shoppingclient.h"

//client端收发信息
namespace Ui {
class ContactPage;
}

class ContactPage : public QWidget
{
    Q_OBJECT

public:

    explicit ContactPage(QWidget *parent = nullptr);
    ~ContactPage();
    ShoppingClient *client;
    QString ManagerName;
    bool isLocked;
private:

    Ui::ContactPage *ui;
    QUdpSocket *udpsocket;
    qintptr port;
    bool appendMsg(QJsonObject obj);
private slots:
   // void receiveMessage(QByteArray msg);
    void receiveMessage();
    void sendMessage();
};

#endif // CONTACTPAGE_H
