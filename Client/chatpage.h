#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include<QTcpSocket>
#include<QTcpServer>
#include<QJsonObject>
#include<QJsonDocument>
#include"client.h"
#include"user.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    ShoppingClient* client;


private:
    Ui::ChatPage *ui;
    QTcpSocket* m_socket;
    qintptr port;
    bool appendMsg(QJsonObject obj);
private slots:
    void receiveMessage();
    void sendMessage();
    void slot_person_ready(Person* );
  //  void setSocket();
};

#endif // CHATPAGE_H
