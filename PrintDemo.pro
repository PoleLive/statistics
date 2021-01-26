#-------------------------------------------------
#
# Project created by QtCreator 2021-01-24T11:46:12
#
#-------------------------------------------------

QT       += core gui sql printsupport xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = ../bin/PrintDemo
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


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    TableWidget.cpp \
    PrinterManager.cpp \
    PrintSettingDialog.cpp \
    ConfigManager.cpp \
    HistogramWidget.cpp \
    SqlTableModel.cpp \
    analogclock.cpp \
    digitalclock.cpp

HEADERS += \
        MainWindow.h \
    TableWidget.h \
    PrinterManager.h \
    PrintSettingDialog.h \
    ConfigManager.h \
    HistogramWidget.h \
    SqlTableModel.h \
    analogclock.h \
    digitalclock.h

FORMS += \
        MainWindow.ui

RESOURCES += \
    icon.qrc
