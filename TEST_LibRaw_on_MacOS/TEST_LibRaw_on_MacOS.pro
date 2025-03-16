QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG +=precompiled_headers

############################# LibRaw #############################
# MacOS 上可能需要依赖库 jpeg-turbo、lcms2、zlib、icu4c
# 如果缺少执行以下命令安装：brew install jpeg-turbo lcms2 zlib icu4c

DEFINES+=LIBRAW_BUILDLIB
INCLUDEPATH += $$PWD/libraw

unix {
    INCLUDEPATH += /opt/homebrew/include
    LIBS += -L/opt/homebrew/lib -ljpeg -lz -llcms2 -lraw  # Mac 通过 brew 安装
    # LIBS += -L/usr/local/lib -lraw  # 自己编译安装
    # LIBS += -L$$PWD/lib -lraw  # 如果 libraw 库在工程文件夹内部
}


win32 {
    # win32:LIBS+=libraw.lib
}

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    libraw/libraw.h \
    libraw/libraw_alloc.h \
    libraw/libraw_const.h \
    libraw/libraw_datastream.h \
    libraw/libraw_internal.h \
    libraw/libraw_types.h \
    libraw/libraw_version.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
