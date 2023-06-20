#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include<QTcpSocket>
#include<QTcpServer>
#include<QJsonObject>
#include<QJsonDocument>
#include<manager.h>

#include <QSqlTableModel>
#include<QMessageBox>
#include<QSqlError>
#include<QSqlRelationalDelegate>

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT
/*聊天模块*/
public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    ShoppingManager* manager;
private:
    Ui::ChatPage *ui;
    QTcpSocket* m_socket;
    QMap<QString,QTcpSocket*> client_map;
    QMap<QString,QVector<QString>* >chat_map;
    QMap<QString,int> select_map;
    QString tmp_id;
    QString tmp_name;
    QTcpServer* m_server;
    qintptr port;
    bool praseMessage(QByteArray msg,QString& name,QString& id,QString& data);
    bool appendMsg(QJsonObject obj);
private slots:
    void receiveMessage(QTcpSocket*);
    void sendMessage();
    void setSocket();
    void chat_change(int);
/*client sql模块*/
public:
    void showEvent(QShowEvent *event);

private:
    QSqlTableModel *model;
    bool isCached;
private slots:
    void submitChange();
    void backChange();
    void searchclient();
    void addclient();
    void delclient();
};

#endif // CHATPAGE_H
