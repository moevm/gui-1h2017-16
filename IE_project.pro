#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T11:32:49
#
#-------------------------------------------------

QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IE_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    model/finance.cpp \
    services/chartservice.cpp \
    services/dbservice.cpp \
    util/utilenums.cpp \
    windows/categoryform.cpp \
    windows/form.cpp \
    windows/mainwindow.cpp

HEADERS  += model/finance.h \
    services/chartservice.h \
    services/dbservice.h \
    util/utilenums.h \
    windows/categoryform.h \
    windows/form.h \
    windows/mainwindow.h

FORMS    += mainwindow.ui \
    form.ui \
    categoryform.ui
