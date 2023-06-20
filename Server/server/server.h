#ifndef SERVER_H
#define SERVER_H
#include "sqlserver.h"
#include "tcpserver.h"
#include"udpserver.h"
#include"handleserver.h"
#include<QObject>

class Server:public QObject
{
public:
    Server(QObject *parent);
    TcpServer *tcpserver;
    SQLServer *sqlserver;
    UdpServer *udpserver;
    HandleServer *handleserver;

private:
    QWidget *m_widget;//指向主窗口
};

#endif // SERVER_H
