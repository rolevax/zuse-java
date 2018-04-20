TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += NDEBUG
}

HEADERS += gui/pdoc.h

SOURCES += main.cpp \
    gui/pdoc.cpp

RESOURCES += qml.qrc

include(libzuse.pri)
include(deployment.pri)

MOC_DIR = ./moc
RCC_DIR = ./qrc
OBJECTS_DIR = ./obj

