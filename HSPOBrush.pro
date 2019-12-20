TEMPLATE = lib
DEFINES += HSPOBRUSH_LIBRARY
CONFIG       += plugin
QT           += widgets
INCLUDEPATH  += ../laigter/
INCLUDEPATH  += ../laigter/src/

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hspobrush.cpp \
    ../laigter/src/imageprocessor.cpp\
    ../laigter/src/lightsource.cpp \
  hspobrushgui.cpp

HEADERS += \
    hspobrush.h \
    ../laigter/src/imageprocessor.h \
    ../laigter/src/lightsource.h \
  hspobrushgui.h

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

unix{
    CONFIG += link_pkgconfig
    packagesExist(opencv4){
        PKGCONFIG += opencv4
        DEFINES += CV_RGBA2GRAY=COLOR_RGBA2GRAY
        DEFINES += CV_RGB2GRAY=COLOR_RGB2GRAY
        DEFINES += CV_GRAY2RGB=COLOR_GRAY2RGB
        DEFINES += CV_GRAY2RGBA=COLOR_GRAY2RGBA
        DEFINES += CV_DIST_L2=DIST_L2
    } else {
        PKGCONFIG += opencv
    }
}

win32: LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_core320.dll
win32: LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_imgproc320.dll
win32: LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_imgcodecs320.dll

win32: INCLUDEPATH += C:\opencv\build\include

FORMS += \
  formhspobrushgui.ui \
  hspobrushgui.ui
