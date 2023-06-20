QT       += core gui
QT       += sql
QT       += network
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatwgt.cpp \
    main.cpp \
    manager.cpp \
    ordermanagewgt.cpp \
    productwgt.cpp \
    server/handleserver.cpp \
    server/mysocket.cpp \
    server/server.cpp \
    server/sqlserver.cpp \
    server/tcpserver.cpp \
    server/tcpserverthread.cpp \
    slidenavigation.cpp \
    loginwgt.cpp \
    user.cpp

HEADERS += \
    chatwgt.h \
    manager.h \
    ordermanagewgt.h \
    productwgt.h \
    server/handleserver.h \
    server/mysocket.h \
    server/server.h \
    server/sqlserver.h \
    server/tcpserver.h \
    server/tcpserverthread.h \
    slidenavigation.h \
    loginwgt.h \
    user.h

FORMS += \
    chatwgt.ui \
    manager.ui \
    ordermanagewgt.ui \
    productwgt.ui \
    loginwgt.ui

#TRANSLATIONS += \
#    Manager_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
