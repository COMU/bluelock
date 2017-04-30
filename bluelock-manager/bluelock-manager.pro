#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T20:32:08
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bluelock-manager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += BT_SCAN_TIME=3000 \
    STATUS_MESSAGE_TIME=3000 \
    CONFIG_PATH=\\\"/etc/bluelock.conf\\\"


SOURCES += main.cpp\
        mainwindow.cpp \
    customwidgets/bluetoothitemwidget.cpp \
    blueconfpp/blueconfpp.cpp \
#    bluetoothlistmodel.cpp \
    bluetoothitem.cpp

HEADERS  += mainwindow.h \
    customwidgets/bluetoothitemwidget.h \
    blueconfpp/blueconfpp.h \
#    bluetoothlistmodel.h \
    bluetoothitem.h

FORMS    += mainwindow.ui \
    customwidgets/bluetoothitemwidget.ui

unix:!macx: LIBS += -L$$PWD/../blueconf/ -lblueconf

INCLUDEPATH += $$PWD/../blueconf
DEPENDPATH += $$PWD/../blueconf
