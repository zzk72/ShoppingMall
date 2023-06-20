#include "client.h"
#include"loginpage.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(qApp->applicationDirPath());
    ShoppingClient w;
    w.move(500,10);
    LoginWgt q(&w);
    q.show();
    return a.exec();
}
