#include "ordermanagewgt.h"
OrderHandlePage::OrderHandlePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderHandlePage)
{
    ui->setupUi(this);
    isCached = false;
}

OrderHandlePage::~OrderHandlePage()
{
    delete ui;
}

void OrderHandlePage::showEvent(QShowEvent *event){
    if(!isCached){
    model = new QSqlRelationalTableModel(nullptr,manager->getDataBase());
    model->setTable("orders");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);//只有提交才保存
    model->select(); //选取整个表的所有行

    ui->tableView->setModel(model);
    //隔行变色
    ui->tableView->setAlternatingRowColors(true);
    //根据内容自动调整列宽
    ui->tableView->setSortingEnabled(true);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(100);
    ui->tableView->horizontalHeader()->setMaximumSectionSize(200);

    connect(ui->btn_back,SIGNAL(clicked()),this,SLOT(backChange()));
    connect(ui->btn_search,SIGNAL(clicked()),this,SLOT(searchOrder()));
    connect(ui->btn_submit,SIGNAL(clicked()),this,SLOT(submitChange()));
    connect(ui->btn_refresh,SIGNAL(clicked()),this,SLOT(refreshOrder()));
    isCached = true;
    }
}

void OrderHandlePage::submitChange(){
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) {
       model->database().commit(); //提交
    } else {
       model->database().rollback(); //回滚
       QMessageBox::warning(this, tr("tableModel"),tr("数据库错误: %1").arg(model->lastError().text()));
    }
}

void OrderHandlePage::backChange(){
    model->revertAll();
}

void OrderHandlePage::searchOrder(){
    QString restriction =QString("order_user_id='%1'").arg(ui->lineEdit->text());
    //根据进行筛选
    model->setFilter(restriction);
    //显示结果
    model->select();
}

void OrderHandlePage::refreshOrder(){
    model->select(); //选取整个表的所有行
}
