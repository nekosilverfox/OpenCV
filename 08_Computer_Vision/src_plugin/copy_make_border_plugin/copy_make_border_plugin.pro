QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += COPY_MAKE_BORDER_LIBRARY
DEFINES += MAKE_CVPLUGIN_LIB

CONFIG += c++17
CONFIG += plugin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../cvplugininterface

SOURCES += \
    copy_make_border_plugin.cpp

HEADERS += \
    ../cvplugininterface/cvplugininterface.h \  # 接口类必须导入为头文件
    copy_make_border_plugin_global.h \
    copy_make_border_plugin.h

# Default rules for deployment.
unix: macx {
    # OpenCV
    INCLUDEPATH += /usr/local/include/opencv4
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_world
}
!isEmpty(target.path): INSTALLS += target

TARGET = CopyMakeBorder_Plugin

FORMS += \
    plugin.ui
