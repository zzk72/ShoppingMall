#ifndef LOGINWGT_H
#define LOGINWGT_H

#include <QWidget>
#include<QPaintEvent>
#include<QPainter>
#include<QMessageBox>
#include<QRegExp>
#include<QRegExpValidator>
#include"client.h"
#include<QJsonObject>
#include<QJsonArray>
#include<QKeyEvent>
#include"registerpage.h"
namespace Ui {
class LoginWgt;
}

class LoginWgt : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWgt(ShoppingClient *p);
    ~LoginWgt();
    ShoppingClient *client;

private:
    Ui::LoginWgt *ui;
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void loginUser();
    void registerUser();
signals:
    void signal_login(QJsonObject);
};

#endif // LOGINWGT_H
