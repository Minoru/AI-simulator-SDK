#-------------------------------------------------
#
# Project created by QtCreator 2012-04-01T15:35:01
#
#-------------------------------------------------

QT       += core gui network

TARGET = Robot
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    robot.cpp \
    manager.cpp \
    ../../NetworkingManager.cpp \
    ../../messages-serializers.cpp

HEADERS += \
    robot.h \
    ../../constants.h \
    manager.h \
    ../../NetworkingManager.h \
    ../../messages.h \
    ../../messages-serializers.h
