#ifndef SHOPPINGMANAGER_H
#define SHOPPINGMANAGER_H

#include <QWidget>
#include "slidenavigation.h"
#include <QSqlDatabase>
#include <QSqlQuery>
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
#include"./server/server.h"

extern QString FLAG_CHARACTER;
namespace Ui {
class ShoppingManager;
}

class ShoppingManager : public QWidget
{
    Q_OBJECT

public:
    explicit ShoppingManager(QWidget *parent = nullptr);
    ~ShoppingManager();
    QSqlDatabase getDataBase();

    void error(QChar character,QString errmsg);
    Person* getPerson();
    bool getConnected();
    bool connectToDB();
    bool disConnect();
    void loadStyleSheet(const QString &styleSheetFile);
    Server* getTotalServer();
public slots:
    void changePage(qintptr index);
    void someoneLogin(QJsonObject obj);
//    void changeStyle(int index);

private:
    Ui::ShoppingManager *ui;
    void helpConnect();
    QSqlDatabase db;//此数据库仅在orderpage、productpage使用
    Server * TotalServer;
    QTcpSocket *m_socket;
//    QString server_IP;//服务端IP地址
//    qintptr server_port;//服务端端口
    Person *logined_user;
    bool isconnected;
//    void setServerIP(QString ip);
//    void setServerPort(qintptr port);
    void closeEvent(QCloseEvent *event);
    bool connectToDataBase(QString SQLkind,QString ip,int port,QString dbname,QString username,QString password);
};

#endif // SHOPPINGMANAGER_H
