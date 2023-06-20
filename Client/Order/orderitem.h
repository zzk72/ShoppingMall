#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QWidget>
#include<QJsonArray>
#include<QJsonObject>

//自定义控件类型
//订单内单项
namespace Ui {
class OrderItem;
}

class OrderItem : public QWidget
{
    Q_OBJECT

public:
    OrderItem(QJsonObject obj);
    ~OrderItem();
    //对产品名、图片、数量等进行设置
    void setProname(QString name);
    void setPicture(QString url);
    void setPronum(QString num);
    void setProprice(QString price);
    void setProtolprice(QString tolprice);

private:
    Ui::OrderItem *ui;
};

#endif // ORDERITEM_H
