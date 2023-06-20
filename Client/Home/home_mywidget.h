#ifndef HOME_MYWIDGET_H
#define HOME_MYWIDGET_H

#include <QWidget>
namespace Ui {
class home_mywidget;
}

class home_mywidget : public QWidget
{
    Q_OBJECT

public:
    explicit home_mywidget(QWidget *parent = nullptr);
    ~home_mywidget();
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    Ui::home_mywidget *ui;
signals:
   void signal_callMeP_click();

};

#endif // HOME_MYWIDGET_H
