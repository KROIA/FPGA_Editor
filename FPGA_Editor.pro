QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(extern/SFML.pri)
include(extern/QT-Ribbon-Widget/QT-Ribbon-Widget.pri)
include(extern/SimpleVerilogParser/SimpleVerilogParser.pri)

srcPath = src
incPath = inc

INCLUDEPATH += $$incPath \
               $$incPath/graphics \
               $$incPath/graphics/shapes \
               $$incPath/core \
               $$incPath/core/tool \
               $$incPath/blocks \

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
    $$srcPath/graphics/shapes/connection.cpp \
    $$srcPath/graphics/shapes/grid.cpp \
    $$srcPath/blocks/logicGate.cpp \
    $$srcPath/blocks/logicGate_CONST.cpp \
    $$srcPath/core/physics.cpp \
    src/blocks/ioPin.cpp \
    src/blocks/logicGateImported.cpp \
    src/blocks/module.cpp \
    src/core/projectConfig.cpp

HEADERS += \
    $$incPath/graphics/mainwindow.h \
    $$incPath/graphics/SFMLCanvas.h \
    $$incPath/graphics/canvas.h \
    $$incPath/graphics/shapes/shape.h \
    $$incPath/graphics/shapes/gate.h \
    $$incPath/graphics/shapes/pin.h \
    $$incPath/core/tool/tool.h \
    $$incPath/core/AABB.h \
    $$incPath/core/classdef.h \
    $$incPath/core/debug.h \
    $$incPath/core/tool/toollistener.h \
    $$incPath/graphics/shapes/connection.h \
    $$incPath/graphics/shapes/grid.h \
    $$incPath/blocks/logicGate.h \
    $$incPath/core/physics.h \
    $$incPath/blocks/logicGate_CONST.h \
    inc/blocks/ioPin.h \
    inc/blocks/logicGateImported.h \
    inc/blocks/module.h \
    inc/core/projectConfig.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
