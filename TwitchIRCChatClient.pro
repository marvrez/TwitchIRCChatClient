#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T19:41:30
#
#-------------------------------------------------

QT       += core gui webengine webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += communi c++11
COMMUNI += core model util

include(lib/libcommuni/src/src.pri)

TARGET = TwitchIRCChatClient
TEMPLATE = app


INCLUDEPATH += src/

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/LoginWindow/loginwindow.cpp \
    src/ircmanager.cpp \
    src/LoginWindow/browswerwindow.cpp \
    src/Messages/message.cpp

HEADERS  += src/mainwindow.h \
    src/LoginWindow/loginwindow.h \
    src/ircmanager.h \
    src/LoginWindow/browswerwindow.h \
    src/Messages/message.h

RESOURCES += lib/QDarkStyleSheet/qdarkstyle/style.qrc \
             resources/loginwindow.qrc \
             #main.qrc

FORMS    += src/ui/mainwindow.ui \
    src/ui/loginwindow.ui \

DISTFILES += \
    src/LoginWindow/twitchbtnconnect.txt
