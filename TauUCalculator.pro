#-------------------------------------------------
#
# Project created by QtCreator 2017-05-06T19:04:36
#
#-------------------------------------------------

QT       += core gui xlsx charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TauUCalculator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    statusdialog.cpp \
    sheetwidget.cpp \
    sheetselectdialog.cpp \
    rworker.cpp \
    resultsdialog.cpp \
    licensedialog.cpp \
    creditsdialog.cpp \
    aboutdialog.cpp \
    taudialog.cpp \
    tauumodel.cpp \
    tauucalculations.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    creditsdialog.h \
    licensedialog.h \
    resultsdialog.h \
    rworker.h \
    sheetselectdialog.h \
    sheetwidget.h \
    statusdialog.h \
    taudialog.h \
    tauumodel.h \
    tauucalculations.h

FORMS    += mainwindow.ui \
    statusdialog.ui \
    sheetselectdialog.ui \
    resultsdialog.ui \
    licensedialog.ui \
    creditsdialog.ui \
    aboutdialog.ui \
    taudialog.ui
