#-------------------------------------------------
#
# Project created by QtCreator 2019-04-18T17:31:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Videostreaming
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

LIBS+=-L"C:\HIWI\Release"

INCLUDEPATH += "C:/HIWI/VideoStreaming/include"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    ffmepg.cpp

HEADERS += \
        mainwindow.h \
    mainwindow.h \
    ffmepg.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    avformat-58.dll \
    avcodec-58.dll \
    testvideo.mp4 \
    avutil-56.dll \
    swscale-5.dll \
    avdevice-58.dll \
    avfilter-7.dll

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/opencv-4.0.1/x64/vc15/lib/ -lopencv_world401
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/opencv-4.0.1/x64/vc15/lib/ -lopencv_world401d

INCLUDEPATH += $$PWD/../../lib/opencv-4.0.1/include
DEPENDPATH += $$PWD/../../lib/opencv-4.0.1/include
