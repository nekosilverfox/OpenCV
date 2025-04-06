QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../CvPluginInterface/CvPluginInterface.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

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
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
