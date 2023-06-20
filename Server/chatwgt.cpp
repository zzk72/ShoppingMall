#include "chatwgt.h"
#include<QHostAddress>
ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    port=7721;
   m_server=new QTcpServer(this);
   m_server->listen(QHostAddress::Any,port);
   connect(m_server,&QTcpServer::newConnection,this,&ChatPage::setSocket);
    connect(ui->btnSend,&QPushButton::clicked,this,&ChatPage::sendMessage);
    connect(ui->client_select,SIGNAL(activated(int)),this,SLOT(chat_change(int)));
    isCached = false;

    ui->btn_del->setIcon(QIcon(":/images/del.png"));
    ui->btn_add->setIcon(QIcon(":/images/add.png"));
}

ChatPage::~ChatPage()
{
    delete ui;
}
void ChatPage::chat_change(int index){
    QString name=ui->client_select->currentText();
    ui->textRecv->clear();
    QVector<QString>* tmp_vector=chat_map.value(name);
    for(int i=0;i<tmp_vector->size();i++){
        QString msg=(*tmp_vector)[i];
        if(msg[0]=='r'){
            msg[0]=':';
            ui->textRecv->append(QString("recev from %1>\n%2\n").arg(name).arg(msg));
        }else if(msg[0]=='s'){
            msg[0]='>';
             ui->textRecv->append("send"+msg+"\n");
        }
    }
}
void ChatPage::sendMessage(){
    const QByteArray send_data=ui->textSend->toPlainText().toUtf8();
    //数据为空就返回
    if(send_data.isEmpty())
        return;
    QString name=ui->client_select->currentText();
    QTcpSocket* socket =client_map.value(name);
/*


*/
    if(!socket){
        qDebug()<<"没有连接";
        return;
    }


    QJsonObject obj;
    obj.insert("id",QString(manager->getPerson()->getId()));
    obj.insert("data",QString(send_data));
    obj.insert("name",manager->getPerson()->name);

    QJsonDocument document(obj);
    socket->write(document.toJson());
    chat_map.value(name)->push_back(QString("s"+send_data));//加入
    ui->textRecv->setTextColor(Qt::blue);
    ui->textRecv->append("send> "+send_data+"\n");
    ui->textSend->clear();
    ui->textRecv->setTextColor(Qt::black);
}
void ChatPage::receiveMessage(QTcpSocket* socket){
    const QByteArray recv_data=(socket->readAll());
    QString id,name,data;
    if(recv_data.isEmpty()){
        return;
    }
    praseMessage(recv_data,name,id,data);
   if(data=="head")
   {
      tmp_id=id;
      tmp_name=name;
   }
   else{
       chat_map.value(name)->push_back("r"+data);
       if(ui->client_select->currentText()==name){
           ui->textRecv->append(QString("recev from: %1>\n%2\n").arg(name).arg(data));
       }else{
           ui->textRecv->append(QString("[系统]收到来自%1的消息，请及时查看！").arg(name));
       }

   }
}
bool ChatPage:: praseMessage(QByteArray msg,QString& name,QString& id,QString& data){
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(msg, &jsonError);  // 转化为 JSON 文档
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
                data=object.value("data").toString();
            }
         }
      }else{data= "数据解析错误";return false;}
return true;
}
void ChatPage::setSocket(){
    qDebug()<<"ChatPage::setSocket";
    QTcpSocket* socket;
    while(m_server->hasPendingConnections()){
        socket=m_server->nextPendingConnection();
        connect(socket,&QTcpSocket::readyRead,[=]{
            receiveMessage(socket);
        });
       QString name=tmp_name;

        qDebug()<<"hasPendingConnections";
        if(socket->waitForReadyRead(2000)){
            receiveMessage(socket);
            client_map.insert(tmp_name,socket);
            QVector<QString>* tmp_vector=new QVector<QString>(1000);
            chat_map.insert(tmp_name,tmp_vector);
            ui->client_select->addItem(tmp_name);
           int count=ui->client_select->count();
            select_map.insert(tmp_name,count);

        }
        connect(socket,&QTcpSocket::disconnected,[=]{
            socket->deleteLater();
          //  clientList.removeOne(m_socket);
            ui->textRecv->append(QString("[%1:%2] Soket Disonnected")
                                 .arg(socket->peerAddress().toString())
                                 .arg(socket->peerPort()));
         ui->client_select->removeItem(select_map[name]);
         select_map.remove(name);
         chat_map.remove(name);
         client_map.remove(name);
        });
    }
qDebug()<<client_map;
}


//-表管理--------------------------------------
void ChatPage::showEvent(QShowEvent *event){
    if(!isCached){
    model = new QSqlTableModel(nullptr,manager->getDataBase());
    model->setTable("users");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setModel(model);
    //使其不可编辑
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //隔行变色
    ui->tableView->setAlternatingRowColors(true);
    //根据内容自动调整列宽
    ui->tableView->setSortingEnabled(true);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(100);
    ui->tableView->horizontalHeader()->setMaximumSectionSize(200);

    connect(ui->btn_back,SIGNAL(clicked()),this,SLOT(backChange()));
    connect(ui->btn_search,SIGNAL(clicked()),this,SLOT(searchclient()));
    connect(ui->btn_submit,SIGNAL(clicked()),this,SLOT(submitChange()));
    connect(ui->btn_add,SIGNAL(clicked()),this,SLOT(addclient()));
    connect(ui->btn_del,SIGNAL(clicked()),this,SLOT(delclient()));
    isCached = true;
    }
}

void ChatPage::submitChange(){
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) {
       model->database().commit(); //提交
    } else {
       model->database().rollback(); //回滚
       QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(model->lastError().text()));
    }
}

void ChatPage::backChange(){
    model->revertAll();
}

void ChatPage::addclient(){
    int rowNum = model->rowCount(); //获得表的行数
    model->insertRow(rowNum); //添加一行
    //model->submitAll(); //可以直接提交
}

void ChatPage::delclient(){
    //获取选中的行
    int curRow = ui->tableView->currentIndex().row();

    //删除该行
    model->removeRow(curRow);

    int ok = QMessageBox::warning(this,tr("提醒"),tr("你确定""下线该产品吗？"),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::No)
        {
           model->revertAll(); //如果不删除，则撤销
        }
        else model->submitAll(); //否则提交，在数据库中删除该行
}

void ChatPage::searchclient(){
    QString restriction = ui->lineEdit->text();
    //根据姓名进行筛选
    model->setFilter(QString("user_name='%1'").arg(restriction));
    //显示结果
    if(model->select())qDebug()<<"success1!";
}
