#-------------------------------------------------
#
# Project created by QtCreator 2019-05-23T16:10:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photogrammetry
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

CONFIG += c++11

SOURCES += \
        EXoperator.cpp \
        intersection.cpp \
        main.cpp \
        mainwindow.cpp \
        matching.cpp \
        orientation.cpp \
        resection.cpp

HEADERS += \
        EXoperator.h \
        intersection.h \
        mainwindow.h \
        matching.h \
        orientation.h \
        resection.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += E:\photo\library\eigen

INCLUDEPATH += E:\photo\library\opencv\include #*（这个是opencv解压以后存放的路径+所选的文件）*
CONFIG(debug, debug|release): {
LIBS += E:\photo\library\opencv\x86\mingw\lib\libopencv_*.a

} else:CONFIG(release, debug|release): {

LIBS += E:\photo\library\opencv\x86\mingw\lib\libopencv_*.a

}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
