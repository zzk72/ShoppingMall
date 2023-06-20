#ifndef OrderHandlePage_H
#define OrderHandlePage_H

#include <QWidget>
#include <QSqlTableModel>
#include"manager.h"
#include<QMessageBox>
#include<QSqlRelationalTableModel>
#include<QSqlError>
#include<QSqlRelationalDelegate>
namespace Ui {
class OrderHandlePage;
}

class OrderHandlePage : public QWidget
{
    Q_OBJECT

public:
    explicit OrderHandlePage(QWidget *parent = nullptr);
    ~OrderHandlePage();
    ShoppingManager *manager;
    void showEvent(QShowEvent *event);

private:
    Ui::OrderHandlePage *ui;
    QSqlRelationalTableModel *model;
    bool isCached;
private slots:
    void submitChange();
    void backChange();
    void searchOrder();
    void refreshOrder();
};

#endif // OrderHandlePage_H
