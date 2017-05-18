#-------------------------------------------------
#
# Tau-U Calculator, Qt Port
# Copyright 2017, Shawn P. Gilroy
# Released under the GPL-V3 license
#
# Source code for project hosted at:
#
# https://github.com/miyamot0
#
#-------------------------------------------------

#-------------------------------------------------
#
# Project created by QtCreator 2017-05-06T19:04:36
#
#-------------------------------------------------

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

QT += core gui widgets xlsx charts

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

SOURCES += \
    main.cpp \
    sheetwidget.cpp \
    sheetselectdialog.cpp \
    resultsdialog.cpp \
    licensedialog.cpp \
    creditsdialog.cpp \
    aboutdialog.cpp \
    taudialog.cpp \
    tauumodel.cpp \
    tauucalculations.cpp

HEADERS += \
    aboutdialog.h \
    creditsdialog.h \
    licensedialog.h \
    resultsdialog.h \
    sheetselectdialog.h \
    sheetwidget.h \
    taudialog.h \
    tauumodel.h \
    tauucalculations.h

FORMS += \
    sheetselectdialog.ui \
    resultsdialog.ui \
    licensedialog.ui \
    creditsdialog.ui \
    aboutdialog.ui \
    taudialog.ui

RESOURCES += \
    resources.qrc

win32 {
    TAU_FILES.files = \
                    License_gnome_icons.txt \
                    License_Qt.txt \
                    License_SCR_Calculator.txt \
                    COPYING

    release: DESTDIR = $$OUT_PWD/build/release
    debug:   DESTDIR = $$OUT_PWD/build/debug

    TAU_FILES.path = $$DESTDIR

    INSTALLS += TAU_FILES
}
macx {
    TAU_FILES.files = \
                    License_gnome_icons.txt \
                    License_Qt.txt \
                    License_SCR_Calculator.txt \
                    COPYING

    TAU_FILES.path = Contents/Resources

    QMAKE_BUNDLE_DATA += TAU_FILES
}

DISTFILES += \
    COPYING \
    License_Qt.txt \
    License_gnome_icons.txt \
    License_SCR_Calculator.txt \
    README.md
