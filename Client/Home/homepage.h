#ifndef HOMEPAGE_H
#define HOMEPAGE_H
#include "../client.h"
#include <QWidget>
#include "carouselimagewindow.h"
#include"user.h"
#include"Person/mepage.h"
namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(ShoppingClient *parent = nullptr);
    ~HomePage();
    ShoppingClient *client;
    QString pic_path ;
    MePage* mep;

private:
    Ui::HomePage *ui;

public slots:
    void slot_person_ready(Person*);
    void MeP_show();
};

#endif // HOMEPAGE_H
