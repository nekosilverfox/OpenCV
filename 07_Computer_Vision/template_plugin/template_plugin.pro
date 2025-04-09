QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Template_Plugin
TEMPLATE = lib

CONFIG += c++17
CONFIG += plugin

DEFINES += TEMPLATE_PLUGIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../cv_plugin_interface

SOURCES += \
        template_plugin.cpp

HEADERS += \
        template_plugin.h \
        template_plugin_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: {
    include("c:/dev/opencv/opencv.pri")
}

unix: !macx{
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

unix: macx {
    # OpenCV
    INCLUDEPATH += /usr/local/include/opencv4
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_world
}

FORMS += \
    plugin.ui
