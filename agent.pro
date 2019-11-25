#-------------------------------------------------
#
# Project created by QtCreator 2018-12-17T12:56:44
#
#-------------------------------------------------

QT       += core gui printsupport



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = agent
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_WARNING_OUTPUT\QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBSPATH = ""
INCLUDEPATH += "/usr/local/include/libusb-1.0"
LIBS += "/usr/local/lib/libusb-1.0.a" -lm

SOURCES += main.cpp\
        mainwindow.cpp \
    print.cpp \
    q_cups.cpp \
    tool.cpp

HEADERS  += mainwindow.h \
    tool.h \
    print.h \
    q_cups_p.h \
    libusbt.h



FORMS    += mainwindow.ui
