#-------------------------------------------------
#
# Project created by QtCreator 2023-03-11T13:54:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloOpenGL
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    csopenglwedgetmag.cpp \
        main.cpp \
        mainwindow.cpp \
    csopenglwedget.cpp \
    csopenglwedgetwave.cpp \
    csopenglwedgetfalse.cpp \
    csopenglwedgetfocus.cpp \
    csopenglwedgetzebra.cpp \
    csopenglwedgetsharpen.cpp \
    csopenglwedgetlut.cpp \
#    csopenglwedgethist.cpp \
#    csopenglwedgetvector.cpp \
    csopenglwedgetfbovector.cpp \
#    csopenglwedgetfbohist.cpp \
    csopenglwedgetfbohistin.cpp \
    csopenglwedgetgreenmatting.cpp \
    mouseevent.cpp \
#    csopenglwedgetlut_copy.cpp

HEADERS += \
    csopenglwedgetmag.h \
        mainwindow.h \
    csopenglwedget.h \
    csopenglwedgetwave.h \
    csopenglwedgetfalse.h \
    csopenglwedgetfocus.h \
    csopenglwedgetzebra.h \
    csopenglwedgetsharpen.h \
    csopenglwedgetlut.h \
#    csopenglwedgethist.h \
#    csopenglwedgetvector.h \
    csopenglwedgetfbovector.h \
#    csopenglwedgetfbohist.h \
    csopenglwedgetfbohistin.h \
    csopenglwedgetgreenmatting.h \
    mouseevent.h

FORMS += \
        mainwindow.ui

#    LIBS += -lopengl32  -lGLU32  -lGLUT

DISTFILES += \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/FALSE.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/FALSE.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/FBOVECTOR.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/FBOVECTOR.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/FOCUS.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/FOCUS.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/GREENMATTING.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/GREENMATTING.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HIST.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HIST.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HIST2D.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HIST2D.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HISTFBO.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HISTFBO.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HISTIN.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/HISTIN.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/LUT.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/LUT.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/SHARPEN.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/SHARPEN.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/VECTOR.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/VECTOR.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/VERTORFBOMAP.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/VERTORFBOMAP.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/VERTORMAP.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/VERTORMAP.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/WAVE.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/WAVE.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/YUYV.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/YUYV.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/ZEBRA.frag \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/Shaders/ZEBRA.vert \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/YUYV/nv12.yuv \
    ../build-HelloOpenGL-Desktop_Qt_5_12_12_GCC_64bit-Debug/YUYV/yuyv.yuv \
    Debug/Shaders/FALSE.frag \
    Debug/Shaders/FALSE.vert \
    Debug/Shaders/FBOVECTOR.frag \
    Debug/Shaders/FBOVECTOR.vert \
    Debug/Shaders/FOCUS.frag \
    Debug/Shaders/FOCUS.vert \
    Debug/Shaders/GREENMATTING.frag \
    Debug/Shaders/GREENMATTING.vert \
    Debug/Shaders/HIST.frag \
    Debug/Shaders/HIST.vert \
    Debug/Shaders/HIST2D.frag \
    Debug/Shaders/HIST2D.vert \
    Debug/Shaders/HISTFBO.frag \
    Debug/Shaders/HISTFBO.vert \
    Debug/Shaders/HISTIN.frag \
    Debug/Shaders/HISTIN.vert \
    Debug/Shaders/LUT.frag \
    Debug/Shaders/LUT.vert \
    Debug/Shaders/SHARPEN.frag \
    Debug/Shaders/SHARPEN.vert \
    Debug/Shaders/VECTOR.frag \
    Debug/Shaders/VECTOR.vert \
    Debug/Shaders/VERTORFBOMAP.frag \
    Debug/Shaders/VERTORFBOMAP.vert \
    Debug/Shaders/VERTORMAP.frag \
    Debug/Shaders/VERTORMAP.vert \
    Debug/Shaders/WAVE.frag \
    Debug/Shaders/WAVE.vert \
    Debug/Shaders/YUYV.frag \
    Debug/Shaders/YUYV.vert \
    Debug/Shaders/ZEBRA.frag \
    Debug/Shaders/ZEBRA.vert \
    Shaders/FALSE.frag \
    Shaders/FALSE.vert \
    Shaders/FBOVECTOR.frag \
    Shaders/FBOVECTOR.vert \
    Shaders/FOCUS.frag \
    Shaders/FOCUS.vert \
    Shaders/GREENMATTING.frag \
    Shaders/GREENMATTING.vert \
    Shaders/HIST.frag \
    Shaders/HIST.vert \
    Shaders/HIST2D.frag \
    Shaders/HIST2D.vert \
    Shaders/HISTFBO.frag \
    Shaders/HISTFBO.vert \
    Shaders/HISTIN.frag \
    Shaders/HISTIN.vert \
    Shaders/LUT.frag \
    Shaders/LUT.vert \
    Shaders/MAG.frag \
    Shaders/MAG.vert \
    Shaders/SHARPEN.frag \
    Shaders/SHARPEN.vert \
    Shaders/VECTOR.frag \
    Shaders/VECTOR.vert \
    Shaders/VERTORFBOMAP.frag \
    Shaders/VERTORFBOMAP.vert \
    Shaders/VERTORMAP.frag \
    Shaders/VERTORMAP.vert \
    Shaders/WAVE.frag \
    Shaders/WAVE.vert \
    Shaders/YUYV.frag \
    Shaders/YUYV.vert \
    Shaders/ZEBRA.frag \
    Shaders/ZEBRA.vert \
    YUYV/nv12.yuv \
    YUYV/yuyv.yuv \
    img/aaa.jpg \
    img/lizard.jpg \
    img/map.jpg \
    img/map2.jpg \
    img/map3.jpg \
    img/tick.jpg \
    img/wt000007.jpg \
    img/wt000023.jpg \
    img/zebra.jpg \
    lut/Canon C-Log.cube \
    lut/Sony S-Log2.cube
