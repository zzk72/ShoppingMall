#include "server.h"

Server::Server(QObject *parent):QObject(parent)
{
    qDebug()<<"Server";
    m_widget = dynamic_cast<QWidget *>(parent);//找爸爸
    sqlserver = new SQLServer("QMYSQL","localhost",3306,"shoppingmall","root","zzlk624802");
    sqlserver->connectToDataBase();

    tcpserver = new TcpServer(parent);//tcpserver和会话窗口相连
    if(tcpserver->listen(QHostAddress::Any,7722)){
        qDebug()<<"tcpserver->isListen()";
    }else{
        qDebug()<<"tcpserver->noListen()";
    }
    if(tcpserver->isListening()){
        qDebug()<<"tcpserver->isListening()";
    }else{
        qDebug()<<"tcpserver->noListening()";
    }
    handleserver = new HandleServer(sqlserver);

    connect(tcpserver,SIGNAL(signal_recvData(const QString&,const qintptr,const QByteArray)),
            handleserver,SLOT(handleRequest(const QString&,const qintptr, const QByteArray)));

    connect(handleserver,SIGNAL(signal_responeReady(const QByteArray,qintptr)),
            tcpserver,SLOT(sendResponse(const QByteArray ,qintptr)));
}
