#-------------------------------------------------
#
# Project created by QtCreator 2012-05-01T21:03:06
#
#-------------------------------------------------

QT       += core gui network

TARGET = Environment
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    manager.cpp \
    envobject.cpp \
    ../../NetworkingManager.cpp \
    ../../messages-serializers.cpp

HEADERS += \
    manager.h \
    envobject.h \
    constants.h \
    ../../NetworkingManager.h \
    ../../messages.h \
    ../../messages-serializers.h
