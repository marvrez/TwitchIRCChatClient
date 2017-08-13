#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T19:41:30
#
#-------------------------------------------------

QT       += core gui webkitwidgets webkit

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
    src/Messages/message.cpp \
    src/ChatWidget/chatwidget.cpp \
    src/ChatWidget/resources.cpp \
    src/ChatWidget/channel.cpp \
    src/Messages/emotemanager.cpp \
    src/ChatWidget/userstate.cpp \
    src/ChatWidget/resizingtextedit.cpp \
    src/ChatWidget/completionmanager.cpp

HEADERS  += src/mainwindow.h \
    src/LoginWindow/loginwindow.h \
    src/ircmanager.h \
    src/LoginWindow/browswerwindow.h \
    src/Messages/message.h \
    src/Messages/mentionmanager.h \
    src/ChatWidget/chatwidget.h \
    src/ChatWidget/resources.h \
    src/ChatWidget/channel.h \
    src/Messages/emotemanager.h \
    src/ChatWidget/userstate.h \
    src/ChatWidget/resizingtextedit.h \
    src/ChatWidget/completionmanager.h

RESOURCES += lib/QDarkStyleSheet/qdarkstyle/style.qrc \
             resources/loginwindow.qrc \
    src/ChatWidget/chatwindow.qrc \
    resources/badges/badges.qrc

FORMS    += src/ui/mainwindow.ui \
    src/ui/loginwindow.ui \
    src/ChatWidget/chatwidget.ui

DISTFILES += \
    src/LoginWindow/twitchbtnconnect.txt \
