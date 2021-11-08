QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(extern/SFML.pri)
include(extern/QT-Ribbon-Widget/QT-Ribbon-Widget.pri)

srcPath = src
incPath = inc

INCLUDEPATH += $$incPath \
               $$incPath/graphics \
               $$incPath/graphics/shapes \
               $$incPath/core \
               $$incPath/core/tool \

SOURCES += \
    $$srcPath/main.cpp \
    $$srcPath/graphics/mainwindow.cpp \
    $$srcPath/graphics/SFMLCanvas.cpp \
    $$srcPath/graphics/canvas.cpp \
    $$srcPath/graphics/shapes/shape.cpp \
    $$srcPath/graphics/shapes/gate.cpp \
    $$srcPath/graphics/shapes/pin.cpp \
    $$srcPath/core/tool/tool.cpp \
    $$srcPath/core/AABB.cpp \
    src/graphics/shapes/connection.cpp \
    src/graphics/shapes/grid.cpp

HEADERS += \
    $$incPath/graphics/mainwindow.h \
    $$incPath/graphics/SFMLCanvas.h \
    $$incPath/graphics/canvas.h \
    $$incPath/graphics/shapes/shape.h \
    $$incPath/graphics/shapes/gate.h \
    $$incPath/graphics/shapes/pin.h \
    $$incPath/core/tool/tool.h \
    $$incPath/core/AABB.h \
    inc/core/classdef.h \
    inc/core/debug.h \
    inc/core/tool/toollistener.h \
    inc/graphics/shapes/connection.h \
    inc/graphics/shapes/grid.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
