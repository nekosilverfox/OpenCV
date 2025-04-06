QT -= gui

TEMPLATE = lib
DEFINES += GAUSSIAN_FILTER_PLUGIN_LIBRARY

CONFIG += c++17 plugin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gaussian_filter_plugin.cpp

HEADERS += \
    CvPluginInterface.h \
    gaussian_filter_plugin_global.h \
    gaussian_filter_plugin.h


unix: macx {
    # 其他依赖库
    INCLUDEPATH += /opt/homebrew/include
    LIBS += -L/opt/homebrew/lib
    LIBS += -L/usr/local/lib

    # OpenCV
    INCLUDEPATH += /usr/local/include/opencv4
    LIBS += -L/opt/homebrew/opt/opencv/lib
    LIBS += -lopencv_world
    INCLUDEPATH += /opt/homebrew/opt/opencv/include/opencv4
    # LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs # 通过 brew 安装的
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
