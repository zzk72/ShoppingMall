#include "shoppage.h"
#include "ui_shoppage.h"

ShopPage::ShopPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopPage)
{
    ui->setupUi(this);
    isCached = false;
    m_stackedwidget = new QStackedWidget();
    ui->horizontalLayout->addWidget(m_stackedwidget);
    ui->list_classification->setSpacing(1);

    connect(ui->list_classification,SIGNAL(currentRowChanged(int)),this,SLOT(updateOneClass(int)));
    connect(this,SIGNAL(signal_refresh(int)),this,SLOT(updateOneClass(int)));
    connect(ui->btn_search,SIGNAL(clicked()),this,SLOT(searchProduct()));

}

ShopPage::~ShopPage()
{
    delete ui;
}

void ShopPage::showEvent(QShowEvent *event){
    if(!isCached){
    initClassification();
    isCached = true;
    }
    return QWidget::showEvent(event);
}

void ShopPage::initClassification(){
    if(client){
        QString FLAG_INSKIND = "02";//商品操作属于02类
        QString FLAG_INS = "04";//执行的是查询操作
        QJsonObject obj;
        obj.insert("want",QJsonValue(QString("pro_classification")));
        obj.insert("isDistinct",QJsonValue(QString("true")));
        //qDebug()<<"ShopPage::initClassification";
        QByteArray data = client->sendCHTTPMsg(FLAG_CHARACTER+FLAG_INSKIND+FLAG_INS,obj);
        //qDebug()<<data;
        QString flag = client->parseHead(data);
        if(flag[0] != "1"){client->error(flag[0],flag.mid(1));return;}
        QJsonArray result = client->parseResponse(data);
        if(!result.isEmpty()){
            for(int i = 0; i< result.size(); i++){
                QString str = result.at(i).toObject().value("pro_classification").toString();
                QListWidgetItem *p = new QListWidgetItem(str);//侧栏单项
                ui->list_classification->addItem(p);//ui->list_classification 为侧栏
                QListWidget *tmp = new QListWidget();
                m_stackedwidget->addWidget(tmp);//预先设置对应数目的stackpage
            }
            //qDebug()<<"result:"<<result.size();
            stack_isCached.push_back(false);
            stack_isCached.resize(result.size());
            stack_isCached.fill(false);//用于标记是否为当前页
            ui->list_classification->setCurrentRow(0);//一开始展示第一页
        }

    }
    else{
        QMessageBox::warning(nullptr,tr("错误"),tr("客户端有误！"));
    }
}

void ShopPage::updateOneClass(int currow){//change productpage
    if(client && !stack_isCached.at(currow)){//更新并展示currow类别页
        QString FLAG_INSKIND = "02";//商品操作属于02类
        QString FLAG_INS = "04";//执行的是查询操作
        QJsonObject obj;
        QString wantclass = ui->list_classification->item(currow)->text();
        obj.insert("want",QJsonValue(QString("*")));
        obj.insert("isDistinct",QJsonValue(QString("false")));
        obj.insert("restriction",QJsonValue(QString("pro_classification='%1'").arg(wantclass)));
        QByteArray data = client->sendCHTTPMsg(FLAG_CHARACTER+FLAG_INSKIND+FLAG_INS,obj);
        qDebug()<<"ShopPage::updateOneClas";
        QString flag = client->parseHead(data);
        if(flag[0] != "1"){client->error(flag[0],flag.mid(1));return;}
        QJsonArray result = client->parseResponse(data);
        //qDebug()<<"ShopPage::updateOneClass: data="<<result;
        QListWidget *newList = new QListWidget();

        newList->setStyleSheet(//设置ui列表的样式表
                                      "QListWidget::Item{ background:white;height:100px;border-radius: 10px;background-color: rgb(255, 255, 255);}"
                                      "QListWidget::Item:hover{background:rgb(246,246,247);}"
                                     );
        newList->setSpacing(5);

        if(!result.isEmpty()){//
            for(int i = 0;i < result.size();i++){
                QJsonObject p = result[i].toObject();
                Product *newproduct = new Product(p);//生成新的产品对象
                if(productlist.contains(newproduct->id)){free(productlist.value(newproduct->id));}
                productlist.insert(newproduct->id,newproduct);
                //加入ui列表
                QListWidgetItem * newWidget = new QListWidgetItem(newList);//设置ListItem
                newWidget->setSizeHint(QSize(300,200));
                ProductItem * addNew = new ProductItem();//设置自定义Item
                connect(addNew,SIGNAL(signal_proitem_dblclick(int)),this,SLOT(showProduct(int)));//根据点击展示商品详细信息
                //设置自定义Item
                addNew->setLabNumColor(false);  //设置红色
                addNew->setLabNumText("￥"+p.value("pro_price").toString());
                addNew->setLabNameText(p.value("pro_name").toString());
                addNew->setLabMessageText(p.value("pro_about").toString());
                addNew->setLabrankingText("销售量:"+p.value("pro_sales").toString());
                addNew->setId(p.value("pro_id").toString());
                addNew->setPath(p.value("pro_pictureaddress").toString());
                newList->setItemWidget(newWidget,addNew);//加入列表
            }
        }
        QWidget *tmp = m_stackedwidget->widget(currow);
        m_stackedwidget->removeWidget(tmp);//移除当页旧数据，以更新
        m_stackedwidget->insertWidget(currow,newList);//添加新的类别页到相应位置
        m_stackedwidget->setCurrentIndex(currow);
       // qDebug()<<"updateoneclass!"<<wantclass<<currow;
        stack_isCached[currow] = true;

    }
    else if(client && stack_isCached.at(currow)){
        m_stackedwidget->setCurrentIndex(currow);
    }
    else{
        QMessageBox::warning(nullptr,tr("错误"),tr("客户端有误！"));
    }
}

void ShopPage::showProduct(int id){
    Product *p = productlist.value(id);
    ProductPage *page = new ProductPage(p);
    connect(page,SIGNAL(signal_addToCart(Product*)),this,SLOT(addToCart(Product*)));
    page->show();
    return;
}

void ShopPage::addToCart(Product *Product){
    emit signal_addToCart(Product);
}

void ShopPage::contextMenuEvent(QContextMenuEvent *event)
{
        QAction *actionshuffle = new QAction(tr("刷新"),this);
        this->addAction(actionshuffle);
        QMenu contextMenu;
        contextMenu.addAction(actionshuffle);
        QAction *selectedAction = contextMenu.exec(QCursor::pos());
        if(selectedAction == actionshuffle){
            int _curindex = m_stackedwidget->currentIndex();
            stack_isCached[_curindex] = false;
            //qDebug()<<productlist;
            emit signal_refresh(_curindex);
        }
}

void ShopPage::searchProduct(){
    QString context = ui->lineEdit->text();
    if(context.trimmed() == ""){
        return;
    }
    bool flag = false;
    QRegExp *exp = new QRegExp(QString("\\w*%1\\w*").arg(context),Qt::CaseInsensitive);
    for(int i = 0;i < productlist.keys().size(); i++){
        Product *p = productlist.value(productlist.keys().at(i));
        if(exp->exactMatch(p->name)){
            ProductPage *page = new ProductPage(p);
            connect(page,SIGNAL(signal_addToCart(Product*)),this,SLOT(addToCart(Product*)));
            page->show();
            flag = true;
        }
    }
    if(!flag)QMessageBox::information(nullptr,tr("提示"),tr("未找到相应商品！"));
}
