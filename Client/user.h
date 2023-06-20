#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include<qjsonobject.h>
//用户类
class Person
{
public:
    Person(QJsonObject);
    QString name;
    QString address;
    QString gender;
    QString password;
    int getmoney();
    int getId();
    void setmoney(int num);

private:
    int id;
    int money;

};

#endif // PERSON_H
