#-------------------------------------------------
#
# Project created by QtCreator 2023-09-15T11:11:31
#
#-------------------------------------------------
#author: Cai Yang

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = realtime_communication
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++14

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        TCP/tcper.cpp \
        TOOLS/tool.cpp \
        HTTP/httper.cpp \
        ROS/roser.cpp

HEADERS += \
        mainwindow.h \
        TCP/tcper.h \
        TOOLS/tool.h \
        TOOLS/struct.h \
        HTTP/httper.h \
        ROS/roser.h


FORMS += \
        mainwindow.ui

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local
#INCLUDEPATH += /opt/ros/melodic/include/
INCLUDEPATH += ../../../messages/devel/include/
INCLUDEPATH += ./realtime_msgs/devel/include/realtime_msgs

LIBS += -lstdc++
LIBS += -L/usr/local/lib -lyaml-cpp
#LIBS += /usr/lib/x86_64-linux-gnu/libboost_system*.so
LIBS += -lcurl
#LIBS += -L/opt/ros/melodic/lib -lroscpp -lroslib
#LIBS += /opt/ros/melodic/lib/lib*.so


message("-----------------------------编译环境------------------------------")
ubuntu_version = $$system(lsb_release -rs)
message("ubuntu version : $$ubuntu_version")
contains(QMAKE_HOST.arch, aarch64){
    message("编译目标平台: arm-64")
    message("编译器平台类型: $$QMAKE_HOST.arch")
    LIBS += /usr/lib/aarch64-linux-gnu/libboost_system*.so

    equals(ubuntu_version, "20.04") {
        #orin 20.04
        message(系统类型: $$QMAKE_HOST.os)
        message("系统版本: $$QMAKE_HOST.version")

        DEPENDPATH += /opt/ros/noetic/include
        INCLUDEPATH += /opt/ros/noetic/include
        LIBS += -L/opt/ros/noetic/lib/ -lroscpp -lroslib -lrosconsole
        LIBS += /opt/ros/noetic/lib/lib*.so
    }else:equals(ubuntu_version, "18.04") {
        #xvaier 18.04
        message(系统类型: $$QMAKE_HOST.os)
        message("系统版本: $$QMAKE_HOST.version")
        DEPENDPATH += /opt/ros/melodic/include
        INCLUDEPATH += /opt/ros/melodic/include
        LIBS += -L/opt/ros/melodic/lib/ -lroscpp -lroslib -lrosconsole
        LIBS += /opt/ros/melodic/lib/lib*.so
    }
}else{
    #x86平台
    message("编译目标平台: x86")
    message("编译器平台类型: $$QMAKE_HOST.arch")
    LIBS += /usr/lib/x86_64-linux-gnu/libboost_system*.so

    equals(ubuntu_version, "18.04") {
        #pc 18.04
        message(系统类型: $$QMAKE_HOST.os)
        message("系统版本: $$QMAKE_HOST.version")

        INCLUDEPATH += /opt/ros/melodic/include
        DEPENDPATH += /opt/ros/melodic/include
        LIBS += -L/opt/ros/melodic/lib -lroscpp -lroslib -lrosconsole
        LIBS += /opt/ros/melodic/lib/lib*.so
    }
    else:equals(ubuntu_version, "20.04") {
        message(系统类型: $$QMAKE_HOST.os)
        message("系统版本: $$QMAKE_HOST.version")

        INCLUDEPATH += /opt/ros/noetic/include
        DEPENDPATH += /opt/ros/noetic/include
        LIBS += -L/opt/ros/noetic/lib -lroscpp -lroslib -lrosconsole
        LIBS += /opt/ros/noetic/lib/lib*.so
    }
}

