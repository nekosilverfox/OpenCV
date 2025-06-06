
QT       += widgets

TARGET = Filter_Plugin
TEMPLATE = lib

CONFIG += plugin

DEFINES += FILTER_PLUGIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../cvplugininterface

SOURCES += \
        filter_plugin.cpp

HEADERS += \
        filter_plugin.h \
        filter_plugin_global.h \
        ../cvplugininterface/cvplugininterface.h

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
