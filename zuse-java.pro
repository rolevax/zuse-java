TEMPLATE = app

QT += qml quick
QT += widgets

CONFIG += c++11

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(release, debug|release): DEFINES += NDEBUG

SOURCES += main.cpp \
    core/doc.cpp \
    core/token.cpp \
    gui/pdoc.cpp \
    ast/ast.cpp \
    ast/listast.cpp \
    ast/scalarast.cpp \
    ast/rootast.cpp \
    ast/parser.cpp \
    ast/bison.cc \
    ast/lex.cpp \
    core/bonetoken.cpp \
    core/fleshtoken.cpp \
    core/soultoken.cpp \
    core/tokens.cpp \
    core/hammer.cpp \
    mode/viewmode.cpp \
    mode/menumode.cpp \
    mode/stringinputmode.cpp \
    mode/numberinputmode.cpp \
    ast/internalast.cpp \
    ast/fixsizeast.cpp \
    mode/identinputmode.cpp \
    ast/boplistast.cpp \
    mode/listinputmode.cpp \
    core/keycode.cpp \
    mode/tilexmode.cpp \
    mode/fixsizeinputmode.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

INCLUDEPATH += "include"

HEADERS += \
    include/core/doc.h \
    include/core/token.h \
    include/ast/ast.h \
    include/ast/listast.h \
    include/gui/pdoc.h \
    include/ast/scalarast.h \
    include/ast/rootast.h \
    include/core/bonetoken.h \
    include/core/fleshtoken.h \
    include/core/soultoken.h \
    include/core/tokens.h \
    include/core/hammer.h \
    include/core/tokensobserver.h \
    include/mode/mode.h \
    include/mode/viewmode.h \
    include/mode/menumode.h \
    include/mode/stringinputmode.h \
    include/mode/numberinputmode.h \
    include/core/editabledoc.h \
    include/ast/internalast.h \
    include/ast/fixsizeast.h \
    include/mode/identinputmode.h \
    include/ast/boplistast.h \
    include/mode/listinputmode.h \
    include/core/keycode.h \
    include/mode/tilexmode.h \
    include/mode/fixsizeinputmode.h

