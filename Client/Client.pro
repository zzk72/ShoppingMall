#-------------------------------------------------
#
# Project created by QtCreator 2022-09-08T13:25:55
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += resources_big
SOURCES += \
    Home/home_mywidget.cpp \
    Store/product.cpp \
    Store/productitem.cpp \
    Store/productpage.cpp \
    Store/shoppage.cpp \
    Store/textticker.cpp \
    chatpage.cpp \
    client.cpp \
    loginpage.cpp \
        main.cpp \
    registerpage.cpp \
    slidenavigation.cpp\
    ShoppingCart/cartpage.cpp \
    Home/carouselimagewindow.cpp \
    Home/homepage.cpp \
    Person/mepage.cpp \
    ShoppingCart/cartitem.cpp \
    Person/meitem.cpp \
    Order/orderitem.cpp \
    Order/orderpage.cpp \
    user.cpp



HEADERS += \
    Home/home_mywidget.h \
    Store/product.h \
    Store/productitem.h \
    Store/productpage.h \
    Store/shoppage.h \
    Store/textticker.h \
    chatpage.h \
    client.h \
    loginpage.h \
    registerpage.h \
    slidenavigation.h\
    ShoppingCart/cartpage.h \
    Home/carouselimagewindow.h \
    Home/homepage.h \
    Person/mepage.h \
    ShoppingCart/cartitem.h \
    Person/meitem.h \
    Order/orderitem.h \
    Order/orderpage.h \
    user.h




FORMS += \
    Home/home_mywidget.ui \
    Store/productitem.ui \
    Store/productpage.ui \
    Store/shoppage.ui \
    chatpage.ui \
    client.ui \
    loginpage.ui \
    registerpage.ui \
    ShoppingCart/cartpage.ui \
    Home/carouselimagewindow.ui \
    Home/homepage.ui \
    Person/mepage.ui \
    ShoppingCart/cartitem.ui \
    Person/meitem.ui \
    Order/orderitem.ui \
    Order/orderpage.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc



