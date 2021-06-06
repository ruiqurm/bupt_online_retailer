QT       += core gui
QMAKE_CC = /usr/bin/gcc-9
QMAKE_CXX = /usr/bin/g++-9
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
UI_DIR = $$PWD
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addgoodsdialog.cpp \
    cartwidget.cpp \
    categorydiscount.cpp \
    goodsbuying.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    market.cpp \
    profile.cpp \
    registerdialog.cpp \
    tranform.cpp \
    usergoodsmanagement.cpp

HEADERS += \
    adapter.h \
    addgoodsdialog.h \
    cartwidget.h \
    categorydiscount.h \
    goodsbuying.h \
    include/concreteGoods.h \
    include/concreteUser.h \
    include/database.h \
    include/exception.h \
    include/goods.h \
    include/sqlite3.h \
    include/sqlite3ext.h \
    include/user.h \
    logindialog.h \
    mainwindow.h \
    market.h \
    profile.h \
    registerdialog.h \
    tranform.h \
    usergoodsmanagement.h

FORMS += \
    addgoodsdialog.ui \
    cartwidget.ui \
    categorydiscount.ui \
    goodsbuying.ui \
    logindialog.ui \
    mainwindow.ui \
    market.ui \
    profile.ui \
    registerdialog.ui \
    tranform.ui \
    usergoodsmanagement.ui
LIBS += -L$$PWD/libs -lbupt_online_ratailer_static
LIBS += -lpthread\
        -ldl

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
