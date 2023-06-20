#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include<QWidget>
#include "tcpserverthread.h"


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = Q_NULLPTR);
    ~TcpServer();
    QList<TcpServerThread*> threadlist;
    QList<qintptr> m_socketList;

private:
    void incomingConnection(qintptr sockDesc);

private slots:
    void clientDisconnected(qintptr sockDesc);
    void recvData(const QString&,const qintptr, const QByteArray);
    void sendResponse(const QByteArray,qintptr port);

private:
    QWidget *m_widget;

signals:
    void signal_newConnection(qintptr port);
    void signal_disConnection(qintptr port);
    void signal_recvData(const QString&,const qintptr, const QByteArray);
};

#endif // SERVER_H
