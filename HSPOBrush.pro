TEMPLATE = lib
DEFINES += HSPOBRUSH_LIBRARY
CONFIG       += plugin
QT           += widgets

TARGET = hspobrush

CONFIG += c++11

isEmpty(LAIGTER_SRC){
  LAIGTER_SRC=../laigter
}


INCLUDEPATH  += $$LAIGTER_SRC/
INCLUDEPATH  += $$LAIGTER_SRC/src/

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
    hsopbrushgui.cpp \
    hspobrush.cpp \
    $$LAIGTER_SRC/src/image_processor.cpp\
    $$LAIGTER_SRC/src/light_source.cpp\
    $$LAIGTER_SRC/src/sprite.cpp\
    $$LAIGTER_SRC//src/texture.cpp
    $$LAIGTER_SRC/src/brush_interface.h

HEADERS += \
  hsopbrushgui.h \
    hspobrush.h \
    $$LAIGTER_SRC/src/image_processor.h \
    $$LAIGTER_SRC/src/light_source.h\
    $$LAIGTER_SRC/src/sprite.h\
    $$LAIGTER_SRC/src/texture.h

FORMS += \
  hsopbrushgui.ui

isEmpty(PREFIX){
unix{
 PREFIX = $$system(echo $HOME)/.local/share/laigter/plugins
}
win32{
 PREFIX = $$system(echo %APPDATA%)/laigter/plugins
}
}

target.path = $$PREFIX/

DESTDIR = $$PREFIX
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

RESOURCES += \
  icons.qrc

DISTFILES += \
  metadata.json



